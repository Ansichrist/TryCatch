#ifndef TC_H
#define TC_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <setjmp.h>
#include <string.h>
#include "macros.h"


typedef struct{ const char *type, *where; void *data; }_t_EXCEPTION;
typedef struct{ jmp_buf *jmp; _t_EXCEPTION e[1]; char buffer[64+7], final;}_t_TRYCATCH;
_t_TRYCATCH *_tryCatch();
#define _CATCH(...) {__VA_ARGS__ break;}}
#define _CATCH0() else{ _CATCH
#define _CATCH1(_type_) else if( !strcmp(EXCEPTION->type,#_type_) ) { _CATCH
#define _CATCH2(_type_,_var_) else if( !strcmp(EXCEPTION->type,#_type_) ) { _type_ _var_= *(_type_*)(EXCEPTION->data); _CATCH
#define _THROW_INFO MACROS_FILE() ":" MACROS_STRING(MACROS_LINE())
#define _THROW0() do{\
    _t_TRYCATCH *_1_=_tryCatch();\
    if(_1_){\
        if(_1_->jmp) longjmp(*_1_->jmp,1);\
        else if(_1_->e->type) printf("\nterminate called after throwing an instance of \'%s\' at [%s]\n\n",_1_->e->type,_1_->e->where);\
        else printf("\nterminate called after throwing at [" _THROW_INFO "]\n\n");\
    }else printf("\nterminate called after throwing at [" _THROW_INFO "]\n\n");\
    exit(-1);\
}while(0)
#define _THROW1(_t_,...) do{\
    _t_TRYCATCH *_1_=_tryCatch();\
    if(_1_ && _1_->jmp){\
        _1_->e->type=#_t_; _1_->e->where=_THROW_INFO;\
        if(_1_->e->data!=_1_->buffer){free(_1_->e->data); _1_->e->data=_1_->buffer;}\
        MACROS_IF(MACROS_ZERO_OR_ONE(__VA_ARGS__))(\
            MACROS_EXTRACT( if( sizeof(_t_)<=sizeof(_1_->buffer) || (_1_->e->data=malloc(sizeof(_t_))) ) { *(_t_*)(_1_->e->data)=(_t_)__VA_ARGS__; longjmp(*_1_->jmp,1);} ),\
            longjmp(*_1_->jmp,1);\
        )\
    }printf("\nterminate called after throwing an instance of \'" #_t_ "\' at [" _THROW_INFO "]\n\n");\
    exit(-1);\
}while(0)

static struct{
    void(*sig)();
    _t_TRYCATCH *main;
    pthread_t id;
    pthread_key_t key;
    char init;
}_tc={NULL};

void _tryCatchSetSigHandler(void(*function)()){
    _tc.sig=function;
}

void _tryCatchDeleter(_t_TRYCATCH *s){
    if(s){
        if(s==_tc.main) _tc.main=NULL;
        if(s->e->data!=s->buffer) free(s->e->data);
        free(s);
    }
}

void _tryCatchInit(){
    _tc.id=pthread_self();
    _tc.init=!pthread_key_create(&_tc.key,(void*)_tryCatchDeleter);
}

void _tryCatchClose(){
    if(_tc.main) _tryCatchDeleter(_tc.main);
    if(_tc.init) pthread_key_delete(_tc.key);
}

_t_TRYCATCH *_tryCatch(){
    _t_TRYCATCH *s=NULL;
    if(_tc.init && !(s=pthread_getspecific(_tc.key)) && (s=malloc(sizeof(*s))) ){
        if(pthread_setspecific(_tc.key,s)){
            free(s); s=NULL;
        }else{
            if(_tc.sig) _tc.sig();
            if(pthread_equal(pthread_self(),_tc.id))
                _tc.main=s;
            memset(s,0,sizeof(*s));
        }
    }
    return s;
}

typedef struct{
    int sigNum;
    void *address;
}E_SIGFAULT;

#define TRY(...)     if( ({jmp_buf _1tc_; _t_TRYCATCH *_2tc_=_tryCatch(); void *_3tc_=_2tc_->jmp; char _4tc_; _2tc_->jmp=&_1tc_; if( (_4tc_=!setjmp(_1tc_)) ){__VA_ARGS__} _2tc_->jmp=_3tc_; _2tc_->final=!_4tc_; _4tc_;}) );else for(;;({THROW();})) if(0);
#define CATCH(...)   MACROS_OVERLOAD_CALL(_CATCH,__VA_ARGS__)
#define FINALLY(...) if( ({char *_1tc_=&_tryCatch()->final; char _2tc_=*_1tc_; *_1tc_=0; _2tc_;}) ){__VA_ARGS__}
#define THROW(...)   MACROS_OVERLOAD_01_CALL(_THROW,__VA_ARGS__)
#define EXCEPTION    ((const _t_EXCEPTION*)(_tryCatch()->e))

void _tryCatchSetSigHandler(void(*function)());





#endif // TC_H




