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
#include "AcoSimulationSystem.h"

//ģ��ѡ��궨��
#define ACO 1
#define REAL 0
//ÿ���������״̬�궨��
#define NONE 0
#define ONLY_UP 1
#define ONLY_DOWN 2
#define BOTH 3
//��ʵģ������ֲ���
    /// ƽ�⽻ͨ ��������²���
    /// ���и߷�/���и߷� �˿ͼ�����¥/��¥
    /// ��·��ͨ �˿ͼ���ǰ��ĳһ��
#define POLICY_BALANCED 0
#define POLICY_UPPEAK 1
#define POLICY_DOWNPEAK 2
#define POLICY_TWOWAY 3

using namespace std;

class MCES : public QMainWindow, public AcoSimulationSystem
{
    Q_OBJECT

public:
    MCES(QWidget *parent = nullptr);
    ~MCES();
    bool initialize_simulate_graph();
    void REAL_request_respond(int num);
    void ACO_request_respond(int num);

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
    void refresh_simulate_graph();

    void on_elevatorNum_textChanged();
    void on_maxHeight_textChanged();
    void on_cabinVolume_textChanged();
    void on_simulateDuration_textChanged();
};


