#pragma once

#include <queue>
#include <stdlib.h>
#include "request.hpp"
#include <vector>

/// 全局变量
#define PICKING_GO_PICK 1
#define PICKING_GO_DEST 2
#define PICKING_STOPING 0

#define MOVING_UP 1
#define MOVING_DOWN 0
#define MOVING_WAIT 2
#define MOVING_ON 3

#define POLICY_EASY 4
#define POLICY_BALANCED 0
#define POLICY_UPPEAK 1
#define POLICY_DOWNPEAK 2
#define POLICY_TWOWAY 3

class elevator
{
public:
    elevator(int maxLoad, int index) :mnMaxLoad(maxLoad), mnCurLoad(0), mnIndex(index),
        mnCurFloor(0), mnLastFloor(-1), mnTargetFloor(0), mnCurPolicy(POLICY_EASY), mnGoDestDuration(0),
        mnGoPickDuration(0)
    {
        mnMaxFloor = 0;
        mbIsFull = false;
        mnCurMovingState = MOVING_WAIT;
        mnCurPickingState = PICKING_STOPING;
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
        switch (mnCurPolicy)
        {
        case POLICY_EASY:
            mqueRequestQueue.push(_r);
            break;
        case POLICY_DOWNPEAK:
        case POLICY_UPPEAK:
        case POLICY_BALANCED:
        case POLICY_TWOWAY:
            mvecUnpickedRequestVector.push_back(_r);
            break;
        default:
            break;
        }
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
       
        switch (mnCurPolicy)
        {
        case POLICY_EASY:
        {
            easy_refresh_request();
            break;
        }
        case POLICY_BALANCED:
        {
            balanced_refresh_request();
            break;
        }
        case POLICY_UPPEAK:
        {
            uppeak_refresh_request();
            break;
        }
        case POLICY_DOWNPEAK:
        {
            downpeak_refresh_request();
            break;
        }
        case POLICY_TWOWAY:
        {
            twoway_refresh_request();
            break;
        }
        default:
            break;
        }
    }

    void easy_refresh_request()
    {
        if (mnCurPickingState == PICKING_STOPING || mnCurMovingState == MOVING_WAIT)
        {
            if (!mqueRequestQueue.empty())
            {
                mnCurPickingState = PICKING_GO_PICK;
                msCurRequest = mqueRequestQueue.front();
                mnTargetFloor = msCurRequest.curFloor;
                mnCurMovingState = ((mnTargetFloor - mnCurFloor) > 0 ? MOVING_UP : MOVING_DOWN);
            }
        }
        switch (mnCurPickingState)
        {
        case PICKING_GO_DEST:
            if (mnCurFloor == msCurRequest.targetFloor) //已经到达当前请求的目标楼层
            {
                mnCurPickingState = PICKING_GO_PICK;
                mqueRequestQueue.pop();
                mnCurLoad -= msCurRequest.passengerNum;
                if (!mqueRequestQueue.empty())
                {
                    msCurRequest = mqueRequestQueue.front();
                    mnTargetFloor = msCurRequest.curFloor;
                    if (mnTargetFloor == mnCurFloor)
                    {
                       mnCurPickingState = PICKING_GO_DEST;
                       mnTargetFloor = msCurRequest.targetFloor;
                       mnCurLoad += msCurRequest.passengerNum;
                    }
                    mnCurMovingState = ((mnTargetFloor - mnCurFloor) > 0 ? MOVING_UP : MOVING_DOWN);
                }
                else 
                {
                    mnCurPickingState = PICKING_STOPING;
                    mnCurMovingState = MOVING_WAIT;
                }
            }
            break;
        case PICKING_GO_PICK:
            if (mnCurFloor == msCurRequest.curFloor) //已到达乘客所在楼层
            {
                mnCurPickingState = PICKING_GO_DEST;
                mnTargetFloor = msCurRequest.targetFloor;
                mnCurLoad += msCurRequest.passengerNum;
                mnCurMovingState = ((mnTargetFloor - mnCurFloor) > 0 ? MOVING_UP : MOVING_DOWN);
            }
            break;
        default:
            break;
        }
    }

    void balanced_refresh_request()
    {
        int min_dist = mnMaxFloor - 1;
        int dist = min_dist;
        if (mnCurMovingState == MOVING_WAIT)
            mnTargetFloor = mnMaxFloor - 1;
        for (int i = 0; i < mvecPickedRequestVector.size(); i++)
        {
            dist = abs(mvecPickedRequestVector[i].targetFloor - mnCurFloor);
            if (dist < min_dist)
            {
                mnTargetFloor = mvecPickedRequestVector[i].targetFloor;
                min_dist = dist;
            }
            if (mvecPickedRequestVector[i].targetFloor == mnCurFloor)
            {
                mnCurLoad -= mvecPickedRequestVector[i].passengerNum;
                if (mbIsFull)
                    mbIsFull = false;
                mvecPickedRequestVector.erase(mvecPickedRequestVector.begin() + i);
                i--;
            }
        }
        if (!mbIsFull)
        {
            for (int i = 0; i < mvecUnpickedRequestVector.size(); i++)
            {
                dist = abs(mvecUnpickedRequestVector[i].curFloor - mnCurFloor);
                if (dist < min_dist && dist != 0)
                {
                    mnTargetFloor = mvecUnpickedRequestVector[i].curFloor;
                    min_dist = dist;
                }
                if (mvecUnpickedRequestVector[i].curFloor == mnCurFloor && !mbIsFull && (mnCurLoad + mvecUnpickedRequestVector[i].passengerNum) <= mnMaxLoad)
                {
                    mnCurLoad += mvecUnpickedRequestVector[i].passengerNum;
                    mvecPickedRequestVector.push_back(mvecUnpickedRequestVector[i]);
                    mvecUnpickedRequestVector.erase(mvecUnpickedRequestVector.begin() + i);
                    if (mnCurLoad == mnMaxLoad)
                        mbIsFull = true;
                }
            }
        }
        if (mvecUnpickedRequestVector.size() != 0 || mvecPickedRequestVector.size() != 0)
        {
            if (mnCurFloor == mnTargetFloor)
                mnCurMovingState = MOVING_WAIT;
            else
            mnCurMovingState = ((mnTargetFloor - mnCurFloor) > 0 ? MOVING_UP : MOVING_DOWN);
        }
        else
            mnCurMovingState = MOVING_WAIT;
    }
    void uppeak_refresh_request()
    {
        if (mnCurPickingState == PICKING_STOPING || mnCurMovingState == MOVING_WAIT)
        {
            if (mvecUnpickedRequestVector.size() > 0)
            {
                mnCurPickingState = PICKING_GO_PICK;   
                mnCurMovingState = MOVING_DOWN;
                mnTargetFloor = 0;
            }
        }
        switch (mnCurPickingState)
        {
        case PICKING_GO_DEST:
        {
            if (mnCurFloor == mnMaxFloor - 1)
            {
                if (mvecUnpickedRequestVector.size() > 0)
                {
                    mnCurMovingState = MOVING_DOWN;
                    mnCurPickingState = PICKING_GO_PICK;
                    mnTargetFloor = 0;
                }
                else
                {
                    mnCurMovingState = MOVING_WAIT;
                    mnCurPickingState = PICKING_STOPING;
                }
            }
            for (int i = 0; i < mvecPickedRequestVector.size(); i++)
            {
                if (mvecPickedRequestVector[i].targetFloor == mnCurFloor)
                {
                    mnCurLoad -= mvecPickedRequestVector[i].passengerNum;
                    if (mbIsFull)
                        mbIsFull = false;
                    mvecPickedRequestVector.erase(mvecPickedRequestVector.begin() + i);
                    i--;
                }
            }
            break;
        }
        case PICKING_GO_PICK:
        {
            if (mnCurFloor == 0)
            {
                mnCurMovingState = MOVING_UP;
                mnCurPickingState = PICKING_GO_DEST;
                mnTargetFloor = mnMaxFloor - 1;
            }
            if (mbIsFull)
                break;
            for (int i = 0; i < mvecUnpickedRequestVector.size(); i++)
            {
                if (mvecUnpickedRequestVector[i].curFloor == mnCurFloor)
                {
                    if (mnCurLoad + mvecUnpickedRequestVector[i].passengerNum > mnMaxLoad)
                    {
                        mbIsFull = true;
                        break;
                    }
                    mnCurLoad += mvecUnpickedRequestVector[i].passengerNum;
                    mvecPickedRequestVector.push_back(mvecUnpickedRequestVector[i]);
                    mvecUnpickedRequestVector.erase(mvecUnpickedRequestVector.begin() + i);
                    i--;
                }
            }
            break;
        }
        default:
            break;
        }
    }
    void downpeak_refresh_request()
    {
        if (mnCurPickingState == PICKING_STOPING || mnCurMovingState == MOVING_WAIT)
        {
            if (mvecUnpickedRequestVector.size() > 0)
            {
                mnCurPickingState = PICKING_GO_PICK;
                mnCurMovingState = MOVING_UP;
                mnTargetFloor = mnMaxFloor - 1;
            }
        }
        switch (mnCurPickingState)
        {
        case PICKING_GO_DEST:
        {
            if (mnCurFloor == 0)
            {
                if (mvecUnpickedRequestVector.size() > 0)
                {
                    mnCurMovingState = MOVING_UP;
                    mnCurPickingState = PICKING_GO_PICK;
                    mnTargetFloor = mnMaxFloor - 1;
                }
                else
                {
                    mnCurMovingState = MOVING_WAIT;
                    mnCurPickingState = PICKING_STOPING;
                }
            }
            for (int i = 0; i < mvecPickedRequestVector.size(); i++)
            {
                if (mvecPickedRequestVector[i].targetFloor == mnCurFloor)
                {
                    mnCurLoad -= mvecPickedRequestVector[i].passengerNum;
                    if (mbIsFull)
                        mbIsFull = false;
                    mvecPickedRequestVector.erase(mvecPickedRequestVector.begin() + i);
                    i--;
                }
            }
            break;
        }
        case PICKING_GO_PICK:
        {
            if (mnCurFloor == mnMaxFloor - 1)
            {
                mnCurMovingState = MOVING_DOWN;
                mnCurPickingState = PICKING_GO_DEST;
                mnTargetFloor = 0;
            }
            if (mbIsFull)
                break;
            for (int i = 0; i < mvecUnpickedRequestVector.size(); i++)
            {
                if (mvecUnpickedRequestVector[i].curFloor == mnCurFloor)
                {
                    if (mnCurLoad + mvecUnpickedRequestVector[i].passengerNum > mnMaxLoad)
                    {
                        mbIsFull = true;
                        break;
                    }
                    mnCurLoad += mvecUnpickedRequestVector[i].passengerNum;
                    mvecPickedRequestVector.push_back(mvecUnpickedRequestVector[i]);
                    mvecUnpickedRequestVector.erase(mvecUnpickedRequestVector.begin() + i);
                    i--;
                }
            }
            break;
        }
        default:
            break;
        }
    }
    void twoway_refresh_request()
    {
        if (mnCurPickingState == PICKING_STOPING || mnCurMovingState == MOVING_WAIT)
        {
            if (mvecUnpickedRequestVector.size() > 0)
            {
                mnCurPickingState = PICKING_GO_PICK;
                mnCurMovingState = MOVING_UP;
                mnTargetFloor = mnMaxFloor - 1;
            }
        }
        switch (mnCurPickingState)
        {
        case PICKING_GO_DEST:
        {
            int _min = mnMaxFloor - 1;
            for (int i = 0; i < mvecPickedRequestVector.size(); i++)
            {
                if (mvecPickedRequestVector[i].targetFloor < _min)
                    _min = mvecPickedRequestVector[i].targetFloor;
            }
            for (int i = 0; i < mvecUnpickedRequestVector.size(); i++)
            {
                if (mvecUnpickedRequestVector[i].curFloor < _min)
                    _min = mvecUnpickedRequestVector[i].curFloor;
            }
            if (_min < mnCurFloor)
                mnTargetFloor = _min;
            if (mnCurFloor == mnTargetFloor)
            {
                if (mvecUnpickedRequestVector.size() > 0)
                {
                    mnCurMovingState = MOVING_UP;
                    mnCurPickingState = PICKING_GO_PICK;
                    mnTargetFloor = mnMaxFloor - 1;
                }
                else
                {
                    mnCurMovingState = MOVING_WAIT;
                    mnCurPickingState = PICKING_STOPING;
                }
            }
            for (int i = 0; i < mvecPickedRequestVector.size(); i++)
            {
                if (mvecPickedRequestVector[i].targetFloor == mnCurFloor)
                {
                    mnCurLoad -= mvecPickedRequestVector[i].passengerNum;
                    if (mbIsFull)
                        mbIsFull = false;
                    mvecPickedRequestVector.erase(mvecPickedRequestVector.begin() + i);
                    i--;
                }
            }
            break;
        }
        case PICKING_GO_PICK:
        {
            int _max = 0;
            for (int i = 0; i < mvecUnpickedRequestVector.size(); i++)
            {
                if (mvecUnpickedRequestVector[i].curFloor > _max)
                    _max = mvecUnpickedRequestVector[i].curFloor;
            }
            for (int i = 0; i < mvecPickedRequestVector.size(); i++)
            {
                if (mvecPickedRequestVector[i].targetFloor > _max)
                    _max = mvecPickedRequestVector[i].targetFloor;
            }
            if (_max > mnCurFloor)
                mnTargetFloor = _max;
            if (mnCurFloor == mnTargetFloor)
            {
                mnCurMovingState = MOVING_DOWN;
                mnCurPickingState = PICKING_GO_DEST;
                mnTargetFloor = 0;
            }
            if (mbIsFull)
                break;
            for (int i = 0; i < mvecUnpickedRequestVector.size(); i++)
            {
                if (mvecUnpickedRequestVector[i].curFloor == mnCurFloor)
                {
                    if (mnCurLoad + mvecUnpickedRequestVector[i].passengerNum > mnMaxLoad)
                    {
                        mbIsFull = true;
                        break;
                    }
                    mnCurLoad += mvecUnpickedRequestVector[i].passengerNum;
                    mvecPickedRequestVector.push_back(mvecUnpickedRequestVector[i]);
                    mvecUnpickedRequestVector.erase(mvecUnpickedRequestVector.begin() + i);
                    i--;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    void refresh_static_info()
    {
        if(mnCurPolicy == POLICY_BALANCED)
        {
            if (mvecPickedRequestVector.size() > mvecUnpickedRequestVector.size())
                mnGoDestDuration++;
            else
                mnGoPickDuration++;
        }
        else
        {
            if (mnCurPickingState == PICKING_GO_DEST)
                mnGoDestDuration++;
            else if (mnCurPickingState == PICKING_GO_PICK)
                mnGoPickDuration++;
        }
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
        QString _str = QString::number(mnCurFloor) + "F " + QString::number(mnCurLoad) + "/" + QString::number(mnMaxLoad) + " " + _state + " -> " + QString::number(mnTargetFloor) + "F";
        return _str;
    }
    //参数
    int mnMaxLoad;
    int mnMaxFloor;
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
    bool mbIsFull;

    request msCurRequest;
    std::queue<request> mqueRequestQueue;
    std::vector<request> mvecUnpickedRequestVector;
    std::vector<request> mvecPickedRequestVector;
};