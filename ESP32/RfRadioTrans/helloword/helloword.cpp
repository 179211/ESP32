#include <iostream>
#include <bitset>
#include <string>

using namespace std;

int main()
{
    // declaring an uninitialized bitset object
    bitset<8> uninitializedBitset;

    // initialization with decimal number
    bitset<8> decimalBitset(5);

    // initialization with binary string
    bitset<8> stringBitset(string("1111"));

    cout << "Uninitialized bitset: " << uninitializedBitset
         << endl;
    cout << "Initialized with decimal: " << decimalBitset
         << endl;
    cout << "Initialized with string: " << stringBitset
         << endl;

    const int bitLength = 12;
    const int maxNumber = (1 << bitLength); // 2^12

    cout << maxNumber << endl;

    for (int i = 0; i < 10; ++i){
        bitset<bitLength> binary(i);
        cout << binary << endl;
    }

    return 0;
}
