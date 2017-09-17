#include<iostream>
#include<stdio.h>
using namespace std;
unsigned long long
choose(unsigned long long n, unsigned long long k) {
    if (k > n) {
        return 0;
    }
    unsigned long long r = 1;
    for (unsigned long long d = 1; d <= k; ++d) {
        r *= n--;
        r /= d;
    }
    return r;
}
int main(void){
    cout << choose(0 , 0) << endl;
    return 0;
}

