#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <Windows.h>
#include <QObject>
#include <iostream>
#include <map>
#include <algorithm>

// #pragma comment(lib,"winmm.lib")


//双结构体记谱
struct v_spo {   // "Voice_SPO"
    //作为 msc结构体 的基本组成单位
    //考虑到使用上的问题,一个通道上仅允许同时演奏三个音是完全可以接受的,再多的话泛用性也不会提高多少,所以只装三个音
    int _v1,_v2,_v3;
};
struct msc {
    //当前的发声过程共有五个参数:通道、乐器、音量、音高、时值
    //按现在这一版的处理方法,通道和时值自动处理,乐器和通道一起处理(而且我打算只用大钢琴)
    //音量难以处理 ( 不是说不行,只是有点麻烦,鉴于它只有锦上添花而不是火中送炭的作用,我决定舍弃它 ( 才不是因为懒和笨！) )
    //所以这里头只需要放音高就行了

    //本曲有三声部
    v_spo v0,v1,v2;
    int C           ; // Command,命令,用于处理一些指令
};


class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(QObject *parent = nullptr);
    std::map <int,int> _mp;
    //预处理
    void init();
    //核心函数
    void MakeSound_0 (v_spo) ;
    void MakeSound_1 (v_spo) ;
    void MakeSound_2 (v_spo) ;
    void Music() ;

signals:
};

#endif // MEDIAPLAYER_H
