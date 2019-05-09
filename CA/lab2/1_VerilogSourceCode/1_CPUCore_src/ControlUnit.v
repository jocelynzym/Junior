`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB (Embeded System Lab)
// Engineer: Haojun Xia
// Create Date: 2019/02/08
// Design Name: RISCV-Pipline CPU
// Module Name: ControlUnit
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: RISC-V Instruction Decoder
//////////////////////////////////////////////////////////////////////////////////
`include "Parameters.v"   
module ControlUnit(
    input wire [6:0] Op,
    input wire [2:0] Fn3,
    input wire [6:0] Fn7,
    output wire JalD,
    output wire JalrD,
    output reg [2:0] RegWriteD,
    output wire MemToRegD,
    output reg [3:0] MemWriteD,
    output wire LoadNpcD,
    output reg [1:0] RegReadD,
    output reg [2:0] BranchTypeD,
    output reg [3:0] AluContrlD,
    output wire [1:0] AluSrc2D,
    output wire AluSrc1D,
    output reg [2:0] ImmType        
    );
	
	/*常量定义*/
	//Op砿
	localparam LD_type_op  = 7'b0000011;   
	localparam ST_type_op  = 7'b0100011;
	localparam R_type_op   = 7'b0110011;
	localparam I_type_op   = 7'b0010011; 
	localparam LUI_type_op = 7'b0110111; 
	localparam AUIPC_type_op = 7'b0010111; 
	localparam B_type_op   = 7'b1100011; 
	localparam JAL_type_op = 7'b1101111;
	localparam JALR_type_op= 7'b1100111;  	
	
	/*指令判定*/
	//LD籿
	wire LOAD;
    wire LB,LH,LW,LBU,LHU;
	//ST籿
	wire STORE;
	wire SB,SH,SW;
	//R籿
	wire R;
	wire ADD,SUB,XOR,OR,AND,SLL,SRL,SRA,SLT,SLTU;
	//I籍
	wire I_typeA;
	wire I_typeB;
	wire ADDI,XORI,ORI,ANDI,SLLI,SRLI,SRAI,SLTI,SLTIU;
	//LUI
	wire LUI;
	//AUIPC
	wire AUIPC;
	//Branch籿
	wire BRANCH;
	wire BEQ,BNE,BLT,BGE,BLTU,BGEU;
	//JAL
    wire JAL;
	//JALR
	 wire JALR;
	
	assign LB= (Op==LD_type_op)&&(Fn3==3'b000);
	assign LH= (Op==LD_type_op)&&(Fn3==3'b001);
	assign LW= (Op==LD_type_op)&&(Fn3==3'b010);
	assign LBU=(Op==LD_type_op)&&(Fn3==3'b100);
	assign LHU=(Op==LD_type_op)&&(Fn3==3'b101);
	assign LOAD=LB||LH||LW||LBU||LHU;
	
	assign SB= (Op==ST_type_op)&&(Fn3==3'b000);
	assign SH= (Op==ST_type_op)&&(Fn3==3'b001);
	assign SW= (Op==ST_type_op)&&(Fn3==3'b010);
	assign STORE=SB||SH||SW;
	
	
	assign ADD=(Op==R_type_op)&&(Fn3==3'b000)&&(Fn7==7'b0000000);
	assign SUB=(Op==R_type_op)&&(Fn3==3'b000)&&(Fn7==7'b0100000);
	assign XOR=(Op==R_type_op)&&(Fn3==3'b100)&&(Fn7==7'b0000000);
	assign OR= (Op==R_type_op)&&(Fn3==3'b110)&&(Fn7==7'b0000000);
	assign AND=(Op==R_type_op)&&(Fn3==3'b111)&&(Fn7==7'b0000000);
	assign SLL=(Op==R_type_op)&&(Fn3==3'b001)&&(Fn7==7'b0000000);
	assign SRL=(Op==R_type_op)&&(Fn3==3'b101)&&(Fn7==7'b0000000);
	assign SRA=(Op==R_type_op)&&(Fn3==3'b101)&&(Fn7==7'b0100000);
	assign SLT=(Op==R_type_op)&&(Fn3==3'b010)&&(Fn7==7'b0000000);
	assign SLTU=(Op==R_type_op)&&(Fn3==3'b011)&&(Fn7==7'b0000000);
	assign R=ADD||SUB||XOR||OR||AND||SLL||SRL||SRA||SLT||SLTU;

	
	assign ADDI=(Op==I_type_op)&&(Fn3==3'b000);
	assign XORI=(Op==I_type_op)&&(Fn3==3'b100);
	assign ORI= (Op==I_type_op)&&(Fn3==3'b110);
	assign ANDI=(Op==I_type_op)&&(Fn3==3'b111);
	assign SLLI=(Op==I_type_op)&&(Fn3==3'b001)&&(Fn7==7'b0000000);
	assign SRLI=(Op==I_type_op)&&(Fn3==3'b101)&&(Fn7==7'b0000000);
	assign SRAI=(Op==I_type_op)&&(Fn3==3'b101)&&(Fn7==7'b0100000);
	assign SLTI=(Op==I_type_op)&&(Fn3==3'b010);
	assign SLTIU=(Op==I_type_op)&&(Fn3==3'b011);
	assign I_typeA=SLLI||SRLI||SRAI;
	assign I_typeB=ADDI||XORI||ORI||ANDI||SLTI||SLTIU;
	
	assign LUI=(Op==LUI_type_op);
	assign AUIPC=(Op==AUIPC_type_op);
	
	assign BEQ=((Op==B_type_op)&&(Fn3==3'b000));
	assign BNE=(Op==B_type_op)&&(Fn3==3'b001);
	assign BLT=(Op==B_type_op)&&(Fn3==3'b100);
	assign BGE=(Op==B_type_op)&&(Fn3==3'b101);
	assign BLTU=(Op==B_type_op)&&(Fn3==3'b110);
	assign BGEU=(Op==B_type_op)&&(Fn3==3'b111);
	assign BRANCH=BEQ||BNE||BLT||BGE||BLTU||BGEU;
	
	
	assign JAL=(Op==JAL_type_op);
	assign JALR=(Op==JALR_type_op);

	
	
	/*输出控制信号*/
	//wire型信叿
	assign JalD=JAL;
	assign JalrD=JALR;
	assign MemToRegD=LB||LH||LW||LBU||LHU;
	assign LoadNpcD=JAL||JALR; 
	assign AluSrc1D= (AUIPC); 
	assign AluSrc2D=(I_typeA==1)?(2'b01):((BRANCH==1||R==1)?(2'b00):(2'b10));

		
	//reg型信叿
	always@(*) //RegWriteD[2:0]
	begin
		if(LB) 		RegWriteD<=`LB;
		else if(LH) RegWriteD<=`LH;
		else if(LW) RegWriteD<=`LW;
		else if(LBU)RegWriteD<=`LBU;
		else if(LHU)RegWriteD<=`LHU;
		else if(R||I_typeA||I_typeB||LUI||AUIPC||JAL||JALR)  RegWriteD<=`LW;
		else 		RegWriteD<=`NOREGWRITE;
	end
	
	always@(*)//MemWriteD[3:0]
	begin
		if(SB) 		MemWriteD<=4'b0001;
		else if(SH) MemWriteD<=4'b0011;
		else if(SW) MemWriteD<=4'b1111;
		else 		MemWriteD<=4'b0000;
	end
	
	
	
	always@(*)//RegReadD[1:0]
	begin
		if(R||STORE||BRANCH) 		RegReadD<=2'b11;
		else if(LOAD||I_typeA||I_typeB||JALR) RegReadD<=2'b10;
		else 		RegReadD<=2'b00;
	end
	
	
	always@(*)//BranchTypeD[2:0]
	begin
		if(BEQ) 		BranchTypeD<=`BEQ;
		else if(BNE) 	BranchTypeD<=`BNE;
		else if(BLT) 	BranchTypeD<=`BLT;
		else if(BLTU) 	BranchTypeD<=`BLTU;
		else if(BGE) 	BranchTypeD<=`BGE;
		else if(BGEU) 	BranchTypeD<=`BGEU;
		else  			BranchTypeD<=`NOBRANCH ;
	end
	
	always@(*)//AluContrlD[2:0]
	begin
		if(ADD||ADDI||LOAD||STORE||AUIPC||JALR) 
							AluContrlD<=`ADD;
		else if(SRL||SRLI) 	AluContrlD<=`SRL;
		else if(SRA||SRAI) 	AluContrlD<=`SRA;
		else if(SLL||SLLI) 	AluContrlD<=`SLL;
		else if(SUB) 		AluContrlD<=`SUB;
		else if(XOR||XORI) 	AluContrlD<=`XOR;
		else if(OR||ORI) 	AluContrlD<=`OR;
		else if(AND||ANDI) 	AluContrlD<=`AND;
		else if(SLT||SLTI) 	AluContrlD<=`SLT;
		else if(SLTU||SLTIU)AluContrlD<=`SLTU;
		else if(LUI) 		AluContrlD<=`LUI;
		else  			AluContrlD<=4'dx ;
	end
	
	
	always@(*)//ImmType[2:0]
	begin
		if(R) 				ImmType<=`RTYPE;
		else if(JALR||I_typeA||I_typeB||LOAD) ImmType<=`ITYPE;
		else if(STORE) 		ImmType<=`STYPE;
		else if(BRANCH) 	ImmType<=`BTYPE;
		else if(LUI||AUIPC) ImmType<=`UTYPE;
		else if(JAL) 		ImmType<=`JTYPE;
		else 				ImmType<=3'dx;
	end
	
endmodule

//功能说明
    //ControlUnit       是本CPU的指令译码器，组合鿻辑电路
//输入
    // Op               是指令的操作码部刿
    // Fn3              是指令的func3部分
    // Fn7              是指令的func7部分
//输出
    // JalD==1          表示Jal指令到达ID译码阶段
    // JalrD==1         表示Jalr指令到达ID译码阶段
    // RegWriteD        表示ID阶段的指令对应的 寄存器写入模弿 ，所有模式定义在Parameters.v丿
    // MemToRegD==1     表示ID阶段的指令需要将data memory读取的忼写入寄存器,
    // MemWriteD        兿4bit，采用独热码格式，对于data memory皿32bit字按byte进行写入,MemWriteD=0001表示只写入最使1个byte，和xilinx bram的接口类伿
    // LoadNpcD==1      表示将NextPC输出到ResultM
    // RegReadD[1]==1   表示A1对应的寄存器值被使用到了，RegReadD[0]==1表示A2对应的寄存器值被使用到了，用于forward的处琿
    // BranchTypeD      表示不同的分支类型，承有类型定义在Parameters.v丿
    // AluContrlD       表示不同的ALU计算功能，所有类型定义在Parameters.v丿
    // AluSrc2D         表示Alu输入溿2的鿉择
    // AluSrc1D         表示Alu输入溿1的鿉择
    // ImmType          表示指令的立即数格式，所有类型定义在Parameters.v丿   
//实验要求  
    //实现ControlUnit模块   