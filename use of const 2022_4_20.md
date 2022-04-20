# The Usage of Const
## const变量
&emsp;对变量而言，const变量代表常量，不能对常量进行赋值或更改操作，故常量必须初始化。  

        const int a; //error!  
&emsp;const变量默认仅在本文件中作用，想要实现共享的话，在头文件中定义const变量并加上extern是最好的选择。  

        in headfile.h: extern const a = 114514;  
## const与引用  
### const引用
&emsp;对于引用，我们也可以附加const属性，const引用禁用了修改操作，比如  
  
        int a = 1;  
        const int &b=a;&emsp;//这种情况下不能通过b来对a进行修改，尽管a是个变量。  
        
### 对const的引用
&emsp;上面我们了解了const引用。那么对于const变量的引用呢？实际上，对const变量**只能**使用const引用！  
  
        const int a = 1;  
        int &b = a;&emsp;//ERROE!  
        
&emsp;但是对于变量进行const引用却是合法的，上面的例子已经给出了。  
## const与指针  
### 指向常量的指针  
       const int *ptr;
&emsp;我们知道，指针和引用有着相似的性质，所以const和引用的关系和其与指针的关系类似  
&emsp;1. 常量指针不能被用于改变其所指对象的值；  
&emsp;2. 常量的地址只能由指向常量的指针存放，但是变量没有这个要求；  
  
### 常量指针  
        int *const ptr;  
&emsp;指针本身也是一种变量，那么常量指针的含义也不言自明了.  
&emsp;当然，指向常量的常量指针也是存在的： 

        const int *const ptr;//有一种野性的美（  
        
## 常量表达式  
&emsp;一个量是否为常量，对于我们来说并不敏感，但是对编译器来说却是截然不同的。**常量表达式**指值不会改变并且在编译过程中就能得到结果的表达式。字面值（比如114514）就是典型的常量表达式.



