#include <lib.h>

#define TMPVA ((char *)0x50005000)
const char *msg = "hello world!\n";
const char *msg2 = "goodbye ,world!\n";

void childofspawn(void);

int main(int argc, char **argv) {

	return 0;
}

void childofspawn(void) {
	strcpy(TMPVA, msg2);
	exit();
}
