.org 0x0
 	.global _start
_start:
    addi t0, zero, 0	#t0=0
    addi t1, zero, 0	#t1=0
    addi t2, zero, 0	
    addi t3, zero, 10	#t3=10
for_out: 
    addi t2, t2, 1 		#t2=1
for_in:
    add  t1, t1, t0		#t1=t0+t1
    addi t0, t0, 1		#t0++
    bne  t0, t3, for_in	#t0=11
    addi t0, zero, 0	#t0=0
    bne  t2, t3, for_out 
    addi t1, t1, 1
