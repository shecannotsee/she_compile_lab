该文档描述了一种简易语言的设计。



- 变量定义

```
a is an integer,The value of a is (10);
# 简单赋值
a will become (10);
# 表达式赋值
a will become (10+2+4);
```

- 循环与判断

```
# 值判断
if the value of a is (10),then:
  b will become (20);
else:
  b will become (10);

# 类型判断
if the type of a is integer,then:
  a will becont (50);
```

- 函数定义，函数调用

```
function add_two_number return integer,
receiving 
  parameter 1 aaaa of type integer , 
  parameter 2 bbbb of type integer :
    aaaa will become (10);
    bbbb will become (20);


```



#### 以上先遗弃了

- 变量定义

  ```
  int a = 10;
  ```

- 算术运算

- 判断与循环

- 函数定义，函数调用

#### 算了，还是先写个c99编译器吧

设计一门语言对当前的我来说太难了

## C99

#### 1.keywords

变量：bool,char,double,float,int

变量修饰：const,unsigned

分支：if,else,switch,case,default

循环：do,while,for,goto

分支控制：break,continue

函数与组合：return,struct,union

全局修饰：static

其他：void,enum

