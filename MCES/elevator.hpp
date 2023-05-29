#pragma once

/// ȫ�ֱ���
/// State��0 for ֹͣ��1 for ����
#define STATE_MOVING 1
#define STATE_STOPING 0

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
    //info
    int mnMaxVolume;
    int mnIndex;
    int mnCurLoad;
    //status
    int mnCurFloor;
    int mnCurState;
    int mnAimFloor;
};