`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB(Embeded System Lab)
// Engineer: Haojun Xia
// Create Date: 2019/03/14 12:03:15
// Design Name: RISCV-Pipline CPU
// Module Name: BranchDecisionMaking
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: Decide whether to branch 
//////////////////////////////////////////////////////////////////////////////////
`include "Parameters.v"   
module BranchDecisionMaking(
    input wire [2:0] BranchTypeE,
    input wire [31:0] Operand1,Operand2,
    output reg BranchE);

    always@(*)
    begin
        case(BranchTypeE)
            `NOBRANCH: BranchE <= 0;
            `BEQ: BranchE <= (Operand1 == Operand2);
            `BNE: BranchE <= (Operand1 != Operand2);
            `BLT: BranchE <= ($signed(Operand1) < $signed(Operand2));
            `BLTU: BranchE <= (Operand1 < Operand2);
            `BGE: BranchE <= ($signed(Operand1) >= $signed(Operand2));
            `BGEU:BranchE <= (Operand1 >= Operand2);
            default: BranchE <= 0;
        endcase
    end
endmodule


/*
//BranchType[2:0]
    `define NOBRANCH  3'd0
    `define BEQ  3'd1
    `define BNE  3'd2
    `define BLT  3'd3
    `define BLTU  3'd4
    `define BGE  3'd5
    `define BGEU  3'd6

*/

//???????
    //BranchDecisionMaking???????,??BranchTypeE???,???????,?????taken?,?BranchE=1'b1
    //BranchTypeE??????Parameters.v?
//????:
    //case()
    //    `BEQ: ???
    //      .......
    //    default:                            BranchE<=1'b0;  //NOBRANCH
    //endcase
//????  
    //??BranchDecisionMaking??