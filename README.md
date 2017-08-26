** 为了解决编程中int、long、double等常见数据类型数值范围的限制，
设计了Bignumber类并实现基本的加减和乘法。**

## Bignumber类型简介

Bignumber可以代表无限大的整数、小数，并实现了基本的加、减和乘法

## Bignumber实现

Bignumber的实现采用了pimpl模式，其类成员数据是一个类型为shared_ptr<BignumberImpl>的智能指针，BignumberIMPL中包含了所有的实现细节，依此来达到实现和接口分离的目的。

BignumberImpl了数据成员包括：
-	std::string numStr_:用来表示数值，其中只包含纯数字，不含其他符号
-	dotPos_:用来表示小数点的位置，方便对小数进行运算
-	mark_:数字的符号

## Bignumber接口：
- ==,!=,<,>,<=,>= : 比较运算
- +,-,* : 加、减和乘法
- getNegete() : 取反
- getAbs() ： 取绝对值

