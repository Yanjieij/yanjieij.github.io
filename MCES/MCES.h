#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MCES.h"
#include <qstring.h>
#include <qlist.h>
#include <vector>
#include <qtimer>
#include "elevator.hpp"
#include "AcoSimulationSystem.h"
#include "RealPolicySimulationSystem.h"

#define ACO 1
#define REAL 0

using namespace std;

class MCES : public QMainWindow
{
    Q_OBJECT

public:
    MCES(QWidget *parent = nullptr);
    ~MCES();
    bool initialize_simulate_graph();

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
    int mnSimulateModel;
    int mnElevatorNum;
    int mnMaxFloorHeight;
    int mnCabinVolume;
    int mnSimulateDuration;
    vector<elevator> mvecElevatorVec;

    RealPolicySimulationSystem mRealSystem;
    AcoSimulationSystem mAcoSystem;

public slots:
    void on_policySelection_currentIndexChanged(const int index);
    void on_modelSelection_currentIndexChanged(const int index);
    void on_startSimulateButton_clicked();
    void on_pauseSimulateButton_clicked();
    void updateSimulateProgress();
    void updateLeftTimeCounter();

    void on_elevatorNum_textChanged();
    void on_maxHeight_textChanged();
    void on_cabinVolume_textChanged();
    void on_simulateDuration_textChanged();
};


