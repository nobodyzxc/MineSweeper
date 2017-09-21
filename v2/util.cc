#include<stdio.h>
#include<stdlib.h>

class sciNotat{
    public:
        float mantissa;;
        int exponent;
        sciNotat(){
            mantissa = 0;
            exponent = 0;
        }
        sciNotat(float m , int e){
            mantissa = m;
            exponent = e;
        }
        void normalize(){
            while(mantissa >= 10)
                mantissa /= 10 , exponent += 1;
            while(mantissa < 1)
                mantissa *= 10 , exponent -= 1;
        }
        void add(sciNotat be){
            if(be.exponent > exponent)
                while(exponent != be.exponent)
                    mantissa /= 10 , exponent += 1;
            else
                while(be.exponent != exponent)
                    be.mantissa /= 10 , be.exponent += 1;
            mantissa += be.mantissa;
            this->normalize();
        }
        bool isZero(){
            const float eps = 0.00001;
            return exponent == 0
                and (mantissa >= -eps)
                and (mantissa <= eps);
        }
        bool greaterThan(sciNotat be){
            if(exponent < be.exponent) return false;
            if(exponent > be.exponent) return true;
            return mantissa >= be.mantissa;
        }
};

char sign(int cur , int it){
    if(cur == it) return '>';
    return cur > it ? '=' : '.';
}

void progress(float status){
    static int init = false;
    if(status < 0){
        init = false;
        return;
    }
    if(not init) init = true;
    else   printf("\033[1A");

    for(int i = 0 ; i < 100 ; i++)
        printf("%c" , sign((int)status , i));
    printf("|");
    puts("");
}

struct sciNotat
choose(unsigned long long n, unsigned long long k){
    if (k > n) puts("choose err") , exit(0);
    struct sciNotat cal(1 , 0);
    for (unsigned long long d = 1; d <= k; ++d) {
        cal.mantissa *= (float)n--;
        cal.mantissa /= (float)d;
        cal.normalize();
    }
    return cal;
}

#ifdef UTIL_MAIN
int main(void){
    sciNotat e = choose(426 , 79);
    e.add(choose(500 , 79));
    printf("%f %d\n" , e.mantissa , e.exponent);
    return 0;
}
#endif
