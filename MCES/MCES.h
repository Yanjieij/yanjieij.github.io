#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MCES.h"
#include <qstring.h>
#include <qlist.h>
#include <vector>
#include <qtimer>
#include <queue>
#include "elevator.hpp"
#include "request.hpp"

//模型选择宏定义
#define EASY 0
#define REAL 1

//每层电梯请求状态宏定义
#define NONE 0
#define ONLY_UP 1
#define ONLY_DOWN 2
#define BOTH 3

//真实模拟的四种策略
    /// 平衡交通 正常情况下采用 POLICY_BALANCED 0
    /// 上行高峰/下行高峰 乘客集中上楼/下楼 POLICY_UPPEAK 1 POLICY_DOWNPEAK 2
    /// 二路交通 乘客集中前往某一层 POLICY_TWOWAY 3

using namespace std;

class MCES : public QMainWindow
{
    Q_OBJECT

public:
    MCES(QWidget *parent = nullptr);
    ~MCES();
    bool initialize_simulate_graph();
    void REAL_request_respond(int num);
    void EASY_request_respond(int num);

    int easy_elevator_select(request _r, int excep = -1);
    int balanced_elevator_select(request _r, int excep = -1);
    int uppeak_elevator_select(request _r, int excep = -1);
    int downpeak_elevator_select(request _r, int excep = -1);
    int twoway_elevator_select(request _r, int excep = -1);

private:
    //界面用
    Ui::MCESClass ui;
    QTimer *mctrlBarTimer;
    QTimer* mctrlCountTimer;
    int mnTimePassed;
    int mnTimerInterval;

    QList<QLabel*> mpElevatorVerticalLayoutList;
    QVBoxLayout* mpVerticalLayoutList;

    //系统参数
    int mnSimulatePolicy;
    int mnSimulateModel;
    int mnElevatorNum;
    int mnMaxFloorHeight;
    int mnCabinVolume;
    int mnSimulateDuration;
    vector<elevator> mvecElevatorVec;
    int mnTwoWayTargetFloor;

    //当前状态变量
    int* mpEachFloorWaitingPassengerNum;
    int* mpEachFloorRequestStatus;
    queue<request> mqueRequestList;

public slots:
    void on_policySelection_currentIndexChanged(const int index);
    void on_modelSelection_currentIndexChanged(const int index);
    void on_startSimulateButton_clicked();
    void on_pauseSimulateButton_clicked();
    void update_simulate_progress();
    void updat_left_time_counter();
    int simulate_passenger_request();
    void activate_elevators_move();
    void refresh_simulate_graph();

    void on_elevatorNum_textChanged();
    void on_maxHeight_textChanged();
    void on_cabinVolume_textChanged();
    void on_simulateDuration_textChanged();
};


