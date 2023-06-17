// User-level IPC library routines

#include <env.h>
#include <lib.h>
#include <mmu.h>

#include <sig.h>

// Send val to whom.  This function keeps trying until
// it succeeds.  It should panic() on any error other than
// -E_IPC_NOT_RECV.
//
// Hint: use syscall_yield() to be CPU-friendly.
void ipc_send(u_int whom, u_int val, const void *srcva, u_int perm) {
	int r;
	while ((r = syscall_ipc_try_send(whom, val, srcva, perm)) == -E_IPC_NOT_RECV) {
		syscall_yield();
	}
	user_assert(r == 0);
}

// Receive a value.  Return the value and store the caller's envid
// in *whom.
//
// Hint: use env to discover the value and who sent it.
u_int ipc_recv(u_int *whom, void *dstva, u_int *perm) {
	int r = syscall_ipc_recv(dstva);
	if (r != 0) {
		user_panic("syscall_ipc_recv err: %d", r);
	}

	if (whom) {
		*whom = env->env_ipc_from;
	}

	if (perm) {
		*perm = env->env_ipc_perm;
	}

	return env->env_ipc_value;
}

int sigaction(int signum, struct sigaction *act, struct sigaction *oldact){
	if (signum > 64 || signum < 1) {
		return -1;
	}
	act->sa_ret = syscall_quit;
	syscall_sigaction(signum, act, oldact);
	return 0;
}
int sigprocmask(int how, const struct sigset_t *set, struct sigset_t *oldset){
	return syscall_sigprocmask(how, set, oldset);
}
int kill(u_int envid, int sig){
	if (sig > 64|| sig < 1)	{
		return -1;
	}
	return syscall_kill(envid,sig);
}
void quit(){
	syscall_quit();
}
void sigemptyset(struct sigset_t *set) { // 清空信号集，将所有位都设置为 0
	set->sig[0] = set->sig[1] = 0;
}
void sigfillset(struct sigset_t *set){ // 设置信号集，即将所有位都设置为 1
	set->sig[0] = set->sig[1] = -1;
}
//signum: 1~64
void sigaddset(struct sigset_t *set, int signum){ // 向信号集中添加一个信号，即将指定信号的位设置为
	--signum;
	(set->sig)[signum>>5] |= 1<<(signum&0x1f);
}
void sigdelset(struct sigset_t *set, int signum){ // 从信号集中删除一个信号，即将指定信号的位设置为
	--signum;
	set->sig[signum>>5] &= ~(1<<(signum&0x1f));
}
int sigismember(const struct sigset_t *set, int signum){ // 检查一个信号是否在信号集中，如果在则返回
	--signum;
	if((set->sig)[signum>>5]&(1<<(signum&0x1f))) {
		return 1;
	}
	return 0;
}

