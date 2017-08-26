#ifndef BIGNUMBERIMPL_H
#define BIGNUMBERIMPL_H

#include <string>
#include <iostream>

class BignumberImpl
{
private:
    std::string numStr_;
    size_t dotPos_;//小数点的位置
    enum class Mark{POSITIVE,NEGETIVE};//代表正负数
    Mark mark_;//数值符号


public:
    explicit BignumberImpl(const std::string & value = "");

    BignumberImpl(const BignumberImpl & value)    =   default;
    BignumberImpl(BignumberImpl && value)   =   default;
    BignumberImpl& operator = (const BignumberImpl & value)   =   default;
    BignumberImpl& operator = (BignumberImpl && value)  =   default;

    inline size_t getIntegerSize() const {return numStr_.size() - dotPos_;}//返回整数部分的长度
    inline size_t getDecimalSize() const {return dotPos_;}//返回小数部分的有效长度
    inline size_t getPrecision() const {return dotPos_;}//数值的精度也即小数部分有效位的大小
    std::string getValue() const;//返回一个格式化之后的数值字符串
    //返回数值字符串的真实大小，也即经过格式化后的数值字符串的大小
    inline size_t size() const {return dotPos_ == 0 ? numStr_.size() + 1 : numStr_.size() + 2;}
    BignumberImpl getNegete() const;//取得相反数
    BignumberImpl getAbs() const;//取得绝对值

    inline bool isPositive() const {return mark_ == Mark::POSITIVE;}
    inline bool isNegetive() const {return mark_ == Mark::NEGETIVE;}

    BignumberImpl operator + (const BignumberImpl& other);
    BignumberImpl& operator += (const BignumberImpl& other);
    BignumberImpl operator - (const BignumberImpl& other);
    BignumberImpl& operator -= (const BignumberImpl& other);

    bool operator == (const BignumberImpl & other) const;
    bool operator != (const BignumberImpl & other) const;
    bool operator < (const BignumberImpl & other) const;
    bool operator > (const BignumberImpl & other) const;
    bool operator <= (const BignumberImpl & other) const;
    bool operator >= (const BignumberImpl & other) const;

    friend std::istream& operator >> (std::istream& in,BignumberImpl& num);
    friend std::ostream& operator << (std::ostream& out,const BignumberImpl& num);
    friend class Bignumber;
};

extern std::istream& operator >> (std::istream& in,BignumberImpl& num);

extern std::ostream& operator << (std::ostream& out,const BignumberImpl& num);




/***********************************************
 *  对代表数值的字符串进行格式化：清除数字前或后无意义的0
 * 对任意一个合法的数值字符串，格式化后变为以下形式：
 * 1.字符串第0位为符号位：+、-
 * 2.整数格式化后不包含小数点，整数开头的0会被清除
 * 3.如果存在小数部分，则小数部分之后的0会被清除
 ***********************************************/
extern void formatString(std::string & numStr);

#endif // BIGNUMBERIMPL_H
