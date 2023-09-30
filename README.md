## Description
Thread safe _**try-сatch**_ statement in C  
Nested _TRY_ supported  
Optional _FINALLY_  
_CATCH_ can also be used for signal handling  
GCC >=C89 supported  

### Warnings:
You can't '_return_' from _TRY_ block  
'_break_' from _CATCH_ has no effect  
Сode inside _THROW_ and _CATCH_ must be identical  
THROW(const char *c) != CATCH(const char *c)   
THROW(const char *c) == CATCH(const char *c)  

## Example
```c
#include "tc.h"

#define LOG(...) printf(__VA_ARGS__)

void f1(){
    void **a=NULL;
    int b=1;

    if((rand()%4)==1){
        THROW(int,1);
        a=*a;
    }else if((rand()%4)==2){
        THROW(int,2);
        int c=b*b/(b-1);
        printf("%d\n",c);
    }else if((rand()%4)==3)
        THROW(char,'!');
}

void sigs(){
    signal(SIGFPE, handleSIGFPE);
    signal(SIGINT, handleSIGINT);
}

int main(int argc, char *argv[])
{
    _tryCatchInit();
    // optional
    _tryCatchSetSigHandler(sigs);
    TRY(
        f1();
    )CATCH(char)(
        LOG("char\n");
    )CATCH(float,f)(
        LOG("float %f\n",f);
    )CATCH(int,sig)(
        LOG("int %d\n",sig);
    )CATCH()(
        printf("anything else\n");
    )
    printf("after TRY CATCH\n");
    _tryCatchClose();
    return 0;
}
```
