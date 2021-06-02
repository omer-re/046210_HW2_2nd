#ifndef BLOCK_API_H
#define BLOCK_API_H
//#include <linux/stddef.h>
#include <errno.h>
//#include <sys/types.h>

int block_query(const char* filename)
{
    int res;
    __asm__
    (
    "pushl %%eax;"
    "pushl %%ebx;"
    "movl $245, %%eax;"
    "movl %1, %%ebx;"
    "int $0x80;"
    "movl %%eax,%0;"
    "popl %%ebx;"
    "popl %%eax;"
    : "=m" (res)
    : "m" (filename)
    );


    return (int)res;
}

int block_clear(void)
{
    int res;
    __asm__
    (
    "pushl %%eax;"
    "movl $244, %%eax;"
    "int $0x80;"
    "movl %%eax,%0;"
    "popl %%eax;"
    : "=m" (res)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return (int)res;
}

int block_add_file(const char* filename)
{
    ssize_t res;
    __asm__
    (
    "pushl %%eax;"
    "pushl %%ebx;"
    "movl $243, %%eax;"
    "movl %1, %%ebx;"
    "int $0x80;"
    "movl %%eax,%0;"
    "popl %%ebx;"
    "popl %%eax;"
    : "=m" (res)
    : "m" (filename)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return (int)res;
}

int block_add_process(pid_t pid)
{
    int res;
    __asm__
    (
    "pushl %%eax;"
    "pushl %%ebx;"
    "movl $246, %%eax;"
    "movl %1, %%ebx;"
    "int $0x80;"
    "movl %%eax,%0;"
    "popl %%ebx;"
    "popl %%eax;"
    : "=m" (res)
    : "m" (pid)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return (int)res;
}

long restricted_syscall_open(const char* filename, int flags, int mode)
{
    int res;
    __asm__
    (
    "pushl %%eax;"
    "pushl %%ebx;"
    "pushl %%ecx;"
    "pushl %%edx;"
    "movl $5, %%eax;"
    "movl %1, %%ebx;"
    "movl %2, %%ecx;"
    "movl %3, %%edx;"
    "int $0x80;"
    "movl %%eax,%0;"
    "popl %%edx;"
    "popl %%ecx;"
    "popl %%ebx;"
    "popl %%eax;"
    : "=m" (res)
    : "m" (filename), "m" (flags), "m"(mode)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return (int)res;
}


/////   OLD VERSION    /////
//
//int block_add_file(const char *filename){
//ssize_t res;
//__asm__
//(
//"pushl %%eax;"
//"pushl %%ebx;"
//"movl $243, %%eax;"
//"movl %1, %%ebx;"
//"int $0x80;"
//"movl %%eax, %0;"
//"popl %%ebx;"
//"popl %%eax;"
//:"=m" (res)
//:"m" (filename)
//);
//
//if (res >= (unsigned long)(-125))
//{
//errno = -res;
//res = -1;
//}
//return (int) res;
//}
//
//
//int block_clear(void){
//    int res;
//    __asm__
//    (
//    "pushl %%eax;"
//    "movl $244, %%eax;"
//    "int $0x80;"
//    "movl %%eax, %0;"
//    "popl %%eax;"
//    :"=m" (res)
//    );
//
//    if (res >= (unsigned long)(-125))
//    {
//        errno = -res;
//        res = -1;
//    }
//    return (int) res;
//}
//
//
////bool block_query(const char *filename){
//int block_query(const char *filename){
//    int res;
//    __asm__
//    (
//    "pushl %%eax;"
//    "pushl %%ebx;"
//    "movl $245, %%eax;"
//    "movl %1, %%ebx;"
//    "int $0x80;"
//    "movl %%eax, %0;"
//    "popl %%ebx;"
//    "popl %%eax;"
//    :"=m" (res)
//    :"m" (filename)
//    );
//
//    if (res >= (unsigned long)(-125))
//    {
//        errno = -res;
//        res = -1;
//    }
//    //return (bool) res;
//    return res;
//}
//
//int block_add_process(pid_t pid){
//
//    int res;
//    __asm__
//    (
//    "pushl %%eax;"
//    "pushl %%ebx;"
//    "movl $246, %%eax;"
//    "movl %1, %%ebx;"
//    "int $0x80;"
//    "movl %%eax, %0;"
//    "popl %%ebx;"
//    "popl %%eax;"
//    :"=m" (res)
//    : "m" (pid)
//    );
//
//    if (res >= (unsigned long)(-125))
//    {
//        errno = -res;
//        res = -1;
//    }
//    return (int) res;
//}

#endif