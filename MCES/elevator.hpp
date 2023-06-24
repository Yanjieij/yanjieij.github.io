#pragma once

#include <queue>
#include "request.hpp"
#include <vector>

/// 全局变量
#define STATE_GO_PICK 1
#define STATE_GO_DEST 2
#define STATE_STOPING 0

#define MOVING_UP 1
#define MOVING_DOWN 0
#define MOVING_WAIT 2

#define POLICY_EASY 4
#define POLICY_BALANCED 0
#define POLICY_UPPEAK 1
#define POLICY_DOWNPEAK 2
#define POLICY_TWOWAY 3

class elevator
{
public:
    elevator(int maxLoad, int index) :mnMaxLoad(maxLoad), mnCurLoad(0), mnIndex(index),
        mnCurFloor(0), mnLastFloor(0), mnTargetFloor(0), mnCurPolicy(POLICY_EASY), mnGoDestDuration(0),
        mnGoPickDuration(0)
    {
        mnCurMovingState = MOVING_WAIT;
        mnCurPickingState = STATE_STOPING;
    }

    bool easy_access_request(request _r)
    {
        if ((_r.passengerNum + mnCurLoad) > mnMaxLoad)
            return 0;
        else
            return 1;
    }

    void add_request(request _r)
    {
        mqueRequestQueue.push(_r);
    }

    void execute_moving()
    {
        switch (mnCurMovingState)
        {
        case MOVING_UP:
            mnLastFloor = mnCurFloor;
            mnCurFloor++;
            break;
        case MOVING_DOWN:
            mnLastFloor = mnCurFloor;
            mnCurFloor--;
            break;
        case MOVING_WAIT:
            break;
        default:
            break;
        }
        if (mnCurPolicy == POLICY_EASY)
            easy_refresh_request();
        else
            real_refresh_request();
    }

    void easy_refresh_request()
    {
        if (mnCurPickingState == STATE_STOPING || mnCurMovingState == MOVING_WAIT)
        {
            if (!mqueRequestQueue.empty())
            {
                mnCurPickingState = STATE_GO_PICK;
                msCurRequest = mqueRequestQueue.front();
                mnTargetFloor = msCurRequest.curFloor;
                mnCurMovingState = ((mnTargetFloor - mnCurFloor) > 0 ? MOVING_UP : MOVING_DOWN);
            }
        }
        switch (mnCurPickingState)
        {
        case STATE_GO_DEST:
            if (mnCurFloor == msCurRequest.targetFloor) //已经到达当前请求的目标楼层
            {
                mnCurPickingState = STATE_GO_PICK;
                mqueRequestQueue.pop();
                mnCurLoad -= msCurRequest.passengerNum;
                if (!mqueRequestQueue.empty())
                {
                    msCurRequest = mqueRequestQueue.front();
                    mnTargetFloor = msCurRequest.curFloor;
                    if (mnTargetFloor == mnCurFloor)
                    {
                       mnCurPickingState = STATE_GO_DEST;
                       mnTargetFloor = msCurRequest.targetFloor;
                       mnCurLoad += msCurRequest.passengerNum;
                    }
                    mnCurMovingState = ((mnTargetFloor - mnCurFloor) > 0 ? MOVING_UP : MOVING_DOWN);
                }
                else 
                {
                    mnCurPickingState = STATE_STOPING;
                    mnCurMovingState = MOVING_WAIT;
                }
            }
            break;
        case STATE_GO_PICK:
            if (mnCurFloor == msCurRequest.curFloor) //已到达乘客所在楼层
            {
                mnCurPickingState = STATE_GO_DEST;
                mnTargetFloor = msCurRequest.targetFloor;
                mnCurLoad += msCurRequest.passengerNum;
                mnCurMovingState = ((mnTargetFloor - mnCurFloor) > 0 ? MOVING_UP : MOVING_DOWN);
            }
            break;
        default:
            break;
        }
    }

    void real_refresh_request()
    {

    }

    void refresh_static_info()
    {
        if (mnCurPickingState == STATE_GO_DEST)
            mnGoDestDuration++;
        else if (mnCurPickingState == STATE_GO_PICK)
            mnGoPickDuration++;
    }

    QString generate_info_display()
    {
        QString _state;
        if (mnCurMovingState == MOVING_UP)
            _state = "UP";
        else if (mnCurMovingState == MOVING_DOWN)
            _state = "DOWN";
        else
            _state = "WAIT";
        QString _str = QString::number(mnCurFloor) + "F " + QString::number(mnCurLoad) + "/" + QString::number(mnMaxLoad) + " " + _state + " TO " + QString::number(mnTargetFloor) + "F";
        return _str;
    }
    //参数
    int mnMaxLoad;
    int mnIndex;
    int mnCurLoad;
    int mnCurPolicy;
    //状态
    int mnCurFloor;
    int mnLastFloor;
    int mnCurMovingState;
    int mnCurPickingState;
    //统计
    int mnGoDestDuration;
    int mnGoPickDuration;

    std::vector<int> mvecTargetFloor;
    int mnTargetFloor;

    request msCurRequest;
    std::queue<request> mqueRequestQueue;
};