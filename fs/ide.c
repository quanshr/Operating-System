/*
 * operations on IDE disk.
 */

#include "serv.h"
#include <drivers/dev_disk.h>
#include <lib.h>
#include <mmu.h>

// Overview:
//  read data from IDE disk. First issue a read request through
//  disk register and then copy data from disk buffer
//  (512 bytes, a sector) to destination array.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  dst: destination for data read from IDE disk.
//  nsecs: the number of sectors to read.
//
// Post-Condition:
//  Panic if any error occurs. (you may want to use 'panic_on')
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET', 'DEV_DISK_OPERATION_READ',
//  'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS', 'DEV_DISK_BUFFER'

/*
extern int read_sector(int diskno, int offset);
# read sector at specified offset from the beginning of the disk image.
LEAF(read_sector)
	sw a0, 0xB3000010 # select the IDE id.
	sw a1, 0xB3000000 # offset.
	li t0, 0
	sw t0, 0xB3000020 # start read.
	lw v0, 0xB3000030
	nop
	jr ra
	nop
END(read_sector)
*/
int read_sector(int diskno, int offset) {
	int res = 0, t = 0;
	asm volatile("sw %0, 0xB3000010" :"=r"(diskno) );
	asm volatile("sw %0, 0xB3000000" :"=r"(offset) );
	asm volatile("sw %0, 0xB3000020" :"=r"(t) );
	asm volatile("lw %0, 0xB3000030" :"=r"(res) );
	return res;
}

int write_sector(int diskno, int offset) {
	int res = 0, t = 1;
	asm volatile("sw %0, 0xB3000010" :"=r"(diskno) );
	asm volatile("sw %0, 0xB3000000" :"=r"(offset) );
	asm volatile("sw %0, 0xB3000020" :"=r"(t) );
	asm volatile("lw %0, 0xB3000030" :"=r"(res) );
	return res;
}



void ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs) {
	u_int begin = secno * BY2SECT;
	u_int end = begin + nsecs * BY2SECT;

	for (u_int off = 0; begin + off < end; off += BY2SECT) {
		uint32_t temp = diskno;
		/* Exercise 5.3: Your code here. (1/2) */
		
		if (read_sector(temp, begin + off)) {
			panic_on("read_sector error!");
		}
		syscall_read_dev(dst + off, KSEG1 | DEV_DISK_BUFFER, BY2SECT);
	}
}

// Overview:
//  write data to IDE disk.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  src: the source data to write into IDE disk.
//  nsecs: the number of sectors to write.
//
// Post-Condition:
//  Panic if any error occurs.
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET', 'DEV_DISK_BUFFER',
//  'DEV_DISK_OPERATION_WRITE', 'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS'
void ide_write(u_int diskno, u_int secno, void *src, u_int nsecs) {
	u_int begin = secno * BY2SECT;
	u_int end = begin + nsecs * BY2SECT;

	for (u_int off = 0; begin + off < end; off += BY2SECT) {
		uint32_t temp = diskno;
		/* Exercise 5.3: Your code here. (2/2) */
		
		syscall_write_dev(src + off, KSEG1 | DEV_DISK_BUFFER, BY2SECT);
		if (write_sector(temp, begin + off)) {
			panic_on("write_sector error!");
		}

	}
}
