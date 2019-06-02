.org 0x0
 	.global _start
_start:
    addi t0, zero, 0	#t0=0
    addi t1, zero, 0	#t1=0
    addi t2, zero, 101	#t2=101
for:
    add  t1, t1, t0		#t1=t1+t0
    addi t0, t0, 1		#t0++
    bne  t0, t2, for	#t0=101时结束循环
    addi t1, t1, 1		#t1++
