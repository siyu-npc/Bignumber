#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include "bignumberimpl.h"
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

class Bignumber
{
private:
    boost::shared_ptr<BignumberImpl> value_;

    Bignumber(const BignumberImpl & value) {value_ = boost::shared_ptr<BignumberImpl> (new BignumberImpl(value));}

public:
    Bignumber();
    //Bignumber(double value);
    Bignumber(const char* value,size_t n);//value must be a valid number,and n is the length of value
    Bignumber(const std::string& value);//initial by std::string
    Bignumber(long long num);//initial by long long
//    Bignumber(double num);//initial by double

    Bignumber(const Bignumber& other);//copy constructor
    Bignumber& operator = (const Bignumber& other);//asign constructor

    Bignumber(Bignumber&& other);
    Bignumber& operator = (Bignumber&& other);

    inline size_t getPrecision() const {return (*value_).getPrecision();}//数值的精度也即小数部分有效位的大小
    inline std::string getValue() const {return value_->getValue();}//返回一个格式化之后的数值字符串
    //返回数值字符串的真实大小，也即经过格式化后的数值字符串的大小
    inline size_t size() const {return value_->size();}
    Bignumber getNegete() const ;//取得相反数

    inline bool isPositive() const {return value_->isPositive();}
    inline bool isNegetive() const {return value_->isNegetive();}

    friend bool operator == (const Bignumber & num1,const Bignumber & num2);
    friend bool operator != (const Bignumber & num1,const Bignumber & num2);
    friend bool operator < (const Bignumber & num1,const Bignumber & num2);
    friend bool operator > (const Bignumber & num1,const Bignumber & num2);
    friend bool operator <= (const Bignumber & num1,const Bignumber & num2);
    friend bool operator >= (const Bignumber & num1,const Bignumber & num2);

    friend Bignumber operator + (const Bignumber& num1,const Bignumber& num2);
    Bignumber& operator += (const Bignumber& other);
    friend Bignumber operator - (const Bignumber& num1,const Bignumber& num2);
    Bignumber& operator -= (const Bignumber& other);

    friend std::istream& operator >> (std::istream& in,Bignumber& num);
    friend std::ostream& operator << (std::ostream& out,const Bignumber& num);
    ~Bignumber();
};

Bignumber operator + (const Bignumber& num1,const Bignumber& num2);
Bignumber operator - (const Bignumber& num1,const Bignumber& num2);

extern std::istream& operator >> (std::istream& in,Bignumber& num);
extern std::ostream& operator << (std::ostream& out,const Bignumber& num);

extern bool operator != (const Bignumber & num1,const Bignumber & num2);
extern bool operator < (const Bignumber & num1,const Bignumber & num2);
extern bool operator > (const Bignumber & num1,const Bignumber & num2);
extern bool operator <= (const Bignumber & num1,const Bignumber & num2);
extern bool operator >= (const Bignumber & num1,const Bignumber & num2);

extern Bignumber operator + (const Bignumber& num1,const Bignumber& num2);
extern Bignumber operator - (const Bignumber& num1,const Bignumber& num2);

#endif // BIGNUMBER_H
