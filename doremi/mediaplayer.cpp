#include "mediaplayer.h"
#include "pitch.h"

enum Voice
{
    PA = 100000001 , // Prohibition of access , 作为一个空置的参数 , 判断用
    // R  = 1009 , // Rest  , 表示声音的延长
    // S  = 1013 , // Stop  , 表示休止符
    P  = 1019 , // Pedal , 表示踏板的踩下(或松开)
};

//声音指令，每8一格,最小为0x1f,最大0x7f,共十三档,对应数组_V[1]到_V[13]
const int _V[15]={0,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,0};

//乐谱
/*
msc music[] =   //完成进度 100%
    {

        // 用缩进区分乐句和小节
        // 如果时间充足的话,我会在每个乐句旁附上歌词,也算是增加可读性
        //时间充足，但是这个歌词加上没有意义，不整了

        // 设置速度示例 : 在第五次改板子之后,暂时不想碰这玩意

        // 每一个 四分之一小节 用一个复合结构体来表示。

        // 每一个单元的格式示例 {{R,R,R},{R,R,R},{R,R,R},P},
        // 其中每一个R都可以用一个音来代替,每一组中第一个R可以使用S
        //可用命令 :
        //            0   无命令
        //            P   改变踏板状态 (本曲未使用踏板(我懒而已))
        //          _V[i] 控制音量 ( 1 <= i <= 13 )
        {{C4,R,R},{R,R,R},{R,R,R},_V[6]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        //Part 1

        {{G5S,R,R},{S,S,S},{B3,F4S,A4S},_V[2]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{C4S,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{G5S,R,R},{R,R,R},{D4S,A4S,C5S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{F3S,C4S,A4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{G5S,R,R},{R,R,R},{B3,F4S,A4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{C4S,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{G5S,R,R},{R,R,R},{D4S,A4S,C5S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{F3S,C4S,A4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},


        {{G5S,R,R},{A4S,R,R},{B2,F3S,A3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{A4S,R,R},{C3S,G3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{G5S,R,R},{G4S,R,R},{D3S,A3S,C4S},_V[4]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{G4S,R,R},{F2S,C2S,A2S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{G5S,R,R},{F4S,R,R},{B2,F3S,A3S},_V[5]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{A4S,R,R},{C3S,G3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{G5S,R,R},{G4S,R,R},{D3S,A3S,C4S},_V[6]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{G4S,R,R},{F2S,C2S,A2S},0}, {{R,R,R},{R,R,R},{R,R,R},P},
        {{A4S,R,R},{S,S,S},{R,R,R},P}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        // 前奏结束


        {{A4S,D5S,R},{F3S,A3S,R},{B2,R,R},_V[7]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C5S,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{B4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{A4S,R,R},{F3S,A3S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{F3S,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},P},

        {{D5S,R,R},{F3S,A3S,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5,R,R},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{F3S,A3S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{S,S,S},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4S,R,R},{F3S,A3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},


        {{A4S,D5S,R},{F3S,A3S,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C5S,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{B4,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{A4S,R,R},{F3S,A3S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{F3S,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{D5S,R,R},{F3S,A3S,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5,R,R},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C5S,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{F3S,A3S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{S,S,S},{D4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{F3S,A3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},



        {{F4S,F5S,R},{S,S,S},{B1,F2S,B2},_V[9]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,C4S},0},
        {{S,S,S},{R,R,R},{S,S,S},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{C5S,F5,R},{R,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{S,S,S},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,F5,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F4S,F5S,R},{R,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{R,R,R},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,F5,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,F4S},0},
        {{D5S,R,R},{R,R,R},{S,S,S},0}, {{R,R,R},{R,R,R},{D3S,R,R},0}, {{C5S,F5,R},{R,R,R},{F3S,A3S,F4S},0}, {{R,R,R},{R,R,R},{S,S,S},0},
        {{D5S,F5S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,F5,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4S,C5S,R},{R,R,R},{F3S,A3S,F4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{A4S,C5S,R},{R,R,R},{B1,F2S,B2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{F3S,R,R},0}, {{R,R,R},{R,R,R},{A3S,C4S,R},0},
        {{F4S,R,R},{R,R,R},{S,S,S},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{D5S,R,R},{R,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{S,S,S},0},
        {{A4S,C5S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F4,G4S,R},{R,R,R},{F3,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,A4S,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{R,R,R},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,C4S},0},
        {{R,R,R},{R,R,R},{S,S,S},0}, {{R,R,R},{R,R,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{F4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},


        {{F5S,R,R},{R,R,R},{B1,F2S,B2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,C4S},0}, {{F5,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{R,R,R},{C3S,C4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{R,R,R},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,R,R},{R,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,C4S},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,A6S,R},{R,R,R},{F2S,F3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{B5,B6,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{B3,R,R},{F3S,D4S,F4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,F6S,R},{S,S,S},{C5S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,A6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,G6S,R},{R,R,R},{D5S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,F6S,R},{R,R,R},{F4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{F1S,R,R},{R,R,R},_V[10]}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{F1S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0},
        {{R,R,R},{F1S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{F1S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0},
        {{R,R,R},{F1S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{F1S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0},
        {{A4S,A5S,R},{S,S,S},{F1S,F2S,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{C5S,C6S,R},{R,R,R},{S,S,S},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0},
        // 副歌


        {{G4S,G5S,R},{S,S,S},{B1,F2S,B2},_V[11]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{D4S,D5S,R},{R,R,R},{F3S,A3S,R},0}, {{F4S,F5S,R},{R,R,R},{S,S,S},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{G4S,G5S,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{R,R,R},{F3,G3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4S,A5S,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{R,R,R},{F3,G3S,C4S},0}, {{C5S,C6S,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0},

        {{G4S,G5S,R},{S,S,S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{C4S,C5S,R},{R,R,R},{F3S,A3S,C4S},0}, {{F4S,F5S,R},{R,R,R},{S,S,S},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{A1S,R,R},0}, {{R,R,R},{A2S,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4S,A5S,R},{R,R,R},{S,S,S},0}, {{R,R,R},{S,S,S},{F3S,A3S,C4S},0}, {{C5S,C6S,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0},

        {{G4S,G5S,R},{S,S,S},{B1,F2S,B2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{A4S,A5S,R},{R,R,R},{F3S,A3S,R},0}, {{C5S,C6S,R},{R,R,R},{S,S,S},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{C5S,C6S,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{D5S,D6S,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{R,R,R},{F3,G3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C5S,C6S,R},{R,R,R},{F3,G3S,C4S},0}, {{B4,B5,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{S,S,S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{S,S,S},{G6S,R,R},{R,R,R},0}, {{R,R,R},{A6S,R,R},{D3S,R,R},0},
        {{R,R,R},{C7S,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{D7S,R,R},{R,R,R},0}, {{R,R,R},{A6S,R,R},{R,R,R},0}, {{R,R,R},{G6S,R,R},{F3S,A3S,C4S},0},
        {{R,R,R},{A6S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F1S,F2S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4S,A5S,R},{S,S,S},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{F2S,F3S,R},0}, {{C5S,C6S,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0},


        {{G4S,G5S,R},{S,S,S},{G1,F2S,G2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{G2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{D4S,D5S,R},{R,R,R},{F4S,A4S,R},0}, {{F4S,F5S,R},{R,R,R},{S,S,S},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{G4S,G5S,R},{R,R,R},{F4S,A4S,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{R,R,R},{F3,G3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4S,A5S,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{R,R,R},{F3,G3S,C4S},0}, {{C5S,C6S,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0},

        {{G4S,G5S,R},{S,S,S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{C4S,C5S,R},{R,R,R},{F3S,A3S,C4S},0}, {{F4S,F5S,R},{R,R,R},{S,S,S},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{A1S,R,R},0}, {{R,R,R},{A2S,R,R},{F3S,A3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F4S,F5S,R},{R,R,R},{S,S,S},0}, {{F4,F5,R},{S,S,S},{F3S,A3S,C4S},0}, {{D4S,D5S,R},{R,R,R},{R,R,R},0}, {{F4,F5,R},{R,R,R},{R,R,R},0},

        {{D4S,D5S,R},{S,S,S},{B1,F2S,B2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{G4S,G5S,R},{R,R,R},{F3S,A3S,R},0}, {{R,R,R},{R,R,R},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,R},0},
        {{F4,F5,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,R,R},0}, {{D4S,D5S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F4,F5,R},{R,R,R},{F3,G3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4,C5S,F5},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{R,R,R},{B1,B2,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{F3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F4S,C5S,F5S},{R,R,R},{C2S,C3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{G2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F4S,A4S,R},{R,R,R},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,R,R},0},
        {{S,S,S},{R,R,R},{A3S,R,R},0}, {{R,R,R},{R,R,R},{C4S,R,R},0}, {{R,R,R},{R,R,R},{D4S,R,R},0}, {{R,R,R},{R,R,R},{F4S,R,R},0},





        //Part 2

        {{G5S,R,R},{B3,F4S,A4S},{S,S,S},_V[5]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{C4S,G4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{D4S,A4S,C5S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{F3S,C4S,A4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{B3,F4S,A4S},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{C4S,G4S,R},{A4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{A4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{A4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{D4S,F4S,R},{G4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{G4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{G4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{G4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{G4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{G4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{G4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{S,S,S},{R,R,R},0}, {{G4S,R,R},{R,R,R},{S,S,S},0},


        {{A4S,R,R},{A2S,D3S,F3S},{S,S,S},_V[7]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G4S,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{R,R,R},0},
        {{F4S,R,R},{D5S,F5S,A5S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D4S,R,R},{S,S,S},{R,R,R},0}, {{F4,R,R},{R,R,R},{R,R,R},0},
        {{F4S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4,R,R},{C3S,F3,G3S},{R,R,R},0}, {{D4S,R,R},{R,R,R},{R,R,R},0},
        {{C4S,R,R},{A5S,G5S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A3S,R,R},{C3S,R,R},{R,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0},

        {{D4S,R,R},{D3S,F3S,A3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{R,R,R},0},
        {{F4,R,R},{D5S,F5S,A5S},{R,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0}, {{C4S,R,R},{S,S,S},{R,R,R},0}, {{D4S,R,R},{R,R,R},{R,R,R},0},
        {{C4S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F2S,A3S,F3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A3S,R,R},{A4S,C5S,G5S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A3S,R,R},{F2S,R,R},{R,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0},

        {{D4S,R,R},{B2,D3S,F3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{R,R,R},0},
        {{F4,R,R},{C5S,F5S,A5S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{S,S,S},{R,R,R},0}, {{G4S,R,R},{R,R,R},{R,R,R},0},
        {{A4S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G4S,R,R},{C3S,F3,G3S},{R,R,R},0}, {{F4S,R,R},{R,R,R},{R,R,R},0},
        {{F4,R,R},{C5S,G5S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C4S,R,R},{C3S,R,R},{R,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{D3S,F3S,G3S},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F5S,R,R},0}, {{R,R,R},{R,R,R},{G5S,R,R},0},
        {{R,R,R},{F4S,A4S,R},{A5S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{G5S,R,R},0}, {{R,R,R},{F4S,A4S,R},{A5S,R,R},0},
        {{R,R,R},{R,R,R},{C6S,R,R},0}, {{R,R,R},{R,R,R},{D6S,R,R},0}, {{R,R,R},{F2S,R,R},{A5S,R,R},0}, {{R,R,R},{R,R,R},{G5S,R,R},0},
        {{S,S,S},{F4S,A4S,R},{A5S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{F2S,R,R},{R,R,R},0}, {{G4S,R,R},{R,R,R},{R,R,R},0},



        {{A4S,R,R},{A2S,D3S,F3S},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G4S,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{R,R,R},0},
        {{F4S,R,R},{D5S,F5S,A5S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D4S,R,R},{S,S,S},{R,R,R},0}, {{F4,R,R},{R,R,R},{R,R,R},0},
        {{F4S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4,R,R},{C3S,F3,G3S},{R,R,R},0}, {{D4S,R,R},{R,R,R},{R,R,R},0},
        {{C4S,R,R},{A5S,G5S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A3S,R,R},{C3S,R,R},{R,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0},

        {{D4S,R,R},{D3S,F3S,A3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{R,R,R},0},
        {{F4,R,R},{F5S,D5S,A5S},{R,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0}, {{C4S,R,R},{S,S,S},{R,R,R},0}, {{D4S,R,R},{R,R,R},{R,R,R},0},
        {{C4S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F2S,C3S,F3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D4S,R,R},{S,S,S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F2S,R,R},{R,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{B2,D3S,F3S},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{B3,D4S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{D5S,F5S,A5S},{R,R,R},0}, {{D4S,F4S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F4,G4S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C3S,F3,G3S},{R,R,R},0}, {{F4S,A4S,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{C5S,F5,G5S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F4,G4S,R},{C3S,R,R},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{D3S,F3S,A3S},{S,S,S},0}, {{D4S,F4S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{S,S,S},{R,R,R},{D5S,F5S,A5S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{D2S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{F5S,R,R},0}, {{R,R,R},{R,R,R},{G5S,R,R},0}, {{F3S,R,R},{S,S,S},{A5S,R,R},0}, {{C4S,R,R},{R,R,R},{R,R,R},0},
        {{F4S,R,R},{R,R,R},{C6S,R,R},0}, {{G4S,R,R},{R,R,R},{R,R,R},0}, {{A4S,R,R},{R,R,R},{A5S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},


        {{D5S,A5S,F5S},{B1,F2S,B2},{D6S,R,R},_V[9]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{F3S,R,R},{R,R,R},0},
        {{C5S,C6S,R},{B3,D4S,R},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{B4,B5,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B3,D4S,R},{R,R,R},0},
        {{A4S,A5S,R},{S,S,S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C2S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G4S,G5S,R},{F3,G3S,C4S},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0}, {{B4,B5,R},{C2S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{D2S,A2S,D3S},{S,S,S},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0}, {{G4S,G5S,R},{D3S,R,R},{R,R,R},0}, {{F4S,F5S,R},{F3S,R,R},{R,R,R},0},
        {{R,R,R},{A3S,C4S,R},{R,R,R},0}, {{G4S,G5S,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0}, {{B4,B5,R},{A3S,C4S,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0}, {{G4S,G5S,R},{C2S,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F3S,A3S,C4S},{R,R,R},0}, {{D4S,D5S,R},{R,R,R},{R,R,R},0}, {{F4,F5,R},{C2S,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{B1,F2S,B2},{R,R,R},0}, {{F4,F5,R},{R,R,R},{R,R,R},0}, {{D4S,D5S,R},{B2,R,R},{R,R,R},0}, {{C4S,C5S,R},{F3S,R,R},{R,R,R},0},
        {{R,R,R},{B3,D4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B3,D4S,R},{R,R,R},0},
        {{B4,D5S,R},{S,S,S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C2S,R,R},{R,R,R},0}, {{D5S,F5S,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,F4,R},{C2S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{D2S,A2S,D3S},{R,R,R},0}, {{A4S,C5S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0}, {{R,R,R},{F3S,R,R},{R,R,R},0},
        {{F4S,A4S,R},{A3S,C4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A4S,D5S,R},{A3S,C4S,R},{R,R,R},0},
        {{D4S,R,R},{R,R,R},{R,R,R},0}, {{F4S,R,R},{R,R,R},{R,R,R},0}, {{G4S,R,R},{F2S,R,R},{R,R,R},0}, {{A4S,R,R},{R,R,R},{R,R,R},0},
        {{C5S,R,R},{A3S,C4S,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},



        {{A5S,D6S,R},{B2,F3S,B3},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,C6S,R},{F4S,A3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,B5,R},{F4S,A3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,A5S,R},{F4S,A3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F4S,A3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5,G5S,R},{C3S,G3S,C4S},{R,R,R},0}, {{F5S,A5S,R},{R,R,R},{R,R,R},0}, {{G5S,B5,R},{F4,G4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{D3S,A3S,D4S},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F4S,A4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,A5S,R},{F4S,A4S,R},{R,R,R},0}, {{F5,G5S,R},{R,R,R},{R,R,R},0}, {{F5S,A5S,R},{F4S,A4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F4S,A4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F4S,A4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{S,S,S},{F2S,C3S,F3S},{F4,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F4,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{B2,F3S,B3},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,B5,R},{S,S,S},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,A5S,R},{R,R,R},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,G5S,R},{R,R,R},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,R,R},{C3S,G3S,C4S},{F4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{F4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{B1,B2,R},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{R,R,R},{F3S,A3S,R},{D4S,F4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{F3S,A3S,R},{D4S,F4S,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B1,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F3S,A3S,R},{D4S,F4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B1,R,R},{R,R,R},0}, {{R,R,R},{F2S,R,R},{R,R,R},0},


        {{S,S,S},{B1,B2,R},{D4S,F4S,G4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{R,R,R},{F3S,A3S,R},{D4S,F4S,G4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{F3S,A3S,R},{D4S,F4S,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{G4S,R,R},0}, {{R,R,R},{B1,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F3S,A3S,R},{D4S,F4S,A4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B1,R,R},{R,R,R},0}, {{R,R,R},{F2S,R,R},{R,R,R},0},

        {{C5S,R,R},{C2S,C3S,R},{C4S,F4S,G4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{C3S,R,R},{S,S,S},0}, {{C5S,R,R},{F3S,R,R},{R,R,R},0},
        {{C5S,R,R},{G3S,C4S,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{C3S,R,R},{R,R,R},0}, {{C5S,R,R},{G3S,C4S,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{C2S,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{R,R,R},0},
        {{F4S,F5S,R},{G3S,C4S,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{R,R,R},0}, {{F4S,F5S,R},{C3S,R,R},{R,R,R},0}, {{F4S,F5S,R},{R,R,R},{R,R,R},0},

        {{G4S,C5S,G5S},{C1S,C2S,R},{S,S,S},_V[10]}, {{G4S,G5S,R},{R,R,R},{R,R,R},0}, {{G4S,G5S,R},{C1S,R,R},{R,R,R},0}, {{G4S,G5S,R},{R,R,R},{R,R,R},0},
        {{A4S,C5S,A5S},{C1S,C2S,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0}, {{A4S,A5S,R},{C1S,R,R},{R,R,R},0}, {{A4S,A5S,R},{R,R,R},{R,R,R},0},
        {{C5S,C6S,R},{C1S,C2S,R},{R,R,R},0}, {{C5S,C6S,R},{R,R,R},{R,R,R},0}, {{C5S,C6S,R},{C1S,R,R},{R,R,R},0}, {{C5S,C6S,R},{R,R,R},{R,R,R},0},
        {{A5S,A6S,R},{S,S,S},{R,R,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{C6S,C7S,R},{R,R,R},{R,R,R},0}, {{A5S,A6S,R},{R,R,R},{R,R,R},0},

        {{G5S,G6S,R},{B1,F2S,B2},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{D5S,D6S,R},{F3S,A3S,R},{R,R,R},0}, {{F5S,F6S,R},{S,S,S},{R,R,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{G5S,G6S,R},{F3S,A3S,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C2S,R,R},{R,R,R},0}, {{R,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,A6S,R},{C3S,R,R},{R,R,R},0}, {{R,R,R},{F3,G3S,C4S},{R,R,R},0}, {{C6S,C7S,R},{R,R,R},{R,R,R},0}, {{A5S,A6S,R},{R,R,R},{R,R,R},0},



        {{G5S,G6S,R},{D2S,A2S,D3S},{R,R,R},_V[11]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{C5S,C6S,R},{F3S,A3S,R},{R,R,R},0}, {{F5S,F6S,R},{S,S,S},{R,R,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{F3S,A3S,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{A1S,R,R},{R,R,R},0}, {{R,R,R},{F3S,A3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,A6S,R},{S,S,S},{R,R,R},0}, {{R,R,R},{F3S,A3S,C4S},{R,R,R},0}, {{C6S,C7S,R},{R,R,R},{R,R,R},0}, {{A5S,A6S,R},{R,R,R},{R,R,R},0},

        {{G5S,G6S,R},{B1,F2S,B2},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{A5S,A6S,R},{F3S,A3S,C4S},{R,R,R},0}, {{C6S,C7S,R},{S,S,S},{R,R,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{C6S,C7S,R},{F3S,A3S,C4S},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,D7S,R},{C2S,R,R},{R,R,R},0}, {{R,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,C7S,R},{F3,G3S,C4S},{R,R,R},0}, {{B5,B6,R},{R,R,R},{R,R,R},0}, {{A5S,A6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{D2S,A2S,D3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{D3S,R,R},{R,R,R},0},
        {{C6S,R,R},{F3S,A3S,C4S},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{F3S,A3S,C4S},{R,R,R},0},
        {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F1S,F2S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,A6S,R},{F3S,A3S,C4S},{R,R,R},0}, {{R,R,R},{F3S,R,R},{R,R,R},0}, {{C6S,C7S,R},{F2S,R,R},{R,R,R},0}, {{A5S,A6S,R},{R,R,R},{R,R,R},0},

        {{G5S,G6S,R},{B1,F2S,B2},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{D5S,D6S,R},{F3S,A3S,R},{R,R,R},0}, {{F5S,F6S,R},{S,S,S},{R,R,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{G5S,G6S,R},{F3S,A3S,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C2S,R,R},{R,R,R},0}, {{R,R,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,A6S,R},{C3S,R,R},{R,R,R},0}, {{R,R,R},{F3,G3S,C4S},{R,R,R},0}, {{C6S,C7S,R},{R,R,R},{R,R,R},0}, {{A5S,A6S,R},{R,R,R},{R,R,R},0},


        {{G5S,G6S,R},{D2S,A2S,D3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{C5S,C6S,R},{F3S,A3S,R},{R,R,R},0}, {{F5S,F6S,R},{S,S,S},{R,R,R},0}, {{S,S,S},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{F3S,A3S,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{A1S,R,R},{R,R,R},0}, {{R,R,R},{F3S,A3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,F6S,R},{S,S,S},{R,R,R},0}, {{F5,F6,R},{F3S,A3S,C4S},{R,R,R},0}, {{D5S,D6S,R},{R,R,R},{R,R,R},0}, {{F5,F6,R},{R,R,R},{R,R,R},0},

        {{D5S,D6S,R},{B1,F2S,B2},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B2,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{G5S,G6S,R},{F3S,A3S,R},{R,R,R},0}, {{R,R,R},{S,S,S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F3S,A3S,R},{R,R,R},0},
        {{F5,F6,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C2S,R,R},{R,R,R},0}, {{D5S,D6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5,F6,R},{F3,G3S,C4S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5,C6S,F6},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{B1,B2,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,F6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F2S,R,R},{R,R,R},0},
        {{F5S,C6S,F6S},{C2S,C3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{G2S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{F5S,A5S,R},{D2S,A2S,D3S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{D2S,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0},
        {{F3S,A3S,C4S},{D2S,R,R},{R,R,R},0}, {{R,R,R},{D3S,R,R},{R,R,R},0}, {{R,R,R},{A2S,R,R},{R,R,R},0}, {{R,R,R},{D2S,R,R},{R,R,R},0},

        {{S,S,S},{B1,F2S,B2},{S,S,S},_V[9]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{C4S,F4S,C5S},{R,R,R},0}, {{A5S,R,R},{G4S,R,R},{R,R,R},0}, {{G5S,R,R},{F4S,R,R},{R,R,R},0}, {{F5S,R,R},{G4S,R,R},{R,R,R},0},
        {{C6S,R,R},{G4S,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{B2,F3S,C4S},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A3S,R,R},{R,R,R},0}, {{A5S,R,R},{B2,G3S,R},{R,R,R},0}, {{G5S,R,R},{F3S,R,R},{R,R,R},0}, {{F5S,R,R},{G3S,R,R},{R,R,R},0},



        {{S,S,S},{B1,F2S,B2},{G3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{S,S,S},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{C4S,F4S,C5S},{R,R,R},0}, {{A5S,R,R},{G4S,R,R},{R,R,R},0}, {{G5S,R,R},{F4S,R,R},{R,R,R},0}, {{F5S,R,R},{G4S,R,R},{R,R,R},0},
        {{C6S,R,R},{G4S,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{B2,F3S,C4S},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A3S,R,R},{R,R,R},0}, {{A5S,R,R},{B2,G3S,R},{R,R,R},0}, {{G5S,R,R},{F3S,R,R},{R,R,R},0}, {{F5S,R,R},{G3S,R,R},{R,R,R},0},

        {{S,S,S},{C2S,G2S,C3S},{G3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{S,S,S},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{C4S,F4,A4S},{R,R,R},0}, {{B5,R,R},{G4S,R,R},{R,R,R},0}, {{A5S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{G4S,R,R},{R,R,R},0},
        {{C6S,R,R},{G4S,R,R},{R,R,R},0}, {{B5,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{C3S,F3,C4S},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A3S,R,R},{R,R,R},0}, {{B5,R,R},{C3S,G3S,R},{R,R,R},0}, {{A5S,R,R},{F3S,R,R},{R,R,R},0}, {{G5S,R,R},{G3S,R,R},{R,R,R},0},

        {{S,S,S},{C2S,G2S,C3S},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{D5S,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{F4,A4S,C5S},{R,R,R},0}, {{B5,R,R},{A4S,R,R},{R,R,R},0}, {{A5S,R,R},{A4S,R,R},{R,R,R},0}, {{G5S,R,R},{C5S,R,R},{R,R,R},0},
        {{C6S,R,R},{C5S,R,R},{R,R,R},0}, {{B5,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{C3S,F3,D4S},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{C4S,R,R},{R,R,R},0}, {{B5,R,R},{C3S,A3S,R},{R,R,R},0}, {{A5S,R,R},{C3S,A3S,R},{R,R,R},0}, {{G5S,R,R},{C3S,C4S,R},{R,R,R},0},

        {{S,S,S},{D2S,A2S,C3S},{C4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{S,S,S},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{D4S,F4S,A4S},{R,R,R},0}, {{B5,R,R},{F4S,R,R},{R,R,R},0}, {{A5S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{G4S,R,R},{R,R,R},0},
        {{C6S,R,R},{G4S,R,R},{R,R,R},0}, {{B5,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{C3S,F3S,C4S},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{A3S,R,R},{R,R,R},0}, {{B5,R,R},{C3S,F3S,R},{R,R,R},0}, {{A5S,R,R},{C3S,F3S,R},{R,R,R},0}, {{G5S,R,R},{C3S,G3S,R},{R,R,R},0},


        {{S,S,S},{D2S,A2S,C3S},{G3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{S,S,S},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{D6S,R,R},{D4S,F4S,A4S},{R,R,R},0}, {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{B5,R,R},{F4S,R,R},{R,R,R},0}, {{A5S,R,R},{G4S,R,R},{R,R,R},0},
        {{D6S,R,R},{G4S,R,R},{R,R,R},0}, {{C6S,R,R},{R,R,R},{R,R,R},0}, {{B5,R,R},{C3S,F3S,C4S},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0},
        {{D6S,R,R},{A3S,R,R},{R,R,R},0}, {{C6S,R,R},{C3S,F3S,R},{R,R,R},0}, {{B5,R,R},{C3S,F3S,R},{R,R,R},0}, {{A5S,R,R},{C3S,G3S,R},{R,R,R},0},

        {{S,S,S},{F2,C3S,F3},{G3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{S,S,S},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{F6,R,R},{C4S,F4,A4S},{R,R,R},0}, {{D6S,R,R},{F4S,R,R},{R,R,R},0}, {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{G4S,R,R},{R,R,R},0},
        {{F6,R,R},{G4S,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{F3S,F4S,R},{R,R,R},0}, {{G5S,R,R},{G3S,G4S,R},{R,R,R},0},
        {{F6,R,R},{A3S,A4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{B3,B4,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{C2S,G2S,C3S},{S,S,S},_V[10]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,F5,C6S},{G3S,G4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C5S,F5,C6S},{G3S,G4S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,F5,C6S},{C2S,C3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C5S,F5,C6S},{C2S,C3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,F5,C6S},{C2S,C3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C5S,F5,C6S},{C2S,C3S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},




        //Part 3

        {{G5S,R,R},{S,S,S},{B3,F4S,A4S},_V[5]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{C4S,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{R,R,R},{D4S,A4S,C5S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{F3S,C4S,A4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{R,R,R},{B3,F4S,A4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{C4S,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{R,R,R},{D4S,A4S,C5S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{A3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{C4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},


        {{G5S,R,R},{D4S,R,R},{B2,F3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{C4S,R,R},{C3S,F3,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{B3,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{A3S,R,R},{D3S,F3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{F4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{F4S,R,R},{F3S,A3S,D4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{D4S,R,R},{B2,F3S,R},_V[6]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{C4S,R,R},{C3S,F3,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{G3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{R,R,R},{D3S,F3S,A3S},_V[7]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{S,S,S},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A4S,R,R},{A3S,R,R},{F3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},



        {{S,S,S},{D4S,R,R},{B2,F3S,R},_V[8]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,C6S,R},{F4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,D6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,C6S,R},{C4S,R,R},{C3S,F3,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{B3,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{A3S,R,R},{D3S,F3S,R},_V[9]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,C6S,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{F4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,D6S,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,C6S,R},{F4S,R,R},{F3S,A3S,D4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{D4S,R,R},{B2,F3S,R},_V[10]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,C6S,R},{F4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,D6S,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{C4S,R,R},{C3S,F3,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,F6,R},{R,R,R},{G3S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{C6S,G6S,R},{S,S,S},{C2S,C3S,R},_V[11]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,F6S,R},{R,R,R},{C2S,C3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,F6,R},{R,R,R},{C2S,C3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{S,S,S},{A4S,R,R},{S,S,S},0}, {{R,R,R},{S,S,S},{R,R,R},0}, {{R,R,R},{C5S,R,R},{R,R,R},0}, {{R,R,R},{A4S,R,R},{R,R,R},0},


        {{S,S,S},{G4S,R,R},{B1,B2,R},_V[12]}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{A5S,R,R},{D4S,R,R},{F3S,B3,R},0}, {{F5S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{G4S,R,R},{R,R,R},0},
        {{F5S,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{C2S,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{A4S,R,R},{G3S,C4S,F4},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{R,R,R},0}, {{R,R,R},{A4S,R,R},{R,R,R},0},

        {{S,S,S},{G4S,R,R},{D2S,D3S,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{A5S,R,R},{C4S,R,R},{F3S,A3S,R},0}, {{F5S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{F4S,R,R},{R,R,R},0},
        {{F5S,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{A1S,A2S,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{C5S,F5S,R},{F4S,R,R},{A4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{A4S,R,R},{R,R,R},{S,S,S},0},

        {{S,S,S},{G4S,R,R},{B1,B2,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{A5S,R,R},{F4S,A4S,R},{S,S,S},0}, {{F5S,R,R},{F4S,C5S,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{C5S,R,R},{F4S,R,R},0},
        {{A5S,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{D5S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{C2S,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0},
        {{C5S,R,R},{C5S,R,R},{C4S,F4,G4S},0}, {{B4,R,R},{B4,R,R},{R,R,R},0}, {{A4S,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{S,S,S},{D2S,D3S,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{R,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{A5S,R,R},{R,R,R},{D3S,F3S,A3S},0}, {{C6S,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{F6S,R,R},{R,R,R},{D3S,F3S,A3S},0},
        {{F6,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{F1S,F2S,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0},
        {{F4S,C6S,R},{R,R,R},{A4S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{R,R,R},0}, {{R,R,R},{A4S,R,R},{S,S,S},0},



        {{S,S,S},{G4S,R,R},{B1,B2,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{A5S,R,R},{D4S,R,R},{F3S,B3,R},0}, {{F5S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{G4S,R,R},{R,R,R},0},
        {{F5S,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{C2S,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{A4S,R,R},{G3S,C4S,F4},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C5S,R,R},{R,R,R},0}, {{R,R,R},{A4S,R,R},{R,R,R},0},

        {{S,S,S},{G4S,R,R},{D2S,D3S,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{C6S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0},
        {{A5S,R,R},{C4S,R,R},{F3S,A3S,R},0}, {{F5S,R,R},{F4S,R,R},{R,R,R},0}, {{G5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{F4S,R,R},{R,R,R},0},
        {{F5S,R,R},{R,R,R},{R,R,R},0}, {{C5S,R,R},{R,R,R},{R,R,R},0}, {{D5S,R,R},{R,R,R},{A1S,A2S,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0},
        {{S,S,S},{F4S,R,R},{A3S,C4S,R},0}, {{R,R,R},{F4,R,R},{R,R,R},0}, {{R,R,R},{D4S,R,R},{R,R,R},0}, {{R,R,R},{F4,R,R},{R,R,R},0},

        {{S,S,S},{D4S,R,R},{B1,B2,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{R,R,R},{G4S,G5S,R},{F3S,A3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{F4,F5,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,R,R},0}, {{R,R,R},{D4S,D5S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F4,F5,R},{F3,G3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F4,C5S,F5},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{R,R,R},{B1,B2,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F4S,F5S,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F4S,G4S,F5S},{C2S,C3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{G2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C4S,F4S,A4S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{C4S,R,R},{R,R,R},0}, {{R,R,R},{G4S,R,R},{R,R,R},0},
        {{R,R,R},{C5S,R,R},{F3S,A3S,R},0}, {{R,R,R},{G5S,R,R},{D3S,R,R},0}, {{R,R,R},{C6S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{D2S,R,R},0},


        {{D6S,R,R},{D5S,F5S,A5S},{B1,F2S,B2},_V[13]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{S,S,S},{C5S,C6S,R},{F3S,A3S,R},0}, {{R,R,R},{D5S,D6S,R},{R,R,R},0}, {{R,R,R},{F5S,F6S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{G5S,G6S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,G2S,C3S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{A5S,A6S,R},{G3S,C4S,R},0}, {{R,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{F6S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{F5S,A5S,F6S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,R,R},0},
        {{R,R,R},{A5S,A6S,R},{A3S,C4S,R},0}, {{R,R,R},{G5S,G6S,R},{R,R,R},0}, {{R,R,R},{F5S,F6S,R},{F3S,R,R},0}, {{R,R,R},{R,R,R},{A3S,C4S,R},0},
        {{R,R,R},{G5S,G6S,R},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{A1S,A2S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C6S,C7S,R},{F3S,A3S,R},0}, {{R,R,R},{R,R,R},{A2S,R,R},0}, {{R,R,R},{A6S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{A6S,R,R},{A5S,C6S,F6S},{B1,F2S,B2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{S,S,S},{F5S,F6S,R},{F3S,A3S,R},0}, {{R,R,R},{F5,F6,R},{R,R,R},0}, {{R,R,R},{C5S,C6S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{D5S,D6S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,G2S,C3S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C5S,C6S,R},{G3S,C4S,R},0}, {{R,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{A5S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{C6S,R,R},{C5S,F5S,G5S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{S,S,S},{F4S,R,R},{D3S,R,R},0}, {{R,R,R},{G4S,R,R},{F3S,R,R},0},
        {{R,R,R},{A4S,R,R},{A3S,C4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{G4S,R,R},{F3S,R,R},0}, {{R,R,R},{A4S,R,R},{A3S,C4S,R},0},
        {{R,R,R},{B4,R,R},{S,S,S},0}, {{R,R,R},{G4S,R,R},{R,R,R},0}, {{R,R,R},{A4S,R,R},{F1S,F2S,R},0}, {{R,R,R},{B4,R,R},{R,R,R},0},
        {{R,R,R},{C5S,R,R},{A3S,C4S,R},0}, {{R,R,R},{D5S,R,R},{F3S,R,R},0}, {{R,R,R},{A4S,R,R},{F2S,R,R},0}, {{R,R,R},{G4S,R,R},{R,R,R},0},



        {{D6S,R,R},{D5S,F5S,A5S},{B1,F2S,B2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{S,S,S},{C5S,C6S,R},{F3S,A3S,R},0}, {{R,R,R},{D5S,D6S,R},{R,R,R},0}, {{R,R,R},{F5S,F6S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{G5S,G6S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,G2S,C3S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{A5S,A6S,R},{G3S,C4S,R},0}, {{R,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{F6S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{F5S,A5S,F6S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,R,R},0},
        {{R,R,R},{A5S,A6S,R},{A3S,C4S,R},0}, {{R,R,R},{G5S,G6S,R},{R,R,R},0}, {{R,R,R},{F5S,F6S,R},{F3S,R,R},0}, {{R,R,R},{R,R,R},{A3S,C4S,R},0},
        {{R,R,R},{G5S,G6S,R},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{A1S,A2S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C6S,C7S,R},{F3S,A3S,R},0}, {{R,R,R},{R,R,R},{A2S,R,R},0}, {{R,R,R},{D6S,R,R},{R,R,R},0}, {{R,R,R},{F6S,R,R},{R,R,R},0},

        {{D7S,R,R},{D6S,F6S,A6S},{B1,F2S,B2},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{R,R,R},{R,R,R},{F3S,A3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{S,S,S},{A5S,A6S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{G5S,F6,G6S},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,G2S,C3S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{G3S,C4S,R},0}, {{R,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{F5S,F6S,R},{R,R,R},0}, {{R,R,R},{G5S,G6S,R},{R,R,R},0},

        {{A6S,R,R},{A5S,D6S,F6S},{B1,B2,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{F2S,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{R,R,R},{C2S,C3S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{S,S,S},{G5S,G6S,R},{G2S,R,R},0}, {{R,R,R},{F5S,F6S,R},{R,R,R},0},
        {{F6S,R,R},{F5S,G5S,D6S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{S,S,S},{F4S,R,R},{R,R,R},0}, {{R,R,R},{G4S,R,R},{R,R,R},0},
        {{R,R,R},{A4S,R,R},{D3S,R,R},0}, {{R,R,R},{G4S,R,R},{F3S,R,R},0}, {{R,R,R},{A4S,R,R},{A3S,R,R},0}, {{R,R,R},{C5S,R,R},{C4S,R,R},0},


        {{S,S,S},{F4S,A4S,D5S},{B3,R,R},_V[10]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C5S,R,R},{B3,R,R},0}, {{R,R,R},{D5S,R,R},{C4S,R,R},0}, {{R,R,R},{F5S,R,R},{C4S,R,R},0}, {{R,R,R},{S,S,S},{S,S,S},0},
        {{R,R,R},{G5S,R,R},{F4,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C5S,F5,A5S},{F3,G3S,C4S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F5S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{A4S,D5S,F5S},{D3S,F3S,A3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{A5S,R,R},{F4S,R,R},0}, {{R,R,R},{G5S,R,R},{F4,R,R},0}, {{R,R,R},{F5S,R,R},{D4S,R,R},0}, {{R,R,R},{S,S,S},{S,S,S},0},
        {{R,R,R},{G5S,R,R},{F4,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F5,G5S,C6S},{A2S,F4,G3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{A5S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{F5S,A5S,R},{B2,F3S,A3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{F5S,R,R},{D4S,R,R},0}, {{R,R,R},{F5,R,R},{C4S,R,R},0}, {{R,R,R},{C5S,R,R},{A3S,R,R},0}, {{R,R,R},{S,S,S},{S,S,S},0},
        {{R,R,R},{D5S,R,R},{B3,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C4S,R,R},{F4,C5S,R},{C3S,F3,G3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{A4S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{S,S,S},{D4S,A4S,C5S},{D3S,F3S,A3S},_V[11]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{F5S,R,R},{R,R,R},0}, {{R,R,R},{G5S,R,R},{R,R,R},0},
        {{R,R,R},{A5S,R,R},{F4S,A4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{G5S,R,R},{R,R,R},0}, {{R,R,R},{A5S,R,R},{F4S,A4S,R},0},
        {{R,R,R},{B5,R,R},{R,R,R},0}, {{R,R,R},{G5S,R,R},{R,R,R},0}, {{R,R,R},{A5S,R,R},{F2S,F3S,R},0}, {{R,R,R},{B5,R,R},{R,R,R},0},
        {{R,R,R},{C6S,R,R},{A3S,C4S,R},0}, {{R,R,R},{D6S,R,R},{F3S,R,R},0}, {{R,R,R},{A5S,R,R},{F2S,R,R},0}, {{R,R,R},{G5S,R,R},{R,R,R},0},



        {{R,R,R},{D4S,F4S,D5S},{B1,F2S,B2},_V[12]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{B2,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0},
        {{R,R,R},{C4S,C5S,R},{F3S,A3S,R},0}, {{R,R,R},{D4S,D5S,R},{R,R,R},0}, {{R,R,R},{F4S,F5S,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,A3S,R},0},
        {{R,R,R},{G4S,G5S,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{C2S,G2S,C3S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{A4S,A5S,R},{G3S,C4S,R},0}, {{R,R,R},{R,R,R},{C3S,R,R},0}, {{R,R,R},{F5S,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{R,R,R},{F4S,A4S,F5S},{D2S,A2S,D3S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{D3S,R,R},0}, {{R,R,R},{R,R,R},{F3S,R,R},0},
        {{R,R,R},{A4S,A5S,R},{A3S,C4S,R},0}, {{R,R,R},{G4S,G5S,R},{R,R,R},0}, {{R,R,R},{F4S,F5S,R},{F3S,R,R},0}, {{R,R,R},{R,R,R},{A3S,A4S,R},0},
        {{R,R,R},{G4S,G5S,R},{S,S,S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{A1S,A2S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{R,R,R},{C5S,C6S,R},{F3S,F4S,R},0}, {{R,R,R},{R,R,R},{A2S,R,R},0}, {{R,R,R},{D5S,R,R},{R,R,R},0}, {{R,R,R},{F5S,R,R},{R,R,R},0},

        {{R,R,R},{D5S,F5S,A5S},{B1,F2S,B2},_V[13]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{D5S,D6S,R},{B2,R,R},0}, {{R,R,R},{D5S,D6S,R},{D3S,R,R},0},
        {{R,R,R},{D5S,D6S,R},{F3S,A3S,R},0}, {{R,R,R},{D5S,D6S,R},{R,R,R},0}, {{R,R,R},{D5S,D6S,R},{D3S,R,R},0}, {{R,R,R},{D5S,D6S,R},{F3S,A3S,R},0},
        {{R,R,R},{F5,F6,R},{R,R,R},0}, {{R,R,R},{F5,F6,R},{R,R,R},0}, {{R,R,R},{F5,F6,R},{C2S,G2S,C3S},0}, {{R,R,R},{F5,F6,R},{R,R,R},0},
        {{R,R,R},{F5,F6,R},{F3,G3S,C4S},0}, {{R,R,R},{F5,F6,R},{C3S,R,R},0}, {{R,R,R},{F5S,F6S,R},{R,R,R},0}, {{R,R,R},{G5S,G6S,R},{R,R,R},0},

        {{R,R,R},{A5S,C6S,A6S},{B1,B2,R},0}, {{R,R,R},{A5S,C6S,A6S},{R,R,R},0}, {{R,R,R},{A5S,C6S,A6S},{F2S,R,R},0}, {{R,R,R},{A5S,C6S,A6S},{R,R,R},0},
        {{R,R,R},{A5S,C6S,A6S},{C2S,C3S,R},0}, {{R,R,R},{A5S,C6S,A6S},{R,R,R},0}, {{R,R,R},{G5S,G6S,R},{G2S,R,R},0}, {{R,R,R},{F5S,F6S,R},{R,R,R},0},
        {{R,R,R},{F5S,F6S,R},{D2S,A2S,D3S},0}, {{R,R,R},{F5S,A5S,F6S},{R,R,R},0}, {{R,R,R},{F5S,A5S,F6S},{D3S,R,R},0}, {{R,R,R},{F5S,A5S,F6S},{F3S,R,R},0},
        {{R,R,R},{F5S,A5S,F6S},{A3S,R,R},0}, {{R,R,R},{F5S,A5S,F6S},{C4S,R,R},0}, {{R,R,R},{F5S,A5S,F6S},{D4S,R,R},0}, {{R,R,R},{F5S,A5S,F6S},{F4S,R,R},0},


        {{G5S,R,R},{S,S,S},{B3,F4S,A4S},_V[5]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{C4S,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{R,R,R},{D4S,A4S,C5S},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{D6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{F3S,C4S,A4S},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{D5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{R,R,R},{B3,F4S,A4S},_V[4]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{C4S,G4S,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},

        {{G5S,R,R},{R,R,R},{D4S,A4S,C5S},_V[3]}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},



        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{C6S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{F5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{G5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        {{A5S,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0}, {{R,R,R},{R,R,R},{R,R,R},0},
        };
*/

MediaPlayer::MediaPlayer(QObject *parent)
    : QObject{parent}
{
    init(); // init the map
    t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &MediaPlayer::PlayNext);
    connect(this, &MediaPlayer::StopPlaying, t, &QTimer::stop);
    // Music(); // TODO: Now the music plays only when the MediaPlayer is constructed. We can change it.
}

void MediaPlayer::init () { // private, only used in constructor
    _mp[_REST] = PA ;
    _mp[P] = PA ;
    _mp[_STOP] = PA ;
    _mp[0] = PA ;
    _mp[ _V[1] ] = 0x1f;
    for(int i=2;i<=13;i++) _mp[ _V[i] ] = _mp[ _V[i-1] ] + 8 ;
}

void MediaPlayer::MakeSound(v_spo *note, int channalNum, int additionalStrength) {
    midiOutShortMsg (this->handle, 0x7BB0 + channalNum ) ; //midiOut短消息B类
    if (note->_v1 == _STOP) {
        //与else语句的区别是停止后就不发声,打算用于休止符
        return;
    }
    else {
        int k = note->_volume + additionalStrength;
        if (k < 1) {
            k = 1;
        }
        else if (k > 13) {
            k = 13;
        }
        int volume = _mp[_V[k]];
        //定义声音变量voice并发声
        if (note->_v1 != _REST) {
            int voice = (volume << 16) | ((note->_v1) << 8) | (0x90 + channalNum) ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if (note->_v2 != _REST) {
            int voice = (volume << 16) | ((note->_v2) << 8) | (0x90 + channalNum) ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if (note->_v3 != _REST) {
            int voice = (volume << 16) | ((note->_v3) << 8) | (0x90 + channalNum) ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if (note->_v4 != _REST) {
            int voice = (volume << 16) | ((note->_v4) << 8) | (0x90 + channalNum) ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
    }
}

void MediaPlayer::SetMusic(Music *m) {
    assert (m != nullptr);
    this->song = m;
}

void MediaPlayer::PlayMusic(int start)
{
    if (!this->song) {
        return;
    }
    this->currentPlaying = start;
    midiOutOpen (&handle, 0, 0, 0, CALLBACK_NULL) ; //打开MIDI设备

    for (int i = 0; i < this->song->channal_num; ++i) {
        //midiOut短消息C类 //声明乐器、通道
        midiOutShortMsg (handle, this->song->allChannals[i]->type << 8 | (0xC0 + i));
    }

    this->t->start(1000 * 15 / this->song->speed);
}

void MediaPlayer::PlayNext() {
    if (currentPlaying == this->song->length) {
        emit this->StopPlaying();
        this->currentPlaying = 0;
        midiOutClose (handle) ;
        return;
    }
    for (int i = 0; i < this->song->channal_num; ++i) {
        if (this->song->allChannals[i]->notes[currentPlaying]->_v1 != _REST) {
            this->MakeSound(this->song->allChannals[i]->notes[currentPlaying], i, this->song->allChannals[i]->strength);
        }
    }
    this->currentPlaying += 1;
}

MediaPlayer::~MediaPlayer() {
    delete t;
}

/* old version ...
    //遍历乐谱
    for (auto i : music) {

        //处理指令
        if ( _mp[m.C] != PA ){
            volume = _mp[ m.C ] ; //调节音量
        }

        bool pedal = m.C == P;
        // 处理小节结束
        if ( cnt == 0 ) {
            cnt = 16 ;
            // printf("One Bar Has Passed ...\n") ; // 一小节过去了
        }


        if ( pedal ) { // 处理踏板
            midiOutShortMsg ( handle, 0X7BB0 ) ; // 0X7B是结束指令,B0是通道
            midiOutShortMsg ( handle, 0X7BB1 ) ;
            midiOutShortMsg ( handle, 0X7BB2 ) ;
        }
    }
    midiOutClose (handle) ; //关闭MIDI
}
*/
