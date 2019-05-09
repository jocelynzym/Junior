.org 0x0
 	.global _start
_start:
	auipc	x10,1		#AUIPC	x10=0x01000
	jal     zero,tag7	#JAL	
	addi	x2, x2, 1	
tag7:
	jalr	zero,zero,20  #JALR
	addi	x2, x2, 1	
tag8:
	addi	x2, x2, 1
	ori		x1, x0, 1	#ORI	x1=1		x1=1
	addi	x2, x1, 1	#ADDI	x2=2
	sub 	x2, x2, x1	#SUB	x2=1
	addi	x2, x2, 1	#ADDI	x2=2		x2=2
	slli	x3, x2, 1	#SLLI	x3=4
	srli	x3, x3, 1	#SRLI	x3=2
	srai	x3, x3, 1	#SRA	x3=1		x3=1
	sll		x4, x3, x2	#SLL	x4=4
	srl		x4, x4, x1	#SRL	x4=2
	sra		x4, x4, x1	#SRA	x4=1		x4=1
	slt		x5, x4, x2	#SLT	x5=1	
	sltu	x5, x4, x2	#SLTU	x5=1		x5=1
	slti	x6, x5, 2 	#SLTI	x6=1
	sltiu	x6, x5, 2 	#SLTIU	x6=1		x6=1
	xor		x7, x2, x6	#XOR	x7=3	
	xori	x7, x2, 1 	#XORI	x7=3		x7=3
	or		x8, x1, x2	#OR		x8=3		
	addi	x8, x8, 1	#               	x8=4
	and		x9, x1, x7	#AND	x9=1	
	andi	x9, x1, 3 	#ANDI	x9=1		
	addi	x9, x9, 7	#               	x9=8
	lui		x11,0xfffff	#LUI	x11=0xfffff000
	sw		x0,(x0)
	sw		x0,(x8)
	sw		x0,(x9)
	sb		x11,(x0)	#SB		DataRam[0]=0
	sh		x11,(x8)	#SH		DataRam[4]=0x0f000
	sw		x11,(x9)    #SW		DataRam[8]=0xfffff000
	lb		x12,(x0)	#LB		x12=0
	lh		x13,(x8)	#LH		x13=0xfffff000
	lw		x14,(x9)	#LW 	x14=0xfffff000
	lbu		x15,(x8)	#LBU	x15=0
	lhu		x16,(x8)	#LHU	x16=0x0f000
	
		
	beq		x5, x6, tag1
	addi	x2, x2, 1	
tag1:
	bne		x7, x8, tag2
	addi	x2, x2, 1	
tag2:	
	blt		x4, x8, tag3
	addi	x2, x2, 1	
tag3:
	bltu	x4, x8, tag4
	addi	x2, x2, 1	
tag4:
	bge		x8, x6, tag5
	addi	x2, x2, 1
tag5:	
	bgeu	x8, x6, tag5
	addi	x2, x2, 1	
tag6:		
	addi	x2, x2, 0
	
	
	

#x1=1, x1=2, x3=1, x4=1, x5=1, x6=1, x7=3, x8=4, x9=8
#x10=0x01000, x11=0xfffff000
#x12=0, x13=0xfffff000, x14=0xfffff000, x15=0, x16=0x0f000
#x17=8







