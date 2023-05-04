#### 1.词法分析（lexical analysis）

将输入的字符串分隔为一个个单词或者符号，表现形式为token序列，例如以下c程序

```c
int main() {
    int a = 1 + 2;
    return a;
}
```

其token序列为

```
Keyword: int
Identifier: main
Symbol: (
Symbol: )
Symbol: {
Keyword: int
Identifier: a
Symbol: =
Number: 1
Symbol: +
Number: 2
Symbol: ;
Keyword: return
Identifier: a
Symbol: ;
Symbol: }
```



#### 2.语法分析（parse）

将一个个单词和符号，其实也就是token序列转换为抽象语法树（AST，一种树形结构）



#### 2.N抽象语法（abstract syntax）

在语法分析的过程中，通常会将语法树进行抽象化，得到一个更加简洁、清晰的抽象语法树（Abstract Syntax Tree，AST）。抽象语法树通常会去除语法树中的冗余信息，只保留程序的核心语义结构，以便后续的编译过程进行处理。



#### 3.语义分析

检查程序语法的正确性和含义的准确性，以及对程序进行类型检查、计算常量表达式、检查变量是否被定义等操作，以便为代码生成阶段提供必要的信息。