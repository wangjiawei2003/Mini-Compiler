# 项目简介
构造一个简单编译器
词法分析
语法分析
中间代码生成

## 文件说明
### 头文件
1. Common.h 宏定义（程序参数设置）以及枚举类型
2. CommonVar.h 全局变量定义
3. LexicalAnalysis.h  词法分析对应头文件 
4. SyntaxAnalysis.h 语法分析+中间代码生成对应头文件

### cpp
1. main.cpp main函数所在，程序开始，负责读取输出文件
2. LexicalAnalysis.cpp  词法分析
3. SyntaxAnalysis.cpp 语法分析+中间代码生成
---
语法规则以函数的形式实现

1. 比如 constdeclaration() 处理常量声明，
2. vardeclaration() 处理变量声明，
3. statement() 处理语句等。


void constDeclaration()中，获取变量名称

   

   