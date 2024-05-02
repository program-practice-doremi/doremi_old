#include "mediaplayer.h"


//全部音高
enum Scale
{
    Rest = 0, //留了位置,但一直没用

    C8 = 108,

    B7 = 107, A7S = 106, A7 = 105, G7S = 104, G7 = 103, F7S = 102, F7 = 101, E7 = 100, D7S = 99, D7 = 98, C7S = 97, C7 = 96,

    B6 = 95,  A6S = 94,  A6 = 93,  G6S = 92,  G6 = 91,  F6S = 90,  F6 = 89,  E6 = 88,  D6S = 87, D6 = 86, C6S = 85, C6 = 84,

    B5 = 83,  A5S = 82,  A5 = 81,  G5S = 80,  G5 = 79,  F5S = 78,  F5 = 77,  E5 = 76,  D5S = 75, D5 = 74, C5S = 73, C5 = 72,

    B4 = 71,  A4S = 70,  A4 = 69,  G4S = 68,  G4 = 67,  F4S = 66,  F4 = 65,  E4 = 64,  D4S = 63, D4 = 62, C4S = 61, C4 = 60,

    B3 = 59,  A3S = 58,  A3 = 57,  G3S = 56,  G3 = 55,  F3S = 54,  F3 = 53,  E3 = 52,  D3S = 51, D3 = 50, C3S = 49, C3 = 48,

    B2 = 47,  A2S = 46,  A2 = 45,  G2S = 44,  G2 = 43,  F2S = 42,  F2 = 41,  E2 = 40,  D2S = 39, D2 = 38, C2S = 37, C2 = 36,

    B1 = 35,  A1S = 34,  A1 = 33,  G1S = 32,  G1 = 31,  F1S = 30,  F1 = 29,  E1 = 28,  D1S = 27, D1 = 26, C1S = 25, C1 = 24,

    B0 = 23,  A0S = 22,  A0 = 21
    //88键,酷！
    //这里后面应该写s的，但是这样不好打谱，所以就用S咯

};

//常用参数
enum Voice
{
    PA = 100000001 , // Prohibition of access , 作为一个空置的参数 , 判断用
    R  = 1009 , // Rest  , 表示声音的延长
    S  = 1013 , // Stop  , 表示休止符
    P  = 1019 , // Pedal , 表示踏板的踩下(或松开)

    //找到的谱子是 96拍/分 C调,选择将速度设为625
    SPEED = 150,  // 625ms/拍,1/4拍为156.25ms
    //考虑到加速，设为150
};

//声音指令，每8一格,最小为0x1f,最大0x7f,共十三档,对应数组_V[1]到_V[13]
const int _V[15]={0,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,0};


//一些全局变量(用于Music函数)
HMIDIOUT handle   ;
int  volume = 0x2F ; // 音量
int  stime  = SPEED  ; // 音时
    // 它们的修改现在都可以用一条指令解决,不弄只是因为懒
int  cnt    = 1     ; // 小节计时器
    // count元素的每一个单位表示四分之一拍,初值表示弱起小节,目前的唯一用途是控制踏板


//上方定义参数
//#######分界线###########################################################
//下方记录乐谱

//乐谱
msc music[] =   //完成进度 100%
    {
        /* 表示小节线 */
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


MediaPlayer::MediaPlayer(QObject *parent)
    : QObject{parent}
{
    init();
    Music(); // TODO: Now the music plays only when the MediaPlayer is constructed. We can change it.
}

void MediaPlayer::init () {
    // TODO: The init methods only inits the map in mediaplayer.
    // Map "_mp" can be shared by every MediaPlayer.
    // Therefore... could the map "_mp" be static?
    _mp[R] = PA ;
    _mp[P] = PA ;
    _mp[S] = PA ;
    _mp[0] = PA ;
    _mp[ _V[1] ] = 0x1f;
    for(int i=2;i<=13;i++) _mp[ _V[i] ] = _mp[ _V[i-1] ] + 8 ;
}

/**
 * TODO: MakeSound_0 & MakeSound_1 & MakeSound_2 have TO MANY repetitions!
 * Can we modify it to some function like "void MakeSound(v_spo _m, int channal)" ?
 */
void MediaPlayer::MakeSound_0 (v_spo _m) {    //第零通道
    //特判由休止符造成的停止发声
    if( _m._v1 == S ){
        //与else语句的区别是停止后就不发声,打算用于休止符
        midiOutShortMsg ( handle, 0x7BB0 ) ; //midiOut短消息B类
    }
    //开始发声
    else {
        //发出新的声音之前先停止上一个声音
        midiOutShortMsg ( handle, 0x7BB0 ) ; //midiOut短消息B类
        //定义声音变量voice并发声
        if ( _m._v1 != R ) {    // "_v1" 当然是不用判断的，但是为了保证队形整齐还是这么写了
            int voice = (volume << 16) | ( (_m._v1) << 8) | 0x90 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if ( _m._v2 != R ) {
            int voice = (volume << 16) | ( (_m._v2) << 8) | 0x90 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if ( _m._v3 != R ) {
            int voice = (volume << 16) | ( (_m._v3) << 8) | 0x90 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
    }
}
void MediaPlayer::MakeSound_1 (v_spo _m) {    //第一通道
    //特判由休止符造成的停止发声
    if( _m._v1 == S ){
        //与else语句的区别是停止后就不发声,打算用于休止符
        midiOutShortMsg ( handle, 0x7BB1 ) ; //midiOut短消息B类
    }
    //开始发声
    else {
        //发出新的声音之前先停止上一个声音
        midiOutShortMsg ( handle, 0x7BB1 ) ; //midiOut短消息B类
        //定义声音变量voice并发声
        if ( _m._v1 != R ) {    // "_v1" 当然是不用判断的，但是为了保证队形整齐还是这么写了
            int voice = (volume << 16) | ( (_m._v1) << 8) | 0x91 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if ( _m._v2 != R ) {
            int voice = (volume << 16) | ( (_m._v2) << 8) | 0x91 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if ( _m._v3 != R ) {
            int voice = (volume << 16) | ( (_m._v3) << 8) | 0x91 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
    }
}
void MediaPlayer::MakeSound_2 (v_spo _m) {    //第二通道
    //特判由休止符造成的停止发声
    if( _m._v1 == S ){
        //与else语句的区别是停止后就不发声,打算用于休止符
        midiOutShortMsg ( handle, 0x7BB2 ) ; //midiOut短消息B类
    }
    //开始发声
    else {
        //发出新的声音之前先停止上一个声音
        midiOutShortMsg ( handle, 0x7BB2 ) ; //midiOut短消息B类
        //定义声音变量voice并发声
        if ( _m._v1 != R ) {    // "_v1" 当然是不用判断的，但是为了保证队形整齐还是这么写了
            int voice = (volume << 16) | ( (_m._v1) << 8) | 0x92 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if ( _m._v2 != R ) {
            int voice = (volume << 16) | ( (_m._v2) << 8) | 0x92 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
        if ( _m._v3 != R ) {
            int voice = (volume << 16) | ( (_m._v3) << 8) | 0x92 ; //音量|音高|通道
            midiOutShortMsg ( handle, voice ) ; //midiOut短消息9类//演奏
        }
    }
}


/**
 * TODO: Now Music can only handle the music listed above.
 * Can we fix that?
 * "void PlayMusic(Music something)"
 */
void MediaPlayer::Music ()
{
    midiOutOpen (&handle, 0, 0, 0, CALLBACK_NULL) ; //打开MIDI设备

    midiOutShortMsg (handle, 0 << 8 | 0xC0) ; //midiOut短消息C类 //声明乐器(0:大钢琴)、通道0(右手一声部)
    midiOutShortMsg (handle, 0 << 8 | 0xC1) ; //midiOut短消息C类 //声明乐器(0:大钢琴)、通道1(右手二声部)
    midiOutShortMsg (handle, 0 << 8 | 0xC2) ; //midiOut短消息C类 //声明乐器(0:大钢琴)、通道2(左手一声部)
    //遍历乐谱
    for (auto i : music) {

        msc m=i;

        cnt -- ; // 四分之一小节过去了

        //处理指令
        if ( _mp[m.C] != PA ){
            volume = _mp[ m.C ] ; //调节音量
        }
        //分通道发声
        //因为同一通道里的音同时开始和停止,所以只判断 "_v1" 的值即可
        if ( m.v0._v1 != R ) MakeSound_0 ( m.v0 ) ;
        if ( m.v1._v1 != R ) MakeSound_1 ( m.v1 ) ;
        if ( m.v2._v1 != R ) MakeSound_2 ( m.v2 ) ;
        //保持音时
        Sleep ( stime ) ; // 四分之一小节
        bool pedal = m.C == P;
        // 处理小节结束
        if ( cnt == 0 ) {
            cnt = 16 ;
            // printf("One Bar Has Passed ...\n") ; // 一小节过去了
        }

        /**
         * BUG: Pedal now has some bug.
         */
        if ( pedal ) { // 处理踏板
            midiOutShortMsg ( handle, 0X7BB0 ) ; // 0X7B是结束指令,B0是通道
            midiOutShortMsg ( handle, 0X7BB1 ) ;
            midiOutShortMsg ( handle, 0X7BB2 ) ;
        }
    }
    midiOutClose (handle) ; //关闭MIDI
}

