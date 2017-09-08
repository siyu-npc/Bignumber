/*************************************************************************
    > File Name: test_bignumberimpl.h
    > Author: 关山月朦胧
    > Mail: 1036014410@qq.com
    > Created Time: Fri 08 Sep 2017 07:14:29 AM CST
 ************************************************************************/
#ifndef TEST_BIGNUMBERIMPL_H
#define TEST_BIGNUMBERIMPL_H

#ifdef GTEST
#define private public
#define protected public
#endif

#include "gtest/gtest.h"
#include "bignumberimpl.h"

TEST(getValue,getValueCase){
    const char * inputCases[] = {"","+0","+123","-123","+123","+1123.322","+1123.322","-1123.322","+1234.5678"};
    const char * checkCases[] = {"+0","+0","+123","-123","+123","+1123.322","+1123.322","-1123.322","+1234.5678"};
    for (size_t i = 0;i < sizeof(inputCases) / sizeof(char*);++i){
        BignumberImpl tmp(inputCases[i]);
        EXPECT_EQ(tmp.getValue(),checkCases[i]);
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

TEST(Compare,Less){
    EXPECT_LT(BignumberImpl("+12345678"),BignumberImpl("+123456789"));
    EXPECT_LT(BignumberImpl("-123456789"),BignumberImpl("0"));
    EXPECT_LT(BignumberImpl("-123456789"),BignumberImpl("-1"));
    EXPECT_LT(BignumberImpl("+1234.567"),BignumberImpl("+1234.5678"));
    EXPECT_LT(BignumberImpl("-1234.567"),BignumberImpl("-1234.56"));
}

TEST(Compare,EqualTest){
    EXPECT_EQ(BignumberImpl("+1234567890987654321"),BignumberImpl("+1234567890987654321"));
    EXPECT_EQ(BignumberImpl("+1234567890.0987654321"),BignumberImpl("+1234567890.0987654321"));
    EXPECT_NE(BignumberImpl("+1234567890"),BignumberImpl("0"));
    EXPECT_NE(BignumberImpl("-1234567.89"),BignumberImpl("+1234567.89"));
}

#endif //TEST_BIGNUMBERIMPL_H
