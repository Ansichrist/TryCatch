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
