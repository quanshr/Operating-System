#include <lib.h>
struct sigset_t set2;
void handler(int num){
static int cnt=0;
cnt++;
debugf("cnt:%d HANDLER:%x %d\n",cnt,syscall_getenvid(),num);
if(cnt==1000){
debugf("CONGRATULATION:TEST PASSED!\n");
kill(0,SIGKILL);
}
}
int main(int argc, char **argv) {
struct sigset_t set;
set.sig[0]=set.sig[1]=0;
struct sigaction act;
act.sa_mask=set;
act.sa_handler=handler;
sigaction(10,&act,NULL);
    int ret = fork();
    debugf("father:%d child:%d\n", syscall_getenvid(), ret);
    if (ret != 0) {
        for(int i = 0; i < 1000; i++){
            kill(ret, 10);
        }
    } else {
        while(1);
    }
    return 0;
}