#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

/**
 * General MIDI Level 1 旋律乐器对照表
 * @note: 只向某个MIDI通道发送Program Change命令设置乐器，是不能发音的，
 * 想要发音，还必须发出开音符指令；有些乐器开始播放后，如果不发关音
 * 符指令，会持续发音很长时间
 */
//Piano 1-8 键盘乐器 define = MIDI GM1列表序号 - 1
#define Acoustic_Grand_Piano 0 //平台钢琴
#define Bright_Acoustic_Piano 1 //亮音钢琴
#define Electric_Grand_Piano 2 //电钢琴
#define Honky_tonk_Piano 3 //酒吧钢琴
#define Electric_Piano_1 4 //电钢琴1
#define Electric_Piano_2 5 //电钢琴2
#define Harpsichord 6 //羽管键琴
#define Clavi 7 //电翼琴
//Chromatic Percussion 9-16 固定音高敲击乐器
#define Celesta 8 //钢片琴
#define Glockenspiel 9 //钟琴
#define Music_Box 10 //音乐盒
#define Vibraphone 11 //颤音琴
#define Marimba 12 //马林巴琴
#define Xylophone 13 //木琴
#define Tubular_Bells 14 //管钟
#define Dulcimer 15 //洋琴
//Organ 17-24 风琴
#define Drawbar_Organ 16 //音栓风琴
#define Percussive_Organ 17 //敲击风琴
#define Rock_Organ 18 //摇滚风琴
#define Church_Organ 19 //教堂管风琴
#define Reed_Organ 20 //簧风琴
#define Accordion 21 //手风琴
#define Harmonica 22 //口琴
#define Tango_Accordion 23 //探戈手风琴
//Guitar 25-32 吉他
#define Acoustic_Guitar_nylon 24 //木吉他 尼龙弦
#define Acoustic_Guitar_steel 25 //木吉他 钢弦
#define Electric_Guitar_jazz 26 //电吉他 爵士
#define Electric_Guitar_clean 27 //电吉他 原音
#define Electric_Guitar_muted 28 //电吉他 闷音
#define Overdriven_Guitar 29 //电吉他 破音
#define Distortion_Guitar 30 //电吉他 失真
#define Guitar_harmonics 31 //吉他泛音
//Bass 33-40 贝斯
#define Acoustic_Bass 32 //民谣贝司
#define Electric_Bass_finger 33 //电贝司 指奏
#define Electric_Bass_pick 34 //电贝司 拨奏
#define Fretless_Bass 35 //无格贝斯
#define Slap_Bass_1 36 //捶钩贝斯1
#define Slap_Bass_2 37 //捶钩贝斯2
#define Synth_Bass_1 38 //合成贝司1
#define Synth_Bass_2 39 //合成贝司2
//Strings 41-48 弦乐器
#define Violin 40 //小提琴
#define Viola 41 //中提琴
#define Cello 42 //大提琴
#define Contrabass 43 //低音大提琴
#define Tremolo_Strings 44 //颤弓弦乐
#define Pizzicato_Strings 45 //弹拨弦乐
#define Orchestral_Harp 46 //竖琴
#define Timpani 47 //定音鼓
//Ensemble 49-56 合奏
#define String_Ensemble_1 48 //弦乐合奏1
#define String_Ensemble_2 49 //弦乐合奏2
#define SynthStrings_1 50 //合成弦乐1
#define SynthStrings_2 51 //合成弦乐2
#define Choir_Aahs 52 //人声“啊”
#define Voice_Oohs 53 //人声“喔”
#define Synth_Voice 54 //合成人声
#define Orchestra_Hit 55 //交响打击乐
//Brass 57-64 铜管乐器
#define Trumpet 56 //小号
#define Trombone 57 //长号
#define Tuba 58 //大号
#define Muted_Trumpet 59 //闷音小号
#define French_Horn 60 //圆号/法国号
#define Brass_Section 61 //铜管乐
#define SynthBrass_1 62 //合成铜管1
#define SynthBrass_2 63 //合成铜管2
// Reed 65-72 簧乐器
#define Soprano_Sax 64 //高音萨克斯风
#define Alto_Sax 65 //中音萨克斯风
#define Tenor_Sax 66 //次中音萨克斯风
#define Baritone_Sax 67 //上低音萨克斯风
#define Oboe 68 //双簧管
#define English_Horn 69 //英国管
#define Bassoon 70 //低音管
#define Clarinet 71 //单簧管
//Pipe 73-80 吹管乐器
#define Piccolo 72 //短笛
#define Flute 73 //长笛
#define Recorder 74 //直笛
#define Pan_Flute 75 //排笛
#define Blown_Bottle 76 //瓶笛
#define Shakuhachi 77 //尺八
#define Whistle 78 //哨子
#define Ocarina 79 //陶笛
//Synth Lead 81-88 合成音主旋律
#define Lead_1_square 80 //方波
#define Lead_2_sawtooth 81 //锯齿波
#define Lead_3_calliope 82 //汽笛风琴
#define Lead_4_chiff 83 //合成吹管
#define Lead_5_charang 84 //合成电吉他
#define Lead_6_voice 85 //人生键盘
#define Lead_7_fifths 86 //五度音
#define Lead_8_bass_lead 87 //贝司吉他合奏
//Synth Pad 89-96 合成音和弦衬底
#define Pad_1_new age 88 //新世纪
#define Pad_2_warm 89 //温暖
#define Pad_3_polysynth 90 //多重和音
#define Pad_4_choir 91 //人生合唱
#define Pad_5_bowed 92 //玻璃
#define Pad_6_metallic 93 //金属
#define Pad_7_halo 94 //光华
#define Pad_8_sweep 95 //扫掠
//Synth Effects 97-104 合成音效果
#define FX_1_rain 96 //雨
#define FX_2_soundtrack 97 //电影音效
#define FX_3_crystal 98 //水晶
#define FX_4_atmosphere 99 //气氛
#define FX_5_brightness 100 //明亮
#define FX_6_goblins 101 //魅影
#define FX_7_echoes 102 //回音
#define FX_8_sci_fi 103 //科幻
//Ethnic 105-112 民族乐器
#define Sitar 104 //西塔琴
#define Banjo 105 //五弦琴/斑鸠琴
#define Shamisen 106 //三味线
#define Koto 107 //十三弦琴/古筝
#define Kalimba 108 //卡林巴铁片琴
#define Bag_pipe 109 //苏格兰风琴
#define Fiddle 110 //古提琴
#define Shanai 111 //獸笛,类似唢呐
//Percussive 113-120 打击乐器
#define Tinkle_Bell 112 //叮铛铃
#define Agogo 113 //阿哥哥鼓
#define Steel_Drums 114 //钢鼓
#define Woodblock 115 //木鱼
#define Taiko_Drum 116 //太鼓
#define Melodic_Tom 117 //定音铜鼓
#define Synth_Drum 118 //合成股
#define Reverse_Cymbal 119 //逆转钹声
//Sound Effects 121-128 特殊音效
#define Guitar_Fret_Noise 120 //吉他滑弦杂声
#define Breath_Noise 121 //呼吸杂声
#define Seashore 122 //海岸
#define Bird_Tweet 123 //鸟鸣
#define Telephone_Ring 124 //电话铃声
#define Helicopter 125 //直升机
#define Applause 126 //拍手
#define Gunshot 127 //枪声

#endif // INSTRUMENTS_H
