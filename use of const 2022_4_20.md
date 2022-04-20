# The Usage of Const
对变量而言，const变量代表常量，不能对常量进行赋值或更改操作，故常量必须初始化。  
    const int a; //error!  
const变量默认仅在本文件中作用，想要实现共享的话，在头文件中定义const变量并加上extern是最好的选择。  
    in headfile.h: extern const a = 114514;  

