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

