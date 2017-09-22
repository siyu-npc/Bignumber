#include "bignumberimpl.h"
#include <regex>
#include <string>
#include <cmath>
#include <iostream>

extern void formatString(std::string & numStr);

/******************************************
 * 两个positive BignumberImpl 相加，
 * NOTE：必须保证传入的数为整数或0
 * 返回值：相加得到的BignumberImpl
 * ****************************************/
static BignumberImpl positiveAdd(const BignumberImpl & n1,const BignumberImpl & n2){
    //对两个数的小数部分补零以使小数部分长度相同
    std::string left = n1.getValue(),right = n2.getValue();
    if (n1.getDecimalSize() > n2.getDecimalSize()) {
        if (n2.getDecimalSize() == 0)//如果两个数中其中一个是整数另一个是小数，则在补零前需要在整数后加一个小数点
            right.push_back('.');
        right += std::string(n1.getDecimalSize() - n2.getDecimalSize(),'0');
    }
    else {
        if (n1.getDecimalSize() == 0)
            left.push_back('.');
        left += std::string(n2.getDecimalSize() - n1.getDecimalSize(),'0');
    }

    if (n1.getIntegerSize() > n2.getIntegerSize()) {
        right.insert(1,n1.getIntegerSize() - n2.getIntegerSize(),'0');
    }
    else left.insert(1,n2.getIntegerSize() - n1.getIntegerSize(),'0');

    std::string result(left.size() + 1,'0');
    int flag = 0;//进位标识
    size_t i = left.size();
    while(i-- > 1) {
        if (left[i] != '.') {
            int tmp = (left[i] - '0') + (right[i] - '0') + flag;
            result[i + 1] = tmp % 10 + '0';
            flag = ((tmp >= 10) ? 1 : 0);
        }
        else result[i + 1] = '.';
    }
    if (flag == 1) result[1] = '1';

    formatString(result);
    BignumberImpl sum(result);
    return sum;
}

static std::string positiveAdd(const std::string & n1,const std::string & n2){
    return positiveAdd(BignumberImpl(n1),BignumberImpl(n2)).getValue();
}
/******************************************
 * 两个positive BignumberImpl相减，
 * NOTE：必须保证num1>=num2,
 * 返回值：相减得到的BignumberImpl
 * ****************************************/
static BignumberImpl positiveSub(const BignumberImpl & n1,const BignumberImpl & n2) {
    //补零处理，同positiveAdd
    std::string left = n1.getValue(),right = n2.getValue();
    if (n1.getDecimalSize() > n2.getDecimalSize()) {
        if (n2.getDecimalSize() == 0)
            right.push_back('.');
        right += std::string(n1.getDecimalSize() - n2.getDecimalSize(),'0');
    }
    else {
        if (n1.getDecimalSize() == 0)
            left.push_back('.');
        left += std::string(n2.getDecimalSize() - n1.getDecimalSize(),'0');
    }

    if (n1.getIntegerSize() > n2.getIntegerSize()) {
        right.insert(1,n1.getIntegerSize() - n2.getIntegerSize(),'0');
    }
    else left.insert(1,n2.getIntegerSize() - n1.getIntegerSize(),'0');

    std::string result(left.size() + 1,'0');
    int flag = 0;//借位标识
    size_t i = left.size();
    while(i-- > 1) {
        if (left[i] != '.') {
            if (left[i] - flag < right[i]) {
                result[i + 1] = (10 + left[i] - right[i] - flag) % 10 + '0';
                flag = 1;
            }
            else {
                result[i + 1] = (left[i] - right[i] - flag) % 10 + '0';
                flag = 0;
            }
        }
        else result[i + 1] = '.';
    }

    formatString(result);
    BignumberImpl sum(result);
    return sum;
}
static std::string positiveSub(const std::string & n1,const std::string & n2){
	return positiveSub(BignumberImpl(n1),BignumberImpl(n2)).getValue();
}
/************************************************
 * the multiple method of BignumberImpl
 * *********************************************/
BignumberImpl BignumberImpl::operator * (const BignumberImpl & other) const {
	std::string left = getValue();
	std::string right = other.getValue();
	std::string result("0");

	//将两个只包含数字的字符串相加
	//返回相加的结果
	auto addString = [](std::string & str1,std::string & str2) -> std::string {
		size_t i = str1.size();
		size_t j = str2.size();
		size_t len = std::max(i,j);
		std::string result(len + 1,'0');
		int flag = 0;
		while (i && j) {
			int tmp = (str1[--i] - '0') + (str2[--j] - '0') + flag;
			result[len--] = tmp % 10 + '0';
			flag = tmp / 10;
		}
		result[len] += flag;
		while (i--) {
			result[len--] += (str1[i] - '0');
		}
		while (j--) {
			result[len--] += (str2[j] - '0');
		}
		std::cout<<str1 << "+" <<str2 << " = "<<result<<std::endl;
		return result;
	};
	size_t i = left.size();
	while (i--) {
		if (left[i] == '+' || left[i] == '-') break;
		if (left[i] == '.') continue;
		std::string tmp("");
		int flag = 0;
		size_t j = right.size();

		while (j--){
			if (right[j] == '.') continue;
			if (right[j] == '+' || right[j] == '-') break;
			int mul = (left[i] - '0') * (right[j] - '0') + flag;
			tmp.push_back(mul % 10 + '0');
			flag = mul / 10;
		}
		std::reverse(tmp.begin(),tmp.end());
		if (getDecimalSize() != 0 && (left.size() - i > getDecimalSize()))
			tmp += std::string(left.size() - i - 2,'0');
		else tmp += std::string(left.size() - i - 1,'0');
		std::cout<<"tmp = "<<tmp<<std::endl;
		result = addString(result,tmp);
		std::cout<<"result = "<<result<<std::endl;
	}
	
	//如果运算结果是小数，则插入小数点
	if (getDecimalSize() != 0 || other.getDecimalSize() != 0) 
		result.insert(result.size() - (getDecimalSize() + other.getDecimalSize()),1,'.');
	if (left[0] != right[0]) result = "-" + result;
	else result = "+" + result;
	formatString(result);
	return BignumberImpl(result);
}
BignumberImpl& BignumberImpl::operator *= (const BignumberImpl & other) {
	*this = *this + other;
	return *this;
}

/*************************************************
 * BignumberImpl的构造函数
 * BignumberImpl仅接受一个字符串构造函数的参数，并且
 * 该字符串合法且经过了格式
 * ***********************************************/
BignumberImpl::BignumberImpl(const std::string & value):value_(value){
    if (value == "") value_ = "+0";
    if (value_.find('.') == std::string::npos) dotPos_ = 0;
    else dotPos_ = value_.size() - value_.find('.') - 1;
}

BignumberImpl BignumberImpl::operator + (const BignumberImpl & other) const {
    BignumberImpl result;
    if (this->isPositive()) {
        //两个正数相加
        if (other.isPositive())
            result = positiveAdd(*this,other);
        else {
            //正数加负数，分别考虑绝对值的相对大小
            if (*this >= other.getNegete()) result = positiveSub(*this,other.getNegete());
            else result = positiveSub(other,*this).getNegete();
        }
    }
	else {
        //负数加正数
        if (other.isPositive()) {
            result = other + *this;
            //if (getNegete() >= other) result = positiveSub(getNegete(),other).getNegete();
            //else result = positiveSub(other,getNegete());
        }
        //负数加负数
        else
            result = positiveAdd(getNegete(),other.getNegete()).getNegete();
    }
    return result;
}
BignumberImpl& BignumberImpl::operator += (const BignumberImpl & other) {
	*this = *this + other;
	return *this;
}
BignumberImpl BignumberImpl::operator - (const BignumberImpl & other) const {
    BignumberImpl result;
    if (this->isPositive()){
        //两个正数相减
        if (other.isPositive()) {
            //分别考虑绝对值的相对大小
            if (*this >= other) result = positiveSub(*this,other);
            else result = positiveSub(other,*this).getNegete();
        }
        //正数减负数
        else result = positiveAdd(*this,other.getNegete());
    }
    else {
        //负数减正数
        if (other.isPositive()){
            result = positiveAdd(getNegete(),other).getNegete();
        }
        //负数减负数
        else {
            if (getNegete() >= other.getNegete()) result = positiveSub(getNegete(),other.getNegete()).getNegete();
            else result = positiveSub(other.getNegete(),getNegete());
        }
    }
    return result;
}
BignumberImpl& BignumberImpl::operator -= (const BignumberImpl & other) {
	*this = *this + other;
	return *this;
}

std::ostream& operator << (std::ostream& out,const BignumberImpl& num){
    out<<num.getValue();
    return out;
}

/********************************
 * compare functions
 *******************************/
bool BignumberImpl::operator == (const BignumberImpl & other) const {
    return value_ == other.value_;
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
                if (this->value_[i] < other.value_[i]) return true;
            for (i = 0;i < std::min(this->getPrecision(),other.getPrecision());++i)
                if (this->value_[this->getIntegerSize() + i] < other.value_[other.getIntegerSize() + i]) return true;
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

/*************************************
 * getter functions of BignumberImpl
 * **********************************/
BignumberImpl BignumberImpl::getNegete() const {
	if (value_ == "+0") return *this;
    BignumberImpl negete(*this);
    if (negete.isNegetive()) negete.value_[0] = '+';
    else negete.value_[0] = '-';
    return negete;
}
BignumberImpl BignumberImpl::getAbs() const {
    BignumberImpl abs(*this);
    abs.value_[0] = '+';
    return abs;
}
std::string BignumberImpl::getValue() const {
	return value_;
}


/*************************************************
 * 测试代码
 ************************************************/
#include "gtest/gtest.h"

TEST(Calculate,AddTwoPositive){
    positiveAdd(BignumberImpl("+9999999999999999999999999999.999999999999999"),BignumberImpl("+9999999999999999999999999999"));
    EXPECT_EQ(positiveAdd(BignumberImpl("+124"),BignumberImpl("+26")),BignumberImpl("+150"));
    EXPECT_EQ(positiveAdd(BignumberImpl("+0.001234"),BignumberImpl("+0.111234")),BignumberImpl("+0.112468"));
    EXPECT_EQ(positiveAdd(BignumberImpl("+9999999999999999999999999999"),BignumberImpl("+9999999999999999999999999999")),\
              BignumberImpl("+19999999999999999999999999998"));
    EXPECT_EQ(positiveAdd(BignumberImpl("+1234.01"),BignumberImpl("+1234.01")),BignumberImpl("+2468.02"));
    EXPECT_EQ(positiveAdd(BignumberImpl("+9999999999999999999999999999.999999999999999"),BignumberImpl("+9999999999999999999999999999.999999999999999")),\
              BignumberImpl("+19999999999999999999999999999.999999999999998"));
    EXPECT_EQ(positiveAdd(BignumberImpl("+987654321.0123456789"),BignumberImpl("+111111111111111.345678")),BignumberImpl("+111112098765432.3580236789"));
    EXPECT_EQ(positiveAdd(BignumberImpl("+55555444446666600000.1234"),BignumberImpl("+55555666664444499999.8766")),BignumberImpl("+111111111111111100000"));

    EXPECT_EQ(positiveAdd(BignumberImpl("+9999999999999999999999999999.999999999999999"),BignumberImpl("+9999999999999999999999999999")),\
              BignumberImpl("+19999999999999999999999999998.999999999999999"));
}
TEST(Calculate,SubTwoPositive){
    EXPECT_EQ(positiveSub(BignumberImpl("+1234"),BignumberImpl("+1234")),BignumberImpl("+0"));
    EXPECT_EQ(positiveSub(BignumberImpl("+999999999777777777666666666"),BignumberImpl("+444444444333333333222222222")),\
              BignumberImpl("+555555555444444444444444444"));
    EXPECT_EQ(positiveSub(BignumberImpl("+123.002"),BignumberImpl("+123.002")),BignumberImpl("+0"));
    EXPECT_EQ(positiveSub(BignumberImpl("+1234.2341"),BignumberImpl("+234.99998")),BignumberImpl("+999.23412"));
    EXPECT_EQ(positiveSub(BignumberImpl("+999999999999999.111111111111111"),BignumberImpl("+111111111111111.999999999999999")),\
              BignumberImpl("+888888888888887.111111111111112"));
    EXPECT_EQ(positiveSub(BignumberImpl("+9999999999.1111111111"),BignumberImpl("+999999999.999999999999999")),\
              BignumberImpl("+8999999999.111111111100001"));
}

TEST(BignumberImplMemFun,getValue){
    const char * inputCases[] = {"","+0","+123","-123","+123","+1123.322","+1123.322","-1123.322","+1234.5678"};
    const char * checkCases[] = {"+0","+0","+123","-123","+123","+1123.322","+1123.322","-1123.322","+1234.5678"};
    for (size_t i = 0;i < sizeof(inputCases) / sizeof(char*);++i){
        BignumberImpl tmp(inputCases[i]);
        EXPECT_EQ(tmp.getValue(),checkCases[i]);
    }
}
TEST(BignumberImplMemFun,getNegete){
	const char * input[] = {"","+0","+123","-123","+1123.322","-0.000034567"};
	const char * output[] = {"+0","+0","-123","+123","-1123.322","+0.000034567"};
	for (size_t i = 0;i < sizeof(input) / sizeof(char *);++i){
		EXPECT_EQ(BignumberImpl(input[i]).getNegete(),BignumberImpl(output[i]));
	}
}

TEST(Calculate,BignumberImplOperatorAdd){
    EXPECT_EQ(BignumberImpl("+0") + BignumberImpl("+0"),BignumberImpl("+0"));
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
    EXPECT_EQ(BignumberImpl("+9999999999999999999999999999")+BignumberImpl("+9999999999999999999999999999.999999999999999"),\
              BignumberImpl("+19999999999999999999999999998.999999999999999"));
    EXPECT_EQ(BignumberImpl("+55555444446666600000.1234") + \
              BignumberImpl("+55555666664444499999.8766"),BignumberImpl("+111111111111111100000"));
    EXPECT_EQ(BignumberImpl("+987654321.0123456789") + \
              BignumberImpl("+111111111111111.345678"),BignumberImpl("+111112098765432.3580236789"));
    EXPECT_EQ(BignumberImpl("+123456") + BignumberImpl("-3456"),BignumberImpl("+120000"));
    EXPECT_EQ(BignumberImpl("-1234.5678") + BignumberImpl("+234.4322"),BignumberImpl("-1000.1356"));
    EXPECT_EQ(BignumberImpl("-23456.9876") + BignumberImpl("-1234567.897654321"),BignumberImpl("-1258024.885254321"));
}

TEST(Calculate,BignumberImplOperatorSub){
    EXPECT_EQ(BignumberImpl("+0") - BignumberImpl("+0"),BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("+1234") - BignumberImpl("-1234"),BignumberImpl("+2468"));
    EXPECT_EQ(BignumberImpl("+1234.5678") - BignumberImpl("-1234.5678"),BignumberImpl("+1234.5678") + BignumberImpl("+1234.5678"));
    EXPECT_EQ(BignumberImpl("+1234") - BignumberImpl("+1234.01"),BignumberImpl("-0.01"));
    EXPECT_EQ(BignumberImpl("+9999999999999999999999999999") - BignumberImpl("+9999999999999999999999999999"),\
              BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("-1234.01") - BignumberImpl("-1234.01"),BignumberImpl("+0"));
    EXPECT_EQ(BignumberImpl("+9999999999999999999999999999.999999999999999") - \
              BignumberImpl("+9999999999999999999999999999.999999999999998"),\
              BignumberImpl("+0.000000000000001"));
    EXPECT_EQ(BignumberImpl("-9999999999999999999999999999.999999999999999") - \
                            BignumberImpl("+9999999999999999999999999999"),\
              BignumberImpl("-19999999999999999999999999998.999999999999999"));
}
TEST(Calculate,BignumberImplOperatorMul){
	EXPECT_EQ(BignumberImpl("+1")*BignumberImpl("+1"),BignumberImpl("+1"));
	EXPECT_EQ(BignumberImpl("+10")*BignumberImpl("+1"),BignumberImpl("+10"));

	EXPECT_EQ(BignumberImpl("+11")*BignumberImpl("+999"),BignumberImpl("+10989"));
	EXPECT_EQ(BignumberImpl("+10")*BignumberImpl("-1"),BignumberImpl("-10"));
	EXPECT_EQ(BignumberImpl("+100.11111")*BignumberImpl("+1"),BignumberImpl("+100.11111"));
	EXPECT_EQ(BignumberImpl("-100.11111")*BignumberImpl("+1"),BignumberImpl("-100.11111"));
	EXPECT_EQ(BignumberImpl("+1.23")*BignumberImpl("+109.12345"),BignumberImpl("+134.2218435"));
	EXPECT_EQ(BignumberImpl("+999999999999999.9999999999")*BignumberImpl("+10000000000000000000"),\
			BignumberImpl("+9999999999999999999999999000000000"));
	EXPECT_EQ(BignumberImpl("+999999999999999.9999999999")*BignumberImpl("+10000000000000000000.1"),\
			BignumberImpl("+10000000000000000000099998999999999.99999999999"));
}
TEST(Compare,Less){
    EXPECT_LT(BignumberImpl("+12345678"),BignumberImpl("+123456789"));
    EXPECT_LT(BignumberImpl("-123456789"),BignumberImpl("+0"));
    EXPECT_LT(BignumberImpl("-123456789"),BignumberImpl("-1"));
    EXPECT_LT(BignumberImpl("+1234.567"),BignumberImpl("+1234.5678"));
    EXPECT_LT(BignumberImpl("-1234.567"),BignumberImpl("-1234.56"));
}

TEST(Compare,Equal){
    EXPECT_EQ(BignumberImpl("+1234567890987654321"),BignumberImpl("+1234567890987654321"));
    EXPECT_EQ(BignumberImpl("+1234567890.0987654321"),BignumberImpl("+1234567890.0987654321"));
    EXPECT_NE(BignumberImpl("+1234567890"),BignumberImpl("0"));
    EXPECT_NE(BignumberImpl("-1234567.89"),BignumberImpl("+1234567.89"));
}
TEST(Compare,Greater){
    EXPECT_GT(BignumberImpl("+1"),BignumberImpl("+0"));
    EXPECT_GT(BignumberImpl("+0"),BignumberImpl("-0.00000001"));
    EXPECT_GT(BignumberImpl("+1234"),BignumberImpl("+123"));
    EXPECT_GT(BignumberImpl("+1234"),BignumberImpl("-1234"));
}

TEST(BignumberImplMemFun,getDecimalSize){
    std::string input[] = {"+0","+1234545345345","-0.12345","+12334234.12312312","+9999999999999999999999999999.999999999999999"};
    size_t output[] = {0,0,5,8,15};
    for (size_t i = 0;i < 4;++i)
        EXPECT_EQ(BignumberImpl(input[i]).getDecimalSize(),output[i]);
}

TEST(BignumberImplMemFun,getIntegerSize){
    std::string input[] = {"+0","+1234545345345","-0.12345","+12334234.12312312"};
    size_t output[] = {1,13,1,8};
    for (size_t i = 0;i < 4;++i)
        EXPECT_EQ(BignumberImpl(input[i]).getIntegerSize(),output[i]);
}
TEST(BignumberImplMemFun,getAbs){
    const char * input[] = {"+0","+1234545345345","-0.12345","+12334234.12312312"};
	const char * output[] = {"+0","+1234545345345","-999999999999999","+0.12345","-0.12345"};
	for (size_t i = 0;i < sizeof(input) / sizeof(std::string);++i){
		EXPECT_EQ(BignumberImpl(input[i]).getAbs(),BignumberImpl(output[i]));
	}
}
