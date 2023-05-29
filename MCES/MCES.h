#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MCES.h"
#include <qstring.h>
#include <qtimer>

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
    int mnDuration;
    int mnTimerInterval;

public slots:
    void on_policySelection_currentIndexChanged(const int index);
    void on_modelSelection_currentIndexChanged(const int index);
    void on_startSimulateButton_clicked();
    void on_pauseSimulateButton_clicked();
    void updateSimulateProgress();
    void updateLeftTimeCounter();
    void on_simulateDuration_textChanged();
};


