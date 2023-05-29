#include "MCES.h"

MCES::MCES(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//初始化工作
	mnTimePassed = 0;
	mnDuration = 0;
	mnTimerInterval = 1000;	//1000ms
	mpBarTimer = new QTimer(this);
	mpBarTimer->setInterval(mnTimerInterval);
	mpCountTimer = new QTimer(this);
	mpCountTimer->setInterval(1000);
	connect(mpBarTimer, SIGNAL(timeout()), this, SLOT(updateSimulateProgress()));
	connect(mpCountTimer, SIGNAL(timeout()), this, SLOT(updateLeftTimeCounter()));
	ui.policyDescribe->setText(QStringLiteral("均衡交通，是正常时间使用的控制策略"));
	ui.simulateProcess->setRange(0, 100);
	ui.simulateProcess->setValue(0);
}

MCES::~MCES()
{
	delete mpBarTimer, mpCountTimer;
}

//选择策略
void MCES::on_policySelection_currentIndexChanged(const int index)
{
	switch (index)
	{
	case 0:
		ui.policyDescribe->setText(
			QStringLiteral("均衡交通，是正常时间使用的控制策略"));
		break;
	case 1:
		ui.policyDescribe->setText(
			QStringLiteral("绝大部分乘客要上楼，一般用于上班时间"));
		break;
	case 2:
		ui.policyDescribe->setText(
			QStringLiteral("绝大部分乘客要下楼，一般用于下班时间"));
		break;
	case 3:
		ui.policyDescribe->setText(
			QStringLiteral("乘客集中前往某一楼层，一般用于开会、吃饭时间"));
		break;
	default:
		ui.policyDescribe->setText(
			QStringLiteral("平衡状态：均衡交通，是正常时间使用的控制策略"));
		break;
	}
}

//选择模拟算法
void MCES::on_modelSelection_currentIndexChanged(const int index)
{
	//切换到蚁群算法
	if (index == 1)
	{
		ui.policySelection->setEnabled(0);
		ui.policyDescribe->setText(QStringLiteral(
			"调度策略仅适用于现实调度模拟，不适用于蚁群调度。"));
	}
	//切换到现实模拟算法
	else if (index == 0)
	{
		ui.policySelection->setEnabled(1);

		switch (ui.policySelection->currentIndex())
		{
		case 0:
			ui.policyDescribe->setText(QStringLiteral(
				"均衡交通，是正常时间使用的控制策略"));
			break;
		case 1:
			ui.policyDescribe->setText(QStringLiteral(
				"绝大部分乘客要上楼，一般用于上班时间"));
			break;
		case 2:
			ui.policyDescribe->setText(QStringLiteral(
				"绝大部分乘客要下楼，一般用于下班时间"));
			break;
		case 3:
			ui.policyDescribe->setText(QStringLiteral(
				"乘客集中前往某一楼层，一般用于开会、吃饭时间"));
			break;
		default:
			ui.policyDescribe->setText(QStringLiteral(
				"平衡状态：均衡交通，是正常时间使用的控制策略"));
			break;
		}
	}
}

//点击 开始模拟
void MCES::on_startSimulateButton_clicked()
{
	if (ui.simulateProcess->value() == 100)
	{
		ui.simulateProcess->setValue(0);
		mnTimePassed = 0;
	}
	mpBarTimer->start();
	mpCountTimer->start();

	int _nTime = mnDuration;
	QString _str;
	_str += QString::number(_nTime / 60);
	_str += " : ";
	_str += QString::number(_nTime % 60);
	ui.leftTime->setText(_str);
	ui.currentStatus->setText(QStringLiteral("正在运行"));
}
//点击 暂停模拟
void MCES::on_pauseSimulateButton_clicked()
{
	mpBarTimer->stop();
	mpCountTimer->stop();
	ui.currentStatus->setText(QStringLiteral("已暂停"));
}

//进度条和计时器配合
void MCES::updateSimulateProgress()
{
	int _nCurrentValue = ui.simulateProcess->value();
	_nCurrentValue++;
	if (_nCurrentValue >= 100)
	{
		mpBarTimer->stop();
		mpCountTimer->stop();
		ui.currentStatus->setText(QStringLiteral("已结束"));
	}
	ui.simulateProcess->setValue(_nCurrentValue);
}
void MCES::updateLeftTimeCounter()
{
	mnTimePassed++;
	int _nTime = mnDuration - mnTimePassed;
	if (_nTime < 0)
		return;
	QString _str;
	_str += QString::number(_nTime / 60);
	_str += " : ";
	_str += QString::number(_nTime % 60);
	ui.leftTime->setText(_str);
}
void MCES::on_simulateDuration_textChanged()
{
	mnDuration = ui.simulateDuration->text().toInt();
	mnTimerInterval = mnDuration * 10;
	mpBarTimer->setInterval(mnTimerInterval);
}