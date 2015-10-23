/**
  * @file log.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 16:59:19 2015
  */


#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "log.h"
#include "../util/vector.h"
#include "../algo/str.h"


int logger_init(logger *log, int flag, const char *out) {
	log->flag = flag;
	
	log->out = open(out, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (log->out == -1) {
		return ERR_CREATE_LOG_FILE;
	}
	
	// initail buffer
	int retCode = buffer_init(&(log->buf), 0);
	if (retCode > 0) {
		close(log->out);
		return retCode;
	}

	// create and initial the attributes for lock
	pthread_mutexattr_t mtx_attr;
	retCode = pthread_mutexattr_init(&mtx_attr);
	if (retCode > 0) {
		return retCode;
	}
	retCode = pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_ERRORCHECK);
	if (retCode > 0) {
		return retCode;
	}

	// initial lock
	retCode = pthread_mutex_init(&(log->lock), &mtx_attr);
	pthread_mutexattr_destroy(&mtx_attr);
	if (retCode > 0) {
		close(log->out);
		return retCode;
	}

	return 0;
}


int logger_destroy(logger *log) {
	int out_ret_code = close(log->out);
	if (out_ret_code == -1) {
		return ERR_DESTROY_OUT_FILE;
	}

	int buffer_ret_code = buffer_destroy(&(log->buf));
	if (buffer_ret_code > 0) {
		return buffer_ret_code;
	}

	int lock_ret_code = pthread_mutex_destroy(&(log->lock));
	if (lock_ret_code > 0) {
		return ERR_DESTROY_MUTEX;
	}
	
	return 0;
}


static int lock(logger *log) {	
	return pthread_mutex_lock(&(log->lock));
}

static int unlock(logger *log) {
	return pthread_mutex_unlock(&(log->lock));
}


static int append_time_element(buffer *buf, vector *tsp, unsigned int index) {
	void *str = NULL;
	int retCode = v_get_ptr(tsp, index, &str);
	if (retCode > 0) {
		return retCode;
	}
	retCode = buffer_append(buf, str, strlen((char*)str));
	if (retCode > 0) {
		return retCode;
	}
	return buffer_append(buf, " ", 1);
}


static int format_time(logger *log) {
	vector tsp;
	vector_init(&tsp, 0);
	
	time_t ti = time(NULL);
	char *time_str = (char*)(ctime(&(ti)));
	str_split(time_str, " \n", &tsp);
	int retCode = 0;
	if (log->flag & DATE) {
		// append month
		if ((retCode = append_time_element(&(log->buf), &tsp, 1)) > 0) {
			return retCode;
		}
		// append day
		if ((retCode = append_time_element(&(log->buf), &tsp, 2)) > 0) {
			return retCode;
		}
		// append year
		if ((retCode = append_time_element(&(log->buf), &tsp, 4)) > 0) {
			return retCode;
		}
	}
	if (log->flag & TIME) {
		// append time
		if ((retCode = append_time_element(&(log->buf), &tsp, 3)) > 0) {
			return retCode;
		}
	}

	return 0;
}


static int append_path_element(buffer *buf, const char *str) {
	int retCode = buffer_append(buf, (void*)str, strlen(str));
	if (retCode) {
		return retCode;
	}
	return buffer_append(buf, " ", 1);
}


static int format_file_path(logger *log) {
	int retCode = 0;
	if (log->flag & SFILE) {
		if ((retCode = buffer_append(&(log->buf), "[", 1)) > 0) {
			return retCode;
		}
		if ((retCode = append_path_element(&(log->buf), __FILE__)) > 0) {
			return retCode;
		}
		char line_buf[20];
		if ((retCode = append_path_element(&(log->buf), itoa(__LINE__, line_buf, 10))) > 0) {
			return retCode;
		}
		buffer_reduce(&(log->buf), 1);
		if ((retCode = buffer_append(&(log->buf), "] ", 2)) > 0) {
			return retCode;
		}
	}

	return 0;
}


static int format_tag(buffer *buf, const char *tag) {
	int retCode = 0;
	if ((retCode = buffer_append(buf, "[", 1)) > 0) {
		return retCode;
	}
	if ((retCode = buffer_append(buf, (void*)tag, strlen(tag))) > 0) {
		return retCode;
	}
	if ((retCode = buffer_append(buf, "] ", 2)) > 0) {
		return retCode;
	}
	return retCode;
}


static int output(logger *log, const char *tag, const char *content) {
	int retCode = 0;

	lock(log);

	if ((retCode = format_time(log)) > 0) {
		unlock(log);
		return retCode;
	}
	if ((retCode = format_file_path(log)) > 0) {
		unlock(log);
		return retCode;
	}
	if ((retCode = format_tag(&(log->buf), tag)) > 0) {
		unlock(log);
		return retCode;
	}

	retCode = buffer_append(&(log->buf), (void*)content, strlen(content));
	if (retCode > 0) {
		unlock(log);
		return retCode;
	}
	retCode = buffer_append(&(log->buf), "\n", 1);
	if (retCode > 0) {
		unlock(log);
		return retCode;
	}

	if (buffer_size(&(log->buf)) > THR) {
		unlock(log);
		return logger_flush(log);
	}

	return unlock(log);
}


int logger_printf(logger *log, const char *format, ...) {
	va_list start;
	va_start(start, format);
	//TODO log will be trunc if length exceed 2048
	char buf[2048];
	vsnprintf(buf, 2048, format, start);

	return output(log, "INFO", buf);
}


int logger_flush(logger *log) {
	lock(log);
	size_t size = buffer_size(&(log->buf));
	int retCode = write(log->out, buf_to_str(&(log->buf)), size);
	if (retCode == -1) {
		unlock(log);
		return retCode;
	}
	
	if ((retCode = buffer_reduce(&(log->buf), size)) > 0) {
		unlock(log);
		return retCode;
	}

	return unlock(log);
}
