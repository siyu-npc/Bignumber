#include "bignumberimpl.h"
#include "gtest/gtest.h"
#include <regex>


void formatString(std::string & numStr){
    std::string tmp;
    size_t nonZeroBegin = 0;
    if (numStr[0] == '-' || numStr[0] == '+') {
        tmp.push_back(numStr[0]);
        nonZeroBegin = numStr.find_first_not_of('0',1);
    }
    else{
        tmp.push_back('+');
        nonZeroBegin = numStr.find_first_not_of('0');
    }
    if (nonZeroBegin == std::string::npos){
        numStr = "+0";
        return;
    }
    size_t endPos = 0,dotPos = 0;
    if ((dotPos = numStr.find('.')) != std::string::npos){
        size_t nonZeroEnd   = numStr.find_last_not_of('0');
        if (dotPos < nonZeroEnd) endPos = nonZeroEnd;
        else endPos = dotPos - 1;
    }
    else endPos = numStr.size() - 1;
    if (nonZeroBegin == dotPos) nonZeroBegin -= 1;
    do {
        tmp.push_back(numStr[nonZeroBegin]);
    } while ((++nonZeroBegin) <= endPos);
    numStr = tmp;
}

TEST(FormatString,formatStringOfNumber){
    const char * inputCases[] = {"","0","0000","+000123","-000123","000123.","+000123000","-000123000",\
                                 "0.0","00012.23","+000123.23","-000123.23","0001.00","000123.000","0.000222","000.0222"};
    const char * checkCases[] = {"+0","+0","+0","+123","-123","+123","+123000","-123000",\
                                 "+0","+12.23","+123.23","-123.23","+1","+123","+0.000222","+0.0222"};
    for (size_t i = 0;i < sizeof(inputCases) / sizeof(char *);++i){
        std::string tmp = inputCases[i];
        formatString(tmp);
        EXPECT_EQ(tmp,std::string(checkCases[i]))<<" inputCases["<<i<<"] is failed!";
    }
}

std::string BignumberImpl::getValue() const {
    std::string result(this->size(),'0');
    if (isNegetive()) result[0] = '-';
    else result[0] = '+';
    size_t i = 1;
    for (;i <= getIntegerSize();++i){
        result[i] = numStr_[i - 1];
    }
    if (getPrecision() != 0) {
        result[i] = '.';
        for (;i <= size() - 2;++i) result[i + 1] = numStr_[i - 1];
    }

    return result;
}
TEST(getValue,getValueCase){
    const char * inputCases[] = {"","+0","+123","-123","+123","+1123.322","+1123.322","-1123.322","+1234.5678"};
    const char * checkCases[] = {"+0","+0","+123","-123","+123","+1123.322","+1123.322","-1123.322","+1234.5678"};
    for (size_t i = 0;i < sizeof(inputCases) / sizeof(char*);++i){
        BignumberImpl tmp(inputCases[i]);
        EXPECT_EQ(tmp.getValue(),checkCases[i]);
    }
}

/******************************************
 * 两个代表整数的数值字符串相加，
 * NOTE：这两个字符串均应为格式化后的字符串，在本程序
 * 中用于两个BignumberImpl相加的内部实现
 * 返回的字符串为格式化后的字符串
 * ****************************************/
static std::string positiveAdd(const std::string & num1,size_t dot1,const std::string & num2,size_t dot2){
    //分别对两个数值字符串的整数和小数部分进行补零
    //整数部分补零：
    int intSizeNum1 = dot1 == 0 ? num1.size() - 1 : num1.size() - dot1 - 2;
    int intSizeNum2 = dot2 == 0 ? num2.size() - 1 : num2.size() - dot2 - 2;
    std::string integerNum1(std::max(intSizeNum1,intSizeNum2),'0');
    std::string integerNum2(std::max(intSizeNum1,intSizeNum2),'0');

    for (int i = integerNum1.size() - 1;i >= 0 && intSizeNum1 > 0;--i,--intSizeNum1)
        integerNum1[i] = num1[intSizeNum1];
    for (int i = integerNum2.size() - 1;i >= 0 && intSizeNum2 > 0;--i,--intSizeNum2)
        integerNum2[i] = num2[intSizeNum2];

    //小数部分补零
    std::string decimalNum1,decimalNum2;
    if (dot1 != 0 || dot2 != 0){
        decimalNum1 = std::string(std::max(dot1,dot2),'0');
        decimalNum2 = std::string(std::max(dot1,dot2),'0');
        for (size_t i = 0;i < decimalNum1.size() && num1.size() - dot1 + i < num1.size();++i)
            decimalNum1[i] = num1[num1.size() - dot1 + i];
        for (size_t i = 0;i < decimalNum2.size() && num2.size() - dot2 + i < num2.size();++i)
            decimalNum2[i] = num2[num2.size() - dot2 + i];
    }

    //开始进行计算
    int flag = 0;//进位标识
    //首先计算小数部分
    for (int i = decimalNum1.size() - 1;i >= 0;--i){
        int tmp = decimalNum1[i] - '0' + decimalNum2[i] - '0' + flag;
        if (tmp >= 10) flag = 1;else flag = 0;
        decimalNum1[i] = tmp % 10 + '0';
    }
    //计算整数部分
    for (int i = integerNum1.size() - 1;i >= 0;--i){
        int tmp = integerNum1[i] - '0' + integerNum2[i] - '0' + flag;
        if (tmp >= 10) flag = 1;else flag = 0;
        integerNum1[i] = tmp % 10 + '0';
    }

    std::string result;
    if (flag == 1) {
        if (decimalNum1.size() == 0)
            result = "1" + integerNum1;
        else result = "1" + integerNum1 + "." + decimalNum1;
    }
    else {
        if (decimalNum1.size() == 0)
            result = integerNum1;
        else result = integerNum1 + "." + decimalNum1;
    }

    //格式化结果并返回
    formatString(result);
    return result;
}
TEST(Aalculate,AddTwoPositive){
    EXPECT_EQ(positiveAdd("+1234",0,"+1234",0),std::string("+2468"));
    EXPECT_EQ(positiveAdd("+9999999999999999999999999999",0,"+9999999999999999999999999999",0),"+19999999999999999999999999998");
    EXPECT_EQ(positiveAdd("+1234.01",2,"+1234.01",2),"+2468.02");
    EXPECT_EQ(positiveAdd("+9999999999999999999999999999.999999999999999",15,"+9999999999999999999999999999.999999999999999",15),\
              "+19999999999999999999999999999.999999999999998");
    EXPECT_EQ(positiveAdd("+9999999999999999999999999999.999999999999999",15,"+9999999999999999999999999999",0),\
              "+19999999999999999999999999998.999999999999999");
    EXPECT_EQ(positiveAdd("+55555444446666600000.1234",4,"+55555666664444499999.8766",4),"+111111111111111100000");
    EXPECT_EQ(positiveAdd("+987654321.0123456789",10,"+111111111111111.345678",6),"+111112098765432.3580236789");
}


/******************************************
 * 两个代表整数的数值字符串相减，
 * NOTE：这两个字符串均应为格式化后的字符串，且必须保证num1>=num2,
 * 此函数在本程序中用于两个BignumberImpl相减的内部实现
 * 返回的字符串为格式化后的字符串
 * ****************************************/
static std::string positiveSub(const std::string & num1,size_t dot1,const std::string & num2,size_t dot2){
    //分别对两个数值字符串的整数和小数部分进行补零
    //整数部分补零：
    int intSizeNum1 = dot1 == 0 ? num1.size() - 1 : num1.size() - dot1 - 2;
    int intSizeNum2 = dot2 == 0 ? num2.size() - 1 : num2.size() - dot2 - 2;
    std::string integerNum1(std::max(intSizeNum1,intSizeNum2),'0');
    std::string integerNum2(std::max(intSizeNum1,intSizeNum2),'0');

    for (int i = integerNum1.size() - 1;i >= 0 && intSizeNum1 > 0;--i,--intSizeNum1)
        integerNum1[i] = num1[intSizeNum1];
    for (int i = integerNum2.size() - 1;i >= 0 && intSizeNum2 > 0;--i,--intSizeNum2)
        integerNum2[i] = num2[intSizeNum2];

    //小数部分补零
    std::string decimalNum1,decimalNum2;
    if (dot1 != 0 || dot2 != 0){
        decimalNum1 = std::string(std::max(dot1,dot2),'0');
        decimalNum2 = std::string(std::max(dot1,dot2),'0');
        for (size_t i = 0;i < decimalNum1.size() && num1.size() - dot1 + i < num1.size();++i)
            decimalNum1[i] = num1[num1.size() - dot1 + i];
        for (size_t i = 0;i < decimalNum2.size() && num2.size() - dot2 + i < num2.size();++i)
            decimalNum2[i] = num2[num2.size() - dot2 + i];
    }

    //开始进行计算
    int flag = 0;//进位标识
    //首先计算小数部分
    for (int i = decimalNum1.size() - 1;i >= 0;--i){
        int tmp = 0;
        if (decimalNum1[i] < decimalNum2[i]) {
            tmp = 10 + (decimalNum1[i] - '0') - (decimalNum2[i] - '0') - flag;
            flag = 1;
        }
        else {
            tmp = decimalNum1[i] - '0' - (decimalNum2[i] - '0') - flag;
            flag = 0;
        }
        decimalNum1[i] = tmp % 10 + '0';
    }
    //计算整数部分
    for (int i = integerNum1.size() - 1;i >= 0;--i){
        int tmp = 0;
        if (integerNum1[i] - flag < integerNum2[i]) {
            tmp = 10 + (integerNum1[i] - '0') - (integerNum2[i] - '0') - flag;
            flag = 1;
        }
        else {
            tmp = integerNum1[i] - '0' - (integerNum2[i] - '0') - flag;
            flag = 0;
        }
        integerNum1[i] = tmp % 10 + '0';
    }

    std::string result;
    if (decimalNum1.size() == 0) result = integerNum1;
    else result = integerNum1 + "." + decimalNum1;

    //格式化结果并返回
    formatString(result);
    return result;
}
TEST(Calculate,SubTwoPositive){
    EXPECT_EQ(positiveSub("+1234",0,"+1234",0),"+0");
    EXPECT_EQ(positiveSub("+999999999777777777666666666",0,"+444444444333333333222222222",0),\
              "+555555555444444444444444444");
    EXPECT_EQ(positiveSub("+123.002",3,"+123.002",3),"+0");
    EXPECT_EQ(positiveSub("+1234.2341",4,"+234.99998",5),"+999.23412");
    EXPECT_EQ(positiveSub("+999999999999999.111111111111111",15,"+111111111111111.999999999999999",15),\
              "+888888888888887.111111111111112");
    EXPECT_EQ(positiveSub("+9999999999.1111111111",10,"+999999999.999999999999999",15),\
              "+8999999999.111111111100001");
}

BignumberImpl BignumberImpl::operator + (const BignumberImpl & other) {
    std::string result;
    if (this->isPositive()) {
        //两个正数相加
        if (other.isPositive())
            result = positiveAdd(this->getValue(),this->getPrecision(),\
                                                   other.getValue(),other.getPrecision());
        else {//正数加负数
            BignumberImpl tmp = other.getNegete();
            if (*this >= tmp)
                result = positiveSub(this->getValue(),this->getPrecision(),\
                                                tmp.getValue(),tmp.getPrecision());
            else {
                result = positiveSub(tmp.getValue(),tmp.getPrecision(),\
                                   this->getValue(),this->getPrecision());
                result[0] = '-';
            }
        }
    }
    else {
        //负数加正数
        if (other.isPositive()){
            BignumberImpl tmp = this->getNegete();
            if (other >= tmp)
                result = positiveSub(other.getValue(),other.getPrecision(),tmp.getValue(),tmp.getPrecision());
            else {
                result = positiveSub(tmp.getValue(),tmp.getPrecision(),other.getValue(),other.getPrecision());
                result[0] = '-';
            }
        }
        //负数加负数
        else {
            result = positiveAdd(this->getNegete().getValue(),this->getPrecision(),\
                                 other.getNegete().getValue(),other.getPrecision());
            result[0] = '-';
        }
    }
    return BignumberImpl(result);
}

BignumberImpl BignumberImpl::operator - (const BignumberImpl & other) {
    std::string result;
    if (this->isPositive()){
        //两个正数相减
        if (other.isPositive()) {
            if (*this >= other)
                result = positiveSub(this->getValue(),this->getPrecision(),other.getValue(),other.getPrecision());
            else {
                result = positiveSub(other.getValue(),other.getPrecision(),this->getValue(),this->getPrecision());
                result[0] = '-';
            }
        }
        //正数减负数
        else result = positiveAdd(this->getValue(),this->getPrecision(),\
                                  other.getNegete().getValue(),other.getPrecision());
    }
    else {
        //负数减整数
        if (other.isPositive()) {
            result = positiveAdd(this->getNegete().getValue(),this->getPrecision(),\
                                 other.getValue(),other.getPrecision());
            result[0] = '-';
        }
        //负数减负数
        else return (other.getNegete() - this->getNegete());
    }
    return BignumberImpl(result);
}
TEST(Calculate,BignumberImplOperatorAdd){
    EXPECT_EQ(BignumberImpl("+0") + BignumberImpl("+0"),BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("+0") + BignumberImpl("+0.0"),BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("+0.0") + BignumberImpl("-0.0"),BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("+1234") + BignumberImpl("-1234"),BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("+1234.5678") + BignumberImpl("-1234.5678"),BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("+1234") + BignumberImpl("+1234"),BignumberImpl("+2468"));
    EXPECT_EQ(BignumberImpl("+9999999999999999999999999999") + BignumberImpl("+9999999999999999999999999999"),\
              BignumberImpl("+19999999999999999999999999998"));
    EXPECT_EQ(BignumberImpl("+1234.01") + BignumberImpl("+1234.01"),BignumberImpl("+2468.02"));
    EXPECT_EQ(BignumberImpl("+9999999999999999999999999999.999999999999999") + \
              BignumberImpl("+9999999999999999999999999999.999999999999999"),\
              BignumberImpl("+19999999999999999999999999999.999999999999998"));
    EXPECT_EQ(BignumberImpl("+9999999999999999999999999999.999999999999999") + \
                            BignumberImpl("+9999999999999999999999999999"),\
              BignumberImpl("+19999999999999999999999999998.999999999999999"));
    EXPECT_EQ(BignumberImpl("+55555444446666600000.1234") + \
              BignumberImpl("+55555666664444499999.8766"),BignumberImpl("+111111111111111100000"));
    EXPECT_EQ(BignumberImpl("+987654321.0123456789") + \
              BignumberImpl("+111111111111111.345678"),BignumberImpl("+111112098765432.3580236789"));
    EXPECT_EQ(BignumberImpl("+123456") + BignumberImpl("-3456"),BignumberImpl("+120000"));
    EXPECT_EQ(BignumberImpl("-1234.5678") + BignumberImpl("+234.4322"),BignumberImpl("-1000.1356"));
    EXPECT_EQ(BignumberImpl("-23456.9876") + BignumberImpl("-1234567.897654321"),BignumberImpl("-1258024.885254321"));

}
std::istream& operator >> (std::istream& in,BignumberImpl& num){
    std::string str;
    in>>str;
    num = BignumberImpl(str);
    return in;
}

std::ostream& operator << (std::ostream& out,const BignumberImpl& num){
    out<<num.getValue();
    return out;
}

bool BignumberImpl::operator == (const BignumberImpl & other) const {
    return (mark_ == other.mark_) &&\
           (getPrecision() == other.getPrecision()) && \
           (numStr_ == other.numStr_);
}
bool BignumberImpl::operator != (const BignumberImpl & other) const {
    return !(*this == other);
}
bool BignumberImpl::operator < (const BignumberImpl & other) const {
    //根据符号比较大小
    if (this->isNegetive() && other.isPositive()) return true;
    else if (this->isNegetive() && other.isNegetive()) return !(this->getNegete() < other.getNegete());

    //两个正数比较
    else if (this->isPositive() && other.isPositive()) {
        if (this->getIntegerSize() < other.getIntegerSize()) return true;
        else if (this->getIntegerSize() == other.getIntegerSize()) {
            size_t i = 0;
            for (i = 0;i < this->getIntegerSize();++i)
                if (this->numStr_[i] < other.numStr_[i]) return true;
            for (i = 0;i < std::min(this->getPrecision(),other.getPrecision());++i)
                if (this->numStr_[this->getIntegerSize() + i] < other.numStr_[other.getIntegerSize() + i]) return true;
            if (i < other.getPrecision()) return true;
        }
    }

    return false;
}

bool BignumberImpl::operator > (const BignumberImpl & other) const {
    return !(*this < other || *this == other);
}

bool BignumberImpl::operator <= (const BignumberImpl & other) const {
    return (*this < other || *this == other);
}

bool BignumberImpl::operator >= (const BignumberImpl & other) const {
    return (*this > other || *this == other);
}
/*TEST(Compare,EqualTest){
    EXPECT_EQ(BignumberImpl("+1234567890987654321"),BignumberImpl("+1234567890987654321"));
    EXPECT_EQ(BignumberImpl("+1234567890.0987654321"),BignumberImpl("+1234567890.0987654321"));
    EXPECT_NE(BignumberImpl("+1234567890"),BignumberImpl("0"));
    EXPECT_NE(BignumberImpl("-1234567.89"),BignumberImpl("+1234567.89"));
}*/
TEST(Compare,Less){
    EXPECT_LT(BignumberImpl("+12345678"),BignumberImpl("+123456789"));
    EXPECT_LT(BignumberImpl("-123456789"),BignumberImpl("0"));
    EXPECT_LT(BignumberImpl("-123456789"),BignumberImpl("-1"));
    EXPECT_LT(BignumberImpl("+1234.567"),BignumberImpl("+1234.5678"));
    EXPECT_LT(BignumberImpl("-1234.567"),BignumberImpl("-1234.56"));
}

BignumberImpl BignumberImpl::getNegete() const {
    BignumberImpl negete(*this);
    if (negete.mark_ == Mark::NEGETIVE) negete.mark_ = Mark::POSITIVE;
    else negete.mark_ = Mark::NEGETIVE;
    return negete;
}
BignumberImpl BignumberImpl::getAbs() const {
    BignumberImpl abs(*this);
    abs.mark_ = Mark::POSITIVE;
    return abs;
}
/*************************************************
 * BignumberImpl接受一个字符串构造函数的参数，确保字符串
 * 合法且经过了格式化
 * ***********************************************/
BignumberImpl::BignumberImpl(const std::string & value):dotPos_(0),mark_(Mark::POSITIVE){
    if (value == "") {
        numStr_  =   "0";
    }
    else {
        if (value[0] == '-') mark_ = Mark::NEGETIVE;
        for (size_t i = 1;i < value.size();++i){
            if (isdigit(value[i])) numStr_.push_back(value[i]);
            if (value[i] == '.') dotPos_ = value.size() - i - 1;
        }
    }
}


