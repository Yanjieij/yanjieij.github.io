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

//ģ��ѡ��궨��
#define EASY 0
#define REAL 1

//ÿ���������״̬�궨��
#define NONE 0
#define ONLY_UP 1
#define ONLY_DOWN 2
#define BOTH 3

//��ʵģ������ֲ���
    /// ƽ�⽻ͨ ��������²��� POLICY_BALANCED 0
    /// ���и߷�/���и߷� �˿ͼ�����¥/��¥ POLICY_UPPEAK 1 POLICY_DOWNPEAK 2
    /// ��·��ͨ �˿ͼ���ǰ��ĳһ�� POLICY_TWOWAY 3

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
    //������
    Ui::MCESClass ui;
    QTimer *mctrlBarTimer;
    QTimer* mctrlCountTimer;
    int mnTimePassed;
    int mnTimerInterval;

    QList<QLabel*> mpElevatorVerticalLayoutList;
    QVBoxLayout* mpVerticalLayoutList;

    //ϵͳ����
    int mnSimulatePolicy;
    int mnSimulateModel;
    int mnElevatorNum;
    int mnMaxFloorHeight;
    int mnCabinVolume;
    int mnSimulateDuration;
    vector<elevator> mvecElevatorVec;
    int mnTwoWayTargetFloor;

    //��ǰ״̬����
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


