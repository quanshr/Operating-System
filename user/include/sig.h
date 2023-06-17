#ifndef _SIG_H_
#define _SIG_H_

#define NSIG 64

#define SIGKILL 9
#define SIGSEGV 11
#define SIGTERM 15

typedef struct sigset_t sigset_t;

struct sigset_t{
    int sig[2]; //最多 32*2=64 种信号
};

struct sigaction{
    void (*sa_handler)(int);
    struct sigset_t sa_mask;
    void (*sa_ret)();
};

#endif
