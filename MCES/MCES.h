#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MCES.h"
#include <qstring.h>
#include <vector>
#include <qtimer>
#include "elevator.hpp"

#define ACO 1
#define REAL 0

using namespace std;

class MCES : public QMainWindow
{
    Q_OBJECT

public:
    MCES(QWidget *parent = nullptr);
    ~MCES();

private:
    Ui::MCESClass ui;
    QTimer *mpBarTimer;
    QTimer* mpCountTimer;
    int mnTimePassed;
    int mnTimerInterval;

    int mnSimulateModel;
    vector<elevator> mpElevatorVec;
    int mnElevatorNum;
    int mnMaxHeight;
    int mnCabinVolume;
    int mnSimulateDuration;

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


