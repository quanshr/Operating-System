#include <lib.h>
struct sigset_t set2;
int cnt=0;
void handler(int num){
cnt++;
if(cnt>10){
return ;
}
debugf("cnt:%d HANDLER:%x %d\n",cnt,syscall_getenvid(),num);
}
int main(int argc, char **argv) {
struct sigset_t set;
set.sig[0]=set.sig[1]=0;
struct sigaction act;
act.sa_mask=set;
act.sa_handler=handler;
sigaction(10,&act,NULL);
int father=syscall_getenvid();
int ret = fork();
debugf("father:%d child:%d\n",syscall_getenvid(),ret);
if (ret != 0) {
for(int i=0;i<10;i++){
kill(ret,10);
}
while(cnt!=10);
debugf("Father passed!\n");
} else {
for(int i=0;i<10;i++){
kill(father,10);
}
while(cnt!=10);
debugf("Child passed!\n");
}
return 0;
}
