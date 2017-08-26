#include <iostream>
#include "gtest/gtest.h"
#include "bignumber.h"

using namespace std;


int main(int argc,char **argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
    cout << "Hello World!" << endl;
    return 0;
}

