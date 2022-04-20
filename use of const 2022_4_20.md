# The Usage of Const
## const变量
对变量而言，const变量代表常量，不能对常量进行赋值或更改操作，故常量必须初始化。  
    *const int a; //error!*  
const变量默认仅在本文件中作用，想要实现共享的话，在头文件中定义const变量并加上extern是最好的选择。  
    *in headfile.h: extern const a = 114514;*  
  
## const引用
对于引用，我们也可以附加const属性：  
const引用禁用了修改操作，比如  
*int a = 1;
const int &b=a;  *  
这种情况下不能通过b来对a进行修改，尽管a是个变量。
