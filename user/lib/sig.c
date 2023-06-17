#include <lib.h>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
	if(signum < 1 || signum > 64) {
		return -1;
	}
	syscall_sigaction(signum, act, oldact);
	return 0;
}

int sigprocmask(int how, const struct sigset_t *set, struct sigset_t *oldset) {
	return syscall_sigprocmask(how, set, oldset);
}

int kill(u_int envid, int sig){
	if(sig < 1 || sig > 64) {
		return -1;
	}
	return syscall_kill(envid,sig);
}

void quit(){
	syscall_quit();
}

void sigemptyset(struct sigset_t *set) { // 清空信号集，将所有位都设置为 0 
	(set->sig)[0] = (set->sig)[1] = 0; 
} 

void sigfillset(struct sigset_t *set) { // 设置信号集，即将所有位都设置为 1 
	(set->sig)[0] = (set->sig)[1] = -1; 
} 

// signum: 1~64 

void sigaddset(struct sigset_t *set, int signum) { // 向信号集中添加一个信号，即将指定信号的位设置为
	signum--;
	(set->sig)[signum>>5] |= (1<<(signum&0x1f));
}

void sigdelset(struct sigset_t *set, int signum) { // 从信号集中删除一个信号，即将指定信号的位设置为
	signum--; 
	(set->sig)[signum>>5]&=(~(1<<(signum&0x1f))); 
} 

int sigismember(const struct sigset_t *set, int signum) { // 检查一个信号是否在信号集中，如果在则返回
	if(signum < 1 || signum > 64) {
		return -1;
	}
	signum--; 
	if((set->sig)[signum>>5]&(1<<(signum&0x1f))) return 1; 
	return 0; 
}
