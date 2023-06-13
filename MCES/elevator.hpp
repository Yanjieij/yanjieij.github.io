#pragma once

#include<queue>

/// ȫ�ֱ���
/// State��0 for ֹͣ��1 for ����
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
    //����
    int mnMaxVolume;
    int mnIndex;
    int mnCurLoad;
    //״̬
    int mnCurFloor;
    int mnCurState;
    int mnAimFloor;
    queue<bool> mqueRequestQueue;
};