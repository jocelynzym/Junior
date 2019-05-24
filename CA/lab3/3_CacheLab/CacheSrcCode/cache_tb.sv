`timescale 1ns/100ps
//correct read result:
// 00000078 0000007d 0000003c 0000002c 00000011 0000000b 00000020 00000055 00000042 0000001f 00000010 0000000d 00000041 00000071 0000004c 00000066 00000045 00000034 00000055 00000074 00000054 0000002a 0000006b 00000011 0000000a 0000001a 00000038 00000002 00000004 00000013 00000043 0000003c

module cache_tb();

`define DATA_COUNT (32)
`define RDWR_COUNT (6*`DATA_COUNT)

reg wr_cycle           [`RDWR_COUNT];
reg rd_cycle           [`RDWR_COUNT];
reg [31:0] addr_rom    [`RDWR_COUNT];
reg [31:0] wr_data_rom [`RDWR_COUNT];
reg [31:0] validation_data [`DATA_COUNT];

initial begin
    // 32 sequence write cycles
    rd_cycle[    0] = 1'b0;  wr_cycle[    0] = 1'b1;  addr_rom[    0]='h00000000;  wr_data_rom[    0]='h00000078;
    rd_cycle[    1] = 1'b0;  wr_cycle[    1] = 1'b1;  addr_rom[    1]='h00000004;  wr_data_rom[    1]='h0000007f;
    rd_cycle[    2] = 1'b0;  wr_cycle[    2] = 1'b1;  addr_rom[    2]='h00000008;  wr_data_rom[    2]='h0000003b;
    rd_cycle[    3] = 1'b0;  wr_cycle[    3] = 1'b1;  addr_rom[    3]='h0000000c;  wr_data_rom[    3]='h0000006e;
    rd_cycle[    4] = 1'b0;  wr_cycle[    4] = 1'b1;  addr_rom[    4]='h00000010;  wr_data_rom[    4]='h0000003b;
    rd_cycle[    5] = 1'b0;  wr_cycle[    5] = 1'b1;  addr_rom[    5]='h00000014;  wr_data_rom[    5]='h00000063;
    rd_cycle[    6] = 1'b0;  wr_cycle[    6] = 1'b1;  addr_rom[    6]='h00000018;  wr_data_rom[    6]='h00000020;
    rd_cycle[    7] = 1'b0;  wr_cycle[    7] = 1'b1;  addr_rom[    7]='h0000001c;  wr_data_rom[    7]='h00000020;
    rd_cycle[    8] = 1'b0;  wr_cycle[    8] = 1'b1;  addr_rom[    8]='h00000020;  wr_data_rom[    8]='h00000042;
    rd_cycle[    9] = 1'b0;  wr_cycle[    9] = 1'b1;  addr_rom[    9]='h00000024;  wr_data_rom[    9]='h0000001f;
    rd_cycle[   10] = 1'b0;  wr_cycle[   10] = 1'b1;  addr_rom[   10]='h00000028;  wr_data_rom[   10]='h00000080;
    rd_cycle[   11] = 1'b0;  wr_cycle[   11] = 1'b1;  addr_rom[   11]='h0000002c;  wr_data_rom[   11]='h00000029;
    rd_cycle[   12] = 1'b0;  wr_cycle[   12] = 1'b1;  addr_rom[   12]='h00000030;  wr_data_rom[   12]='h00000061;
    rd_cycle[   13] = 1'b0;  wr_cycle[   13] = 1'b1;  addr_rom[   13]='h00000034;  wr_data_rom[   13]='h0000001c;
    rd_cycle[   14] = 1'b0;  wr_cycle[   14] = 1'b1;  addr_rom[   14]='h00000038;  wr_data_rom[   14]='h0000002b;
    rd_cycle[   15] = 1'b0;  wr_cycle[   15] = 1'b1;  addr_rom[   15]='h0000003c;  wr_data_rom[   15]='h00000066;
    rd_cycle[   16] = 1'b0;  wr_cycle[   16] = 1'b1;  addr_rom[   16]='h00000040;  wr_data_rom[   16]='h00000040;
    rd_cycle[   17] = 1'b0;  wr_cycle[   17] = 1'b1;  addr_rom[   17]='h00000044;  wr_data_rom[   17]='h00000070;
    rd_cycle[   18] = 1'b0;  wr_cycle[   18] = 1'b1;  addr_rom[   18]='h00000048;  wr_data_rom[   18]='h00000012;
    rd_cycle[   19] = 1'b0;  wr_cycle[   19] = 1'b1;  addr_rom[   19]='h0000004c;  wr_data_rom[   19]='h0000001a;
    rd_cycle[   20] = 1'b0;  wr_cycle[   20] = 1'b1;  addr_rom[   20]='h00000050;  wr_data_rom[   20]='h00000035;
    rd_cycle[   21] = 1'b0;  wr_cycle[   21] = 1'b1;  addr_rom[   21]='h00000054;  wr_data_rom[   21]='h00000070;
    rd_cycle[   22] = 1'b0;  wr_cycle[   22] = 1'b1;  addr_rom[   22]='h00000058;  wr_data_rom[   22]='h0000006b;
    rd_cycle[   23] = 1'b0;  wr_cycle[   23] = 1'b1;  addr_rom[   23]='h0000005c;  wr_data_rom[   23]='h00000011;
    rd_cycle[   24] = 1'b0;  wr_cycle[   24] = 1'b1;  addr_rom[   24]='h00000060;  wr_data_rom[   24]='h00000008;
    rd_cycle[   25] = 1'b0;  wr_cycle[   25] = 1'b1;  addr_rom[   25]='h00000064;  wr_data_rom[   25]='h00000022;
    rd_cycle[   26] = 1'b0;  wr_cycle[   26] = 1'b1;  addr_rom[   26]='h00000068;  wr_data_rom[   26]='h00000031;
    rd_cycle[   27] = 1'b0;  wr_cycle[   27] = 1'b1;  addr_rom[   27]='h0000006c;  wr_data_rom[   27]='h00000002;
    rd_cycle[   28] = 1'b0;  wr_cycle[   28] = 1'b1;  addr_rom[   28]='h00000070;  wr_data_rom[   28]='h00000001;
    rd_cycle[   29] = 1'b0;  wr_cycle[   29] = 1'b1;  addr_rom[   29]='h00000074;  wr_data_rom[   29]='h00000045;
    rd_cycle[   30] = 1'b0;  wr_cycle[   30] = 1'b1;  addr_rom[   30]='h00000078;  wr_data_rom[   30]='h00000044;
    rd_cycle[   31] = 1'b0;  wr_cycle[   31] = 1'b1;  addr_rom[   31]='h0000007c;  wr_data_rom[   31]='h00000004;
    // 96 random read and write cycles
    rd_cycle[   32] = 1'b1;  wr_cycle[   32] = 1'b0;  addr_rom[   32]='h0000005c;  wr_data_rom[   32]='h00000000;
    rd_cycle[   33] = 1'b0;  wr_cycle[   33] = 1'b1;  addr_rom[   33]='h0000001c;  wr_data_rom[   33]='h0000004d;
    rd_cycle[   34] = 1'b0;  wr_cycle[   34] = 1'b1;  addr_rom[   34]='h00000038;  wr_data_rom[   34]='h00000006;
    rd_cycle[   35] = 1'b0;  wr_cycle[   35] = 1'b1;  addr_rom[   35]='h00000034;  wr_data_rom[   35]='h00000071;
    rd_cycle[   36] = 1'b1;  wr_cycle[   36] = 1'b0;  addr_rom[   36]='h00000018;  wr_data_rom[   36]='h00000000;
    rd_cycle[   37] = 1'b1;  wr_cycle[   37] = 1'b0;  addr_rom[   37]='h00000078;  wr_data_rom[   37]='h00000000;
    rd_cycle[   38] = 1'b1;  wr_cycle[   38] = 1'b0;  addr_rom[   38]='h00000054;  wr_data_rom[   38]='h00000000;
    rd_cycle[   39] = 1'b1;  wr_cycle[   39] = 1'b0;  addr_rom[   39]='h00000024;  wr_data_rom[   39]='h00000000;
    rd_cycle[   40] = 1'b0;  wr_cycle[   40] = 1'b1;  addr_rom[   40]='h00000030;  wr_data_rom[   40]='h00000041;
    rd_cycle[   41] = 1'b1;  wr_cycle[   41] = 1'b0;  addr_rom[   41]='h00000078;  wr_data_rom[   41]='h00000000;
    rd_cycle[   42] = 1'b1;  wr_cycle[   42] = 1'b0;  addr_rom[   42]='h0000002c;  wr_data_rom[   42]='h00000000;
    rd_cycle[   43] = 1'b1;  wr_cycle[   43] = 1'b0;  addr_rom[   43]='h00000040;  wr_data_rom[   43]='h00000000;
    rd_cycle[   44] = 1'b1;  wr_cycle[   44] = 1'b0;  addr_rom[   44]='h0000006c;  wr_data_rom[   44]='h00000000;
    rd_cycle[   45] = 1'b0;  wr_cycle[   45] = 1'b1;  addr_rom[   45]='h00000008;  wr_data_rom[   45]='h00000027;
    rd_cycle[   46] = 1'b0;  wr_cycle[   46] = 1'b1;  addr_rom[   46]='h00000028;  wr_data_rom[   46]='h00000005;
    rd_cycle[   47] = 1'b0;  wr_cycle[   47] = 1'b1;  addr_rom[   47]='h00000074;  wr_data_rom[   47]='h00000021;
    rd_cycle[   48] = 1'b1;  wr_cycle[   48] = 1'b0;  addr_rom[   48]='h00000008;  wr_data_rom[   48]='h00000000;
    rd_cycle[   49] = 1'b0;  wr_cycle[   49] = 1'b1;  addr_rom[   49]='h0000001c;  wr_data_rom[   49]='h00000055;
    rd_cycle[   50] = 1'b1;  wr_cycle[   50] = 1'b0;  addr_rom[   50]='h00000008;  wr_data_rom[   50]='h00000000;
    rd_cycle[   51] = 1'b0;  wr_cycle[   51] = 1'b1;  addr_rom[   51]='h0000002c;  wr_data_rom[   51]='h0000000d;
    rd_cycle[   52] = 1'b1;  wr_cycle[   52] = 1'b0;  addr_rom[   52]='h00000060;  wr_data_rom[   52]='h00000000;
    rd_cycle[   53] = 1'b0;  wr_cycle[   53] = 1'b1;  addr_rom[   53]='h00000014;  wr_data_rom[   53]='h0000000b;
    rd_cycle[   54] = 1'b0;  wr_cycle[   54] = 1'b1;  addr_rom[   54]='h00000078;  wr_data_rom[   54]='h00000036;
    rd_cycle[   55] = 1'b0;  wr_cycle[   55] = 1'b1;  addr_rom[   55]='h00000070;  wr_data_rom[   55]='h0000006b;
    rd_cycle[   56] = 1'b0;  wr_cycle[   56] = 1'b1;  addr_rom[   56]='h00000070;  wr_data_rom[   56]='h00000004;
    rd_cycle[   57] = 1'b1;  wr_cycle[   57] = 1'b0;  addr_rom[   57]='h0000007c;  wr_data_rom[   57]='h00000000;
    rd_cycle[   58] = 1'b1;  wr_cycle[   58] = 1'b0;  addr_rom[   58]='h00000060;  wr_data_rom[   58]='h00000000;
    rd_cycle[   59] = 1'b1;  wr_cycle[   59] = 1'b0;  addr_rom[   59]='h00000010;  wr_data_rom[   59]='h00000000;
    rd_cycle[   60] = 1'b0;  wr_cycle[   60] = 1'b1;  addr_rom[   60]='h00000038;  wr_data_rom[   60]='h0000007a;
    rd_cycle[   61] = 1'b0;  wr_cycle[   61] = 1'b1;  addr_rom[   61]='h0000000c;  wr_data_rom[   61]='h0000004c;
    rd_cycle[   62] = 1'b1;  wr_cycle[   62] = 1'b0;  addr_rom[   62]='h00000014;  wr_data_rom[   62]='h00000000;
    rd_cycle[   63] = 1'b0;  wr_cycle[   63] = 1'b1;  addr_rom[   63]='h00000040;  wr_data_rom[   63]='h00000045;
    rd_cycle[   64] = 1'b1;  wr_cycle[   64] = 1'b0;  addr_rom[   64]='h00000010;  wr_data_rom[   64]='h00000000;
    rd_cycle[   65] = 1'b1;  wr_cycle[   65] = 1'b0;  addr_rom[   65]='h00000018;  wr_data_rom[   65]='h00000000;
    rd_cycle[   66] = 1'b0;  wr_cycle[   66] = 1'b1;  addr_rom[   66]='h00000074;  wr_data_rom[   66]='h00000059;
    rd_cycle[   67] = 1'b0;  wr_cycle[   67] = 1'b1;  addr_rom[   67]='h00000004;  wr_data_rom[   67]='h0000007d;
    rd_cycle[   68] = 1'b0;  wr_cycle[   68] = 1'b1;  addr_rom[   68]='h00000060;  wr_data_rom[   68]='h0000000a;
    rd_cycle[   69] = 1'b0;  wr_cycle[   69] = 1'b1;  addr_rom[   69]='h00000008;  wr_data_rom[   69]='h0000003c;
    rd_cycle[   70] = 1'b1;  wr_cycle[   70] = 1'b0;  addr_rom[   70]='h0000002c;  wr_data_rom[   70]='h00000000;
    rd_cycle[   71] = 1'b1;  wr_cycle[   71] = 1'b0;  addr_rom[   71]='h0000005c;  wr_data_rom[   71]='h00000000;
    rd_cycle[   72] = 1'b0;  wr_cycle[   72] = 1'b1;  addr_rom[   72]='h00000048;  wr_data_rom[   72]='h00000071;
    rd_cycle[   73] = 1'b1;  wr_cycle[   73] = 1'b0;  addr_rom[   73]='h00000044;  wr_data_rom[   73]='h00000000;
    rd_cycle[   74] = 1'b1;  wr_cycle[   74] = 1'b0;  addr_rom[   74]='h00000054;  wr_data_rom[   74]='h00000000;
    rd_cycle[   75] = 1'b0;  wr_cycle[   75] = 1'b1;  addr_rom[   75]='h00000064;  wr_data_rom[   75]='h00000021;
    rd_cycle[   76] = 1'b1;  wr_cycle[   76] = 1'b0;  addr_rom[   76]='h00000020;  wr_data_rom[   76]='h00000000;
    rd_cycle[   77] = 1'b0;  wr_cycle[   77] = 1'b1;  addr_rom[   77]='h00000054;  wr_data_rom[   77]='h00000041;
    rd_cycle[   78] = 1'b1;  wr_cycle[   78] = 1'b0;  addr_rom[   78]='h0000006c;  wr_data_rom[   78]='h00000000;
    rd_cycle[   79] = 1'b1;  wr_cycle[   79] = 1'b0;  addr_rom[   79]='h00000014;  wr_data_rom[   79]='h00000000;
    rd_cycle[   80] = 1'b1;  wr_cycle[   80] = 1'b0;  addr_rom[   80]='h00000078;  wr_data_rom[   80]='h00000000;
    rd_cycle[   81] = 1'b0;  wr_cycle[   81] = 1'b1;  addr_rom[   81]='h00000078;  wr_data_rom[   81]='h00000037;
    rd_cycle[   82] = 1'b1;  wr_cycle[   82] = 1'b0;  addr_rom[   82]='h00000070;  wr_data_rom[   82]='h00000000;
    rd_cycle[   83] = 1'b1;  wr_cycle[   83] = 1'b0;  addr_rom[   83]='h00000074;  wr_data_rom[   83]='h00000000;
    rd_cycle[   84] = 1'b1;  wr_cycle[   84] = 1'b0;  addr_rom[   84]='h0000002c;  wr_data_rom[   84]='h00000000;
    rd_cycle[   85] = 1'b0;  wr_cycle[   85] = 1'b1;  addr_rom[   85]='h00000048;  wr_data_rom[   85]='h00000055;
    rd_cycle[   86] = 1'b1;  wr_cycle[   86] = 1'b0;  addr_rom[   86]='h00000064;  wr_data_rom[   86]='h00000000;
    rd_cycle[   87] = 1'b1;  wr_cycle[   87] = 1'b0;  addr_rom[   87]='h00000038;  wr_data_rom[   87]='h00000000;
    rd_cycle[   88] = 1'b1;  wr_cycle[   88] = 1'b0;  addr_rom[   88]='h0000002c;  wr_data_rom[   88]='h00000000;
    rd_cycle[   89] = 1'b1;  wr_cycle[   89] = 1'b0;  addr_rom[   89]='h00000048;  wr_data_rom[   89]='h00000000;
    rd_cycle[   90] = 1'b0;  wr_cycle[   90] = 1'b1;  addr_rom[   90]='h00000074;  wr_data_rom[   90]='h00000013;
    rd_cycle[   91] = 1'b1;  wr_cycle[   91] = 1'b0;  addr_rom[   91]='h00000064;  wr_data_rom[   91]='h00000000;
    rd_cycle[   92] = 1'b1;  wr_cycle[   92] = 1'b0;  addr_rom[   92]='h00000058;  wr_data_rom[   92]='h00000000;
    rd_cycle[   93] = 1'b1;  wr_cycle[   93] = 1'b0;  addr_rom[   93]='h0000003c;  wr_data_rom[   93]='h00000000;
    rd_cycle[   94] = 1'b1;  wr_cycle[   94] = 1'b0;  addr_rom[   94]='h00000020;  wr_data_rom[   94]='h00000000;
    rd_cycle[   95] = 1'b1;  wr_cycle[   95] = 1'b0;  addr_rom[   95]='h00000010;  wr_data_rom[   95]='h00000000;
    rd_cycle[   96] = 1'b0;  wr_cycle[   96] = 1'b1;  addr_rom[   96]='h00000010;  wr_data_rom[   96]='h00000011;
    rd_cycle[   97] = 1'b0;  wr_cycle[   97] = 1'b1;  addr_rom[   97]='h00000038;  wr_data_rom[   97]='h00000062;
    rd_cycle[   98] = 1'b0;  wr_cycle[   98] = 1'b1;  addr_rom[   98]='h00000064;  wr_data_rom[   98]='h0000001a;
    rd_cycle[   99] = 1'b1;  wr_cycle[   99] = 1'b0;  addr_rom[   99]='h00000074;  wr_data_rom[   99]='h00000000;
    rd_cycle[  100] = 1'b0;  wr_cycle[  100] = 1'b1;  addr_rom[  100]='h00000078;  wr_data_rom[  100]='h00000043;
    rd_cycle[  101] = 1'b1;  wr_cycle[  101] = 1'b0;  addr_rom[  101]='h00000038;  wr_data_rom[  101]='h00000000;
    rd_cycle[  102] = 1'b1;  wr_cycle[  102] = 1'b0;  addr_rom[  102]='h00000070;  wr_data_rom[  102]='h00000000;
    rd_cycle[  103] = 1'b1;  wr_cycle[  103] = 1'b0;  addr_rom[  103]='h00000064;  wr_data_rom[  103]='h00000000;
    rd_cycle[  104] = 1'b1;  wr_cycle[  104] = 1'b0;  addr_rom[  104]='h00000010;  wr_data_rom[  104]='h00000000;
    rd_cycle[  105] = 1'b1;  wr_cycle[  105] = 1'b0;  addr_rom[  105]='h00000000;  wr_data_rom[  105]='h00000000;
    rd_cycle[  106] = 1'b0;  wr_cycle[  106] = 1'b1;  addr_rom[  106]='h00000054;  wr_data_rom[  106]='h00000043;
    rd_cycle[  107] = 1'b1;  wr_cycle[  107] = 1'b0;  addr_rom[  107]='h0000006c;  wr_data_rom[  107]='h00000000;
    rd_cycle[  108] = 1'b1;  wr_cycle[  108] = 1'b0;  addr_rom[  108]='h00000040;  wr_data_rom[  108]='h00000000;
    rd_cycle[  109] = 1'b0;  wr_cycle[  109] = 1'b1;  addr_rom[  109]='h00000028;  wr_data_rom[  109]='h00000010;
    rd_cycle[  110] = 1'b0;  wr_cycle[  110] = 1'b1;  addr_rom[  110]='h00000038;  wr_data_rom[  110]='h0000004c;
    rd_cycle[  111] = 1'b1;  wr_cycle[  111] = 1'b0;  addr_rom[  111]='h00000058;  wr_data_rom[  111]='h00000000;
    rd_cycle[  112] = 1'b0;  wr_cycle[  112] = 1'b1;  addr_rom[  112]='h0000004c;  wr_data_rom[  112]='h00000074;
    rd_cycle[  113] = 1'b1;  wr_cycle[  113] = 1'b0;  addr_rom[  113]='h00000078;  wr_data_rom[  113]='h00000000;
    rd_cycle[  114] = 1'b0;  wr_cycle[  114] = 1'b1;  addr_rom[  114]='h00000050;  wr_data_rom[  114]='h0000006f;
    rd_cycle[  115] = 1'b0;  wr_cycle[  115] = 1'b1;  addr_rom[  115]='h0000000c;  wr_data_rom[  115]='h0000002c;
    rd_cycle[  116] = 1'b1;  wr_cycle[  116] = 1'b0;  addr_rom[  116]='h00000024;  wr_data_rom[  116]='h00000000;
    rd_cycle[  117] = 1'b0;  wr_cycle[  117] = 1'b1;  addr_rom[  117]='h00000050;  wr_data_rom[  117]='h00000054;
    rd_cycle[  118] = 1'b1;  wr_cycle[  118] = 1'b0;  addr_rom[  118]='h00000060;  wr_data_rom[  118]='h00000000;
    rd_cycle[  119] = 1'b0;  wr_cycle[  119] = 1'b1;  addr_rom[  119]='h0000007c;  wr_data_rom[  119]='h0000003c;
    rd_cycle[  120] = 1'b1;  wr_cycle[  120] = 1'b0;  addr_rom[  120]='h00000054;  wr_data_rom[  120]='h00000000;
    rd_cycle[  121] = 1'b0;  wr_cycle[  121] = 1'b1;  addr_rom[  121]='h00000068;  wr_data_rom[  121]='h00000038;
    rd_cycle[  122] = 1'b1;  wr_cycle[  122] = 1'b0;  addr_rom[  122]='h00000000;  wr_data_rom[  122]='h00000000;
    rd_cycle[  123] = 1'b1;  wr_cycle[  123] = 1'b0;  addr_rom[  123]='h00000020;  wr_data_rom[  123]='h00000000;
    rd_cycle[  124] = 1'b1;  wr_cycle[  124] = 1'b0;  addr_rom[  124]='h00000028;  wr_data_rom[  124]='h00000000;
    rd_cycle[  125] = 1'b0;  wr_cycle[  125] = 1'b1;  addr_rom[  125]='h00000054;  wr_data_rom[  125]='h0000002a;
    rd_cycle[  126] = 1'b0;  wr_cycle[  126] = 1'b1;  addr_rom[  126]='h00000044;  wr_data_rom[  126]='h00000034;
    rd_cycle[  127] = 1'b1;  wr_cycle[  127] = 1'b0;  addr_rom[  127]='h00000008;  wr_data_rom[  127]='h00000000;
    // 32 silence cycles
    rd_cycle[  128] = 1'b0;  wr_cycle[  128] = 1'b0;  addr_rom[  128]='h00000000;  wr_data_rom[  128]='h00000000;
    rd_cycle[  129] = 1'b0;  wr_cycle[  129] = 1'b0;  addr_rom[  129]='h00000000;  wr_data_rom[  129]='h00000000;
    rd_cycle[  130] = 1'b0;  wr_cycle[  130] = 1'b0;  addr_rom[  130]='h00000000;  wr_data_rom[  130]='h00000000;
    rd_cycle[  131] = 1'b0;  wr_cycle[  131] = 1'b0;  addr_rom[  131]='h00000000;  wr_data_rom[  131]='h00000000;
    rd_cycle[  132] = 1'b0;  wr_cycle[  132] = 1'b0;  addr_rom[  132]='h00000000;  wr_data_rom[  132]='h00000000;
    rd_cycle[  133] = 1'b0;  wr_cycle[  133] = 1'b0;  addr_rom[  133]='h00000000;  wr_data_rom[  133]='h00000000;
    rd_cycle[  134] = 1'b0;  wr_cycle[  134] = 1'b0;  addr_rom[  134]='h00000000;  wr_data_rom[  134]='h00000000;
    rd_cycle[  135] = 1'b0;  wr_cycle[  135] = 1'b0;  addr_rom[  135]='h00000000;  wr_data_rom[  135]='h00000000;
    rd_cycle[  136] = 1'b0;  wr_cycle[  136] = 1'b0;  addr_rom[  136]='h00000000;  wr_data_rom[  136]='h00000000;
    rd_cycle[  137] = 1'b0;  wr_cycle[  137] = 1'b0;  addr_rom[  137]='h00000000;  wr_data_rom[  137]='h00000000;
    rd_cycle[  138] = 1'b0;  wr_cycle[  138] = 1'b0;  addr_rom[  138]='h00000000;  wr_data_rom[  138]='h00000000;
    rd_cycle[  139] = 1'b0;  wr_cycle[  139] = 1'b0;  addr_rom[  139]='h00000000;  wr_data_rom[  139]='h00000000;
    rd_cycle[  140] = 1'b0;  wr_cycle[  140] = 1'b0;  addr_rom[  140]='h00000000;  wr_data_rom[  140]='h00000000;
    rd_cycle[  141] = 1'b0;  wr_cycle[  141] = 1'b0;  addr_rom[  141]='h00000000;  wr_data_rom[  141]='h00000000;
    rd_cycle[  142] = 1'b0;  wr_cycle[  142] = 1'b0;  addr_rom[  142]='h00000000;  wr_data_rom[  142]='h00000000;
    rd_cycle[  143] = 1'b0;  wr_cycle[  143] = 1'b0;  addr_rom[  143]='h00000000;  wr_data_rom[  143]='h00000000;
    rd_cycle[  144] = 1'b0;  wr_cycle[  144] = 1'b0;  addr_rom[  144]='h00000000;  wr_data_rom[  144]='h00000000;
    rd_cycle[  145] = 1'b0;  wr_cycle[  145] = 1'b0;  addr_rom[  145]='h00000000;  wr_data_rom[  145]='h00000000;
    rd_cycle[  146] = 1'b0;  wr_cycle[  146] = 1'b0;  addr_rom[  146]='h00000000;  wr_data_rom[  146]='h00000000;
    rd_cycle[  147] = 1'b0;  wr_cycle[  147] = 1'b0;  addr_rom[  147]='h00000000;  wr_data_rom[  147]='h00000000;
    rd_cycle[  148] = 1'b0;  wr_cycle[  148] = 1'b0;  addr_rom[  148]='h00000000;  wr_data_rom[  148]='h00000000;
    rd_cycle[  149] = 1'b0;  wr_cycle[  149] = 1'b0;  addr_rom[  149]='h00000000;  wr_data_rom[  149]='h00000000;
    rd_cycle[  150] = 1'b0;  wr_cycle[  150] = 1'b0;  addr_rom[  150]='h00000000;  wr_data_rom[  150]='h00000000;
    rd_cycle[  151] = 1'b0;  wr_cycle[  151] = 1'b0;  addr_rom[  151]='h00000000;  wr_data_rom[  151]='h00000000;
    rd_cycle[  152] = 1'b0;  wr_cycle[  152] = 1'b0;  addr_rom[  152]='h00000000;  wr_data_rom[  152]='h00000000;
    rd_cycle[  153] = 1'b0;  wr_cycle[  153] = 1'b0;  addr_rom[  153]='h00000000;  wr_data_rom[  153]='h00000000;
    rd_cycle[  154] = 1'b0;  wr_cycle[  154] = 1'b0;  addr_rom[  154]='h00000000;  wr_data_rom[  154]='h00000000;
    rd_cycle[  155] = 1'b0;  wr_cycle[  155] = 1'b0;  addr_rom[  155]='h00000000;  wr_data_rom[  155]='h00000000;
    rd_cycle[  156] = 1'b0;  wr_cycle[  156] = 1'b0;  addr_rom[  156]='h00000000;  wr_data_rom[  156]='h00000000;
    rd_cycle[  157] = 1'b0;  wr_cycle[  157] = 1'b0;  addr_rom[  157]='h00000000;  wr_data_rom[  157]='h00000000;
    rd_cycle[  158] = 1'b0;  wr_cycle[  158] = 1'b0;  addr_rom[  158]='h00000000;  wr_data_rom[  158]='h00000000;
    rd_cycle[  159] = 1'b0;  wr_cycle[  159] = 1'b0;  addr_rom[  159]='h00000000;  wr_data_rom[  159]='h00000000;
    // 32 sequence read cycles
    rd_cycle[  160] = 1'b1;  wr_cycle[  160] = 1'b0;  addr_rom[  160]='h00000000;  wr_data_rom[  160]='h00000000;
    rd_cycle[  161] = 1'b1;  wr_cycle[  161] = 1'b0;  addr_rom[  161]='h00000004;  wr_data_rom[  161]='h00000000;
    rd_cycle[  162] = 1'b1;  wr_cycle[  162] = 1'b0;  addr_rom[  162]='h00000008;  wr_data_rom[  162]='h00000000;
    rd_cycle[  163] = 1'b1;  wr_cycle[  163] = 1'b0;  addr_rom[  163]='h0000000c;  wr_data_rom[  163]='h00000000;
    rd_cycle[  164] = 1'b1;  wr_cycle[  164] = 1'b0;  addr_rom[  164]='h00000010;  wr_data_rom[  164]='h00000000;
    rd_cycle[  165] = 1'b1;  wr_cycle[  165] = 1'b0;  addr_rom[  165]='h00000014;  wr_data_rom[  165]='h00000000;
    rd_cycle[  166] = 1'b1;  wr_cycle[  166] = 1'b0;  addr_rom[  166]='h00000018;  wr_data_rom[  166]='h00000000;
    rd_cycle[  167] = 1'b1;  wr_cycle[  167] = 1'b0;  addr_rom[  167]='h0000001c;  wr_data_rom[  167]='h00000000;
    rd_cycle[  168] = 1'b1;  wr_cycle[  168] = 1'b0;  addr_rom[  168]='h00000020;  wr_data_rom[  168]='h00000000;
    rd_cycle[  169] = 1'b1;  wr_cycle[  169] = 1'b0;  addr_rom[  169]='h00000024;  wr_data_rom[  169]='h00000000;
    rd_cycle[  170] = 1'b1;  wr_cycle[  170] = 1'b0;  addr_rom[  170]='h00000028;  wr_data_rom[  170]='h00000000;
    rd_cycle[  171] = 1'b1;  wr_cycle[  171] = 1'b0;  addr_rom[  171]='h0000002c;  wr_data_rom[  171]='h00000000;
    rd_cycle[  172] = 1'b1;  wr_cycle[  172] = 1'b0;  addr_rom[  172]='h00000030;  wr_data_rom[  172]='h00000000;
    rd_cycle[  173] = 1'b1;  wr_cycle[  173] = 1'b0;  addr_rom[  173]='h00000034;  wr_data_rom[  173]='h00000000;
    rd_cycle[  174] = 1'b1;  wr_cycle[  174] = 1'b0;  addr_rom[  174]='h00000038;  wr_data_rom[  174]='h00000000;
    rd_cycle[  175] = 1'b1;  wr_cycle[  175] = 1'b0;  addr_rom[  175]='h0000003c;  wr_data_rom[  175]='h00000000;
    rd_cycle[  176] = 1'b1;  wr_cycle[  176] = 1'b0;  addr_rom[  176]='h00000040;  wr_data_rom[  176]='h00000000;
    rd_cycle[  177] = 1'b1;  wr_cycle[  177] = 1'b0;  addr_rom[  177]='h00000044;  wr_data_rom[  177]='h00000000;
    rd_cycle[  178] = 1'b1;  wr_cycle[  178] = 1'b0;  addr_rom[  178]='h00000048;  wr_data_rom[  178]='h00000000;
    rd_cycle[  179] = 1'b1;  wr_cycle[  179] = 1'b0;  addr_rom[  179]='h0000004c;  wr_data_rom[  179]='h00000000;
    rd_cycle[  180] = 1'b1;  wr_cycle[  180] = 1'b0;  addr_rom[  180]='h00000050;  wr_data_rom[  180]='h00000000;
    rd_cycle[  181] = 1'b1;  wr_cycle[  181] = 1'b0;  addr_rom[  181]='h00000054;  wr_data_rom[  181]='h00000000;
    rd_cycle[  182] = 1'b1;  wr_cycle[  182] = 1'b0;  addr_rom[  182]='h00000058;  wr_data_rom[  182]='h00000000;
    rd_cycle[  183] = 1'b1;  wr_cycle[  183] = 1'b0;  addr_rom[  183]='h0000005c;  wr_data_rom[  183]='h00000000;
    rd_cycle[  184] = 1'b1;  wr_cycle[  184] = 1'b0;  addr_rom[  184]='h00000060;  wr_data_rom[  184]='h00000000;
    rd_cycle[  185] = 1'b1;  wr_cycle[  185] = 1'b0;  addr_rom[  185]='h00000064;  wr_data_rom[  185]='h00000000;
    rd_cycle[  186] = 1'b1;  wr_cycle[  186] = 1'b0;  addr_rom[  186]='h00000068;  wr_data_rom[  186]='h00000000;
    rd_cycle[  187] = 1'b1;  wr_cycle[  187] = 1'b0;  addr_rom[  187]='h0000006c;  wr_data_rom[  187]='h00000000;
    rd_cycle[  188] = 1'b1;  wr_cycle[  188] = 1'b0;  addr_rom[  188]='h00000070;  wr_data_rom[  188]='h00000000;
    rd_cycle[  189] = 1'b1;  wr_cycle[  189] = 1'b0;  addr_rom[  189]='h00000074;  wr_data_rom[  189]='h00000000;
    rd_cycle[  190] = 1'b1;  wr_cycle[  190] = 1'b0;  addr_rom[  190]='h00000078;  wr_data_rom[  190]='h00000000;
    rd_cycle[  191] = 1'b1;  wr_cycle[  191] = 1'b0;  addr_rom[  191]='h0000007c;  wr_data_rom[  191]='h00000000;
end

initial begin
    validation_data[    0] = 'h00000078; 
    validation_data[    1] = 'h0000007d; 
    validation_data[    2] = 'h0000003c; 
    validation_data[    3] = 'h0000002c; 
    validation_data[    4] = 'h00000011; 
    validation_data[    5] = 'h0000000b; 
    validation_data[    6] = 'h00000020; 
    validation_data[    7] = 'h00000055; 
    validation_data[    8] = 'h00000042; 
    validation_data[    9] = 'h0000001f; 
    validation_data[   10] = 'h00000010; 
    validation_data[   11] = 'h0000000d; 
    validation_data[   12] = 'h00000041; 
    validation_data[   13] = 'h00000071; 
    validation_data[   14] = 'h0000004c; 
    validation_data[   15] = 'h00000066; 
    validation_data[   16] = 'h00000045; 
    validation_data[   17] = 'h00000034; 
    validation_data[   18] = 'h00000055; 
    validation_data[   19] = 'h00000074; 
    validation_data[   20] = 'h00000054; 
    validation_data[   21] = 'h0000002a; 
    validation_data[   22] = 'h0000006b; 
    validation_data[   23] = 'h00000011; 
    validation_data[   24] = 'h0000000a; 
    validation_data[   25] = 'h0000001a; 
    validation_data[   26] = 'h00000038; 
    validation_data[   27] = 'h00000002; 
    validation_data[   28] = 'h00000004; 
    validation_data[   29] = 'h00000013; 
    validation_data[   30] = 'h00000043; 
    validation_data[   31] = 'h0000003c; 

end


reg clk = 1'b1, rst = 1'b1;
initial #4 rst = 1'b0;
always  #1 clk = ~clk;

wire  miss;
wire [31:0] rd_data;
reg  [31:0] index = 0, wr_data = 0, addr = 0;
reg  rd_req = 1'b0, wr_req = 1'b0;
reg rd_req_ff = 1'b0, miss_ff = 1'b0;
reg [31:0] validation_count = 0;

always @ (posedge clk or posedge rst)
    if(rst) begin
        rd_req_ff <= 1'b0;
        miss_ff   <= 1'b0;
    end else begin
        rd_req_ff <= rd_req;
        miss_ff   <= miss;
    end

always @ (posedge clk or posedge rst)
    if(rst) begin
        validation_count <= 0;
    end else begin
        if(validation_count>=`DATA_COUNT) begin
            validation_count <= 'hffffffff;
        end else if(rd_req_ff && (index>(4*`DATA_COUNT))) begin
            if(~miss_ff) begin
                if(validation_data[validation_count]==rd_data)
                    validation_count <= validation_count+1;
                else
                    validation_count <= 0;
            end
        end else begin
            validation_count <= 0;
        end
    end

always @ (posedge clk or posedge rst)
    if(rst) begin
        index   <= 0;
        wr_data <= 0;
        addr    <= 0;
        rd_req  <= 1'b0;
        wr_req  <= 1'b0;
    end else begin
        if(~miss) begin
            if(index<`RDWR_COUNT) begin
                if(wr_cycle[index]) begin
                    rd_req  <= 1'b0;
                    wr_req  <= 1'b1;
                end else if(rd_cycle[index]) begin
                    wr_data <= 0;
                    rd_req  <= 1'b1;
                    wr_req  <= 1'b0;
                end else begin
                    wr_data <= 0;
                    rd_req  <= 1'b0;
                    wr_req  <= 1'b0;
                end
                wr_data <= wr_data_rom[index];
                addr    <= addr_rom[index];
                index <= index + 1;
            end else begin
                wr_data <= 0;
                addr    <= 0;
                rd_req  <= 1'b0;
                wr_req  <= 1'b0;
            end
        end
    end

cache #(
    .LINE_ADDR_LEN  ( 3             ),
    .SET_ADDR_LEN   ( 2             ),
    .TAG_ADDR_LEN   ( 12            ),
    .WAY_CNT        ( 3             )
) cache_test_instance (
    .clk            ( clk           ),
    .rst            ( rst           ),
    .miss           ( miss          ),
    .addr           ( addr          ),
    .rd_req         ( rd_req        ),
    .rd_data        ( rd_data       ),
    .wr_req         ( wr_req        ),
    .wr_data        ( wr_data       )
);

endmodule

