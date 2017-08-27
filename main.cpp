#include <iostream>
#include "gtest/gtest.h"
#include "bignumber.h"

using namespace std;

int main(int argc,char **argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    int _ = RUN_ALL_TESTS();

   cout<<"**********************************\n";
   cout<<"**Welcome use this program!Have a enjoy. **\n";
   cout<<"**Please input formule to calculate.\n";
   cout<<"**********************************\n";
 restart:
    try {
    while (true){
        Bignumber num1,num2;
        Bignumber result;
        char operate;
        cin>>num1>>operate>>num2;
        switch (operate){
        case ('+'):
            result = num1 + num2;break;
        case ('-'):
            result = num1 - num2;break;
        default:
            cout<<"I don't know what it is : "<<operate<<" ,Please input again\n";
            goto restart;
        }
        cout<<result<<endl;
    }
    }
    catch (const std::invalid_argument& err) {
        cout<<err.what()<<endl;
        cout<<"Please reinput a correct num!";
    }
    //catch (...){
        cout<<"Oops!Some unexpected errors happened!"<<endl;
    //}

    cout << "Hello World!" << endl;
    return 0;
}

