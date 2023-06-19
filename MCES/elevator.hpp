#pragma once

#include <queue>

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
    elevator(int maxLoad, int index) :mnMaxLoad(maxLoad), mnCurLoad(0), mnIndex(index),
        mnCurFloor(0), mnLastFloor(0), mnAimFloor(0), mbIsFull(false)
    {
        mnCurState = STATE_STOPING;
    }
    void add_request(request _r)
    {
        mqueRequestQueue.push(_r);
    }
    QString generate_info_display()
    {
        QString _state;
        if (mnCurState == ELEVATOR_UP)
            _state = "↑";
        else
            _state = "↓";
        QString _str = QString::number(mnCurLoad) + "/" + QString::number(mnMaxLoad) + " " + _state + QString::number(mnAimFloor);
        return _str;
    }
    //参数
    int mnMaxLoad;
    int mnIndex;
    int mnCurLoad;
    //状态
    int mnCurFloor;
    int mnLastFloor;
    int mnCurState;
    int mnAimFloor;
    bool mbIsFull;
    std::queue<request> mqueRequestQueue;
};