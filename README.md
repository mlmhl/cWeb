程序架构如下(仿照Golang的处理方式)：
 		
	|
	---- http
	|	  |     
	|	  ---- URL(是否需要像Golang一样那么细粒度？暂且不)
	|	  |
	|	  ---- Header
	|	  |
	|	  ---- Request
	|	  |
	|	  ---- Response
	|	  |
	|	  ---- Server
	|
	---- util
	|     |
	|	  ---- HashTable(可以提供一个针对字符串的hash函数)
	|
	---- log(日志输出)
