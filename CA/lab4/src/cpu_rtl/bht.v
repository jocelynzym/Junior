module btb #(
    parameter ENTRY_LEN = 6    // BTB条目长度
)(
    input               clk,
    input               rst,
    input       [31:0]  PCF,
    input       [31:0]  PCE,
    input       [31:0]  BrNPC,
    input               BranchE,
    input       [6:0]   OpE,
    output  reg [31:0]  PredictedPC,    // 预测结果
    output  reg         PredictedF      // 预测结果有效
);
// 分支指令操作码
localparam BR_OP    = 7'b110_0011;
//BTB条目数量
localparam ENTRY_NUM = 1 << ENTRY_LEN;

// Buffer
reg [31:0]  BranchInstrAddress[ ENTRY_NUM - 1 : 0 ];
reg [31:0]  BranchTargeAddress[ ENTRY_NUM - 1 : 0 ];
reg [1:0]   State[ ENTRY_NUM - 1 : 0 ];



//根据PC确定索引
wire [ENTRY_NUM-1:0] PCF_Tag=PCF[ENTRY_NUM+1:2];
wire [ENTRY_NUM-1:0] PCE_Tag=PCE[ENTRY_NUM+1:2];


//组合逻辑产生预测跳转地址
always @(*)begin
	if( (PCF == BranchInstrAddress[PCF_Tag]) && State[PCF_Tag][1]==1'b1 ) begin
		PredictedF  <= 1'b1;
        PredictedPC <= BranchTargeAddress[PCF_Tag];
    end
	else begin
		PredictedF  <= 1'b0;
        PredictedPC <= 32'b0;
    end
end

// Buffer更新
integer i;
always @(posedge clk or posedge rst) begin
    if( rst ) begin
       for(i=0;i<ENTRY_NUM;i=i+1)   begin
            State[i]                <= 2'b0;
            BranchInstrAddress[i]   <= 32'd0;
            BranchTargeAddress[i]   <= 32'd0;
        end
    end 
	else if(OpE == BR_OP) begin //如果是分支指令,更新结果
		BranchInstrAddress[PCE_Tag]   <= PCE;
        BranchTargeAddress[PCE_Tag]   <= BrNPC;
		//实际taken
		if(BranchE)begin
			case(State_Buff[PCE_Tag])
					2'b00:State[PCE_Tag]<=2'b01;
					2'b01:State[PCE_Tag]<=2'b11;
					2'b10:State[PCE_Tag]<=2'b11;
					2'b11:State[PCE_Tag]<=2'b11;
					default:State[PCE_Tag]<=2'b11;
			endcase
		end
		//实际not taken
		if(!BranchE)begin
			case(State_Buff[PCE_Tag])
					2'b00:State[PCE_Tag]<=2'b00;
					2'b01:State[PCE_Tag]<=2'b00;
					2'b10:State[PCE_Tag]<=2'b00;
					2'b11:State[PCE_Tag]<=2'b10;
					default:State[PCE_Tag]<=2'b00;
			endcase
		end
	end
end

endmodule