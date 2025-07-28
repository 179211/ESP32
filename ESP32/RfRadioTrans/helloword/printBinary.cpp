#include <iostream>
#include <bitset>
#include <string>

using namespace std;

int main()
{
    const int bitLength = 4;
    const int maxNumber = (1 << bitLength); // 2^12

    cout << maxNumber << endl;
    return 0;
}

