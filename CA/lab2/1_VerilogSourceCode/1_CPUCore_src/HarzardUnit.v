//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB（Embeded System Lab＿
// Engineer: Haojun Xia & Xuan Wang
// Create Date: 2019/02/22
// Design Name: RISCV-Pipline CPU
// Module Name: HarzardUnit
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: Deal with harzards in pipline
//////////////////////////////////////////////////////////////////////////////////
module HarzardUnit(
    input wire CpuRst, ICacheMiss, DCacheMiss, 
    input wire BranchE, JalrE, JalD, 
    input wire [4:0] Rs1D, Rs2D, Rs1E, Rs2E, RdE, RdM, RdW,
    input wire [1:0] RegReadE,
    input wire [2:0] MemToRegE, RegWriteM, RegWriteW,
    output reg StallF, FlushF, StallD, FlushD, StallE, FlushE, StallM, FlushM, StallW, FlushW,
    output reg [1:0] Forward1E, Forward2E
    );
	
	//Stall and Flush signals generate
	always@(*)     
	begin       
		if(CpuRst==1)//rst
		begin
			FlushF<=1;FlushD<=1;FlushE<=1;FlushM<=1;FlushW<=1;
			StallF<=0;StallD<=0;StallE<=0;StallM<=0;StallW<=0;
		end
		
		else//~rst
		begin
			if(BranchE||JalrE)//控制冒险
			begin
				FlushF<=0;FlushD<=1;FlushE<=1;FlushM<=0;FlushW<=0;
				StallF<=0;StallD<=0;StallE<=0;StallM<=0;StallW<=0;
			end
			
			else if(JalD)//控制冒险
			begin
				FlushF<=0;FlushD<=1;FlushE<=0;FlushM<=0;FlushW<=0;
				StallF<=0;StallD<=0;StallE<=0;StallM<=0;StallW<=0;
			end
			
			else if(MemToRegE&&((RdE==Rs1D)||(RdE==Rs2D))&&RdE!=0)//lw数据冒险
			begin
				FlushF<=0;FlushD<=0;FlushE<=0;FlushM<=0;FlushW<=0;
				StallF<=1;StallD<=1;StallE<=0;StallM<=0;StallW<=0;
			end
			
			else//无冒陿
			begin
				FlushF<=0;FlushD<=0;FlushE<=0;FlushM<=0;FlushW<=0;
				StallF<=0;StallD<=0;StallE<=0;StallM<=0;StallW<=0;
			end
		
		end
	end 
	
	
   	
	//Rs1D, Rs2D, Rs1E, Rs2E, RdE, RdM, RdW     用来处理数据相关，分别表示源寄存噿1号码，源寄存噿2号码，目标寄存器号码
    //RegReadE RegReadD[1]==1                   表示A1对应的寄存器值被使用到了，RegReadD[0]==1表示A2对应的寄存器值被使用到了，用于forward的处琿
    //RegWriteM, RegWriteW                      用来处理数据相关，RegWrite!=3'b0说明对目标寄存器有写入操使
    //MemToRegE                                 表示Ex段当前指仿 从Data Memory中加载数据到寄存器中

	
    //Forward Register Source 1
	always@(*)
	begin
		if((RegWriteM!=3'b0)&&(RdM!=0)&&(RdM==Rs1E)&&(RegReadE[1])) 	 
			Forward1E<=2'b10;
		else if((RegWriteW!=3'b0)&&(RdW!=0)&&!((RdM==Rs1E)&&(|RegWriteM))&&(RdW==Rs1E)&&(RegReadE[1]))
			Forward1E<=2'b01;
		else Forward1E<=2'b00;
	end

    //Forward Register Source 2
	always@(*)
	begin
		if((RegWriteM!=3'b0)&&(RdM!=0)&&(RdM==Rs2E)&&(RegReadE[0])) 	 
			Forward2E<=2'b10;
		else if((RegWriteW!=3'b0)&&(RdW!=0)&&!((RdM==Rs2E)&&(|RegWriteM))&&(RdW==Rs2E)&&(RegReadE[0]))
			Forward2E<=2'b01;
		else Forward2E<=2'b00;
	end
	

endmodule

//功能说明
    //HarzardUnit用来处理流水线冲突，通过插入气泡，forward以及冲刷流水段解决数据相关和控制相关，组合鿻辑电路
    //可以朿后实现㿂前期测试CPU正确性时，可以在每两条指令间插入四条空指令，然后直接把本模块输出定为，不forward，不stall，不flush 
//输入
    //CpuRst                                    外部信号，用来初始化CPU，当CpuRst==1时CPU全局复位清零（所有段寄存器flush），Cpu_Rst==0时cpu弿始执行指仿
    //ICacheMiss, DCacheMiss                    为后续实验预留信号，暂时可以无视，用来处理cache miss
    //BranchE, JalrE, JalD                      用来处理控制相关
    //Rs1D, Rs2D, Rs1E, Rs2E, RdE, RdM, RdW     用来处理数据相关，分别表示源寄存噿1号码，源寄存噿2号码，目标寄存器号码
    //RegReadE RegReadD[1]==1                   表示A1对应的寄存器值被使用到了，RegReadD[0]==1表示A2对应的寄存器值被使用到了，用于forward的处琿
    //RegWriteM, RegWriteW                      用来处理数据相关，RegWrite!=3'b0说明对目标寄存器有写入操使
    //MemToRegE                                 表示Ex段当前指仿 从Data Memory中加载数据到寄存器中
//输出
    //StallF, FlushF, StallD, FlushD, StallE, FlushE, StallM, FlushM, StallW, FlushW    控制五个段寄存器进行stall（维持状态不变）和flush（清零）
    //Forward1E, Forward2E                                                              控制forward
//实验要求  
    //实现HarzardUnit模块   M)&&(RdM!=0)&&(RdM==Rs2E)&&RegReadE[1];

