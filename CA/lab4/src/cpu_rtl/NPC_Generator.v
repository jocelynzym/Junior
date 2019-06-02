`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB（Embeded System Lab）
// Engineer: Haojun Xia
// Create Date: 2019/03/14 11:21:33
// Design Name: RISCV-Pipline CPU
// Module Name: NPC_Generator
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: Choose Next PC value
//////////////////////////////////////////////////////////////////////////////////
//revise
module NPC_Generator(
    input wire [31:0] PCF,JalrTarget, BranchTarget, JalTarget,
    input wire BranchE,JalD,JalrE,
    output reg [31:0] PC_In,
	//COMMIT:add
	//begin:-------------------
	input   [31:0]  PCE,
    input   [31:0]  PredictedPC,
    input           PredictedF,
    input           PredictedE  
	//end:----------------------
    );
	
    always @(*)
    begin
		//COMMIT:revise
		//begin:-------------------
        if(JalrE)
            PC_In <= JalrTarget;
        else if(BranchE & ~PredictedE)//revise:分支语句,预测失败,分支成功
            PC_In <= BranchTarget;
		else if(~BranchE & PredictedE)//add:分支语句,预测成功,分支失败
            PC_In <= PCE + 4;
        else if(JalD)
            PC_In <= JalTarget;
		else if(PredictedF)//add
            PC_In <= PredictedPC;
        else
            PC_In <= PCF+4;
		//end:-----------------------
		
    end
endmodule
