# -
构造一个简单编译器
词法分析
语法分析
中间代码生成


1. 目前要用的文件：

   - LexicalAnalysis.h——总头文件
   - SyntaxAnalysis.cpp——语法分析+中间代码生成
   - testLex.h——词法分析（因为语法分析要用就改成头文件形式加进来）

2. 测试输入是两个：input.txt和input2.txt文件

3. 输出是：output.txt文件

   

   备注：

   只用到了上面几个文件，其他的都可以删掉，然后ltq的代码因为他是输入文件的形式所以语法分析不能调用，我改成我们这里的testLex.h文件，他的头文件原来的内容加在了LexicalAnalysis.h一起，然后没用他的两个文件（在github上）