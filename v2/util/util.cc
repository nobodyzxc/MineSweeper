#include<stdio.h>

char sign(int cur , int it){
    if(cur == it) return '>';
    return cur > it ? '=' : '.';
}

void progress(float status){
    static int init = false;
    if(not init) init = true;
    else   printf("\033[1A");

    for(int i = 0 ; i < 100 ; i++)
        printf("%c" , sign((int)status , i));

    printf("|");
    puts("");
}

/*
https://stackoverflow.com/questions/1838368/
calculating-the-amount-of-combinations
*/
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
