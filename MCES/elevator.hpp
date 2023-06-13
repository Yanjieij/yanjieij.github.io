#pragma once

#include<queue>

/// 全局变量
/// State：0 for 停止，1 for 运行
#define STATE_MOVING_UP 1
#define STATE_MOVING_DOWN 2
#define STATE_STOPING 0

#define ELEVATOR_UP 1
#define ELEVATOR_DOWN 0

class elevator
{
public:
    elevator(int maxLoad, int index) :mnMaxVolume(maxLoad), mnCurLoad(0), mnIndex(index),
        mnCurFloor(0), mnAimFloor(0)
    {
        mnCurState = STATE_STOPING;
    }
    int getLoad()
    {
        return mnCurLoad;
    }
    int getFloor()
    {
        return mnCurFloor;
    }
private:
    //参数
    int mnMaxVolume;
    int mnIndex;
    int mnCurLoad;
    //状态
    int mnCurFloor;
    int mnCurState;
    int mnAimFloor;
    queue<bool> mqueRequestQueue;
};