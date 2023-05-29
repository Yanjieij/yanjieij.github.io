#include "MCES.h"

MCES::MCES(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//��ʼ������
	mnTimePassed = 0;
	mnDuration = 0;
	mnTimerInterval = 1000;	//1000ms
	mpBarTimer = new QTimer(this);
	mpBarTimer->setInterval(mnTimerInterval);
	mpCountTimer = new QTimer(this);
	mpCountTimer->setInterval(1000);
	connect(mpBarTimer, SIGNAL(timeout()), this, SLOT(updateSimulateProgress()));
	connect(mpCountTimer, SIGNAL(timeout()), this, SLOT(updateLeftTimeCounter()));
	ui.policyDescribe->setText(QStringLiteral("���⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
	ui.simulateProcess->setRange(0, 100);
	ui.simulateProcess->setValue(0);
}

MCES::~MCES()
{
	delete mpBarTimer, mpCountTimer;
}

//ѡ�����
void MCES::on_policySelection_currentIndexChanged(const int index)
{
	switch (index)
	{
	case 0:
		ui.policyDescribe->setText(
			QStringLiteral("���⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
		break;
	case 1:
		ui.policyDescribe->setText(
			QStringLiteral("���󲿷ֳ˿�Ҫ��¥��һ�������ϰ�ʱ��"));
		break;
	case 2:
		ui.policyDescribe->setText(
			QStringLiteral("���󲿷ֳ˿�Ҫ��¥��һ�������°�ʱ��"));
		break;
	case 3:
		ui.policyDescribe->setText(
			QStringLiteral("�˿ͼ���ǰ��ĳһ¥�㣬һ�����ڿ��ᡢ�Է�ʱ��"));
		break;
	default:
		ui.policyDescribe->setText(
			QStringLiteral("ƽ��״̬�����⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
		break;
	}
}

//ѡ��ģ���㷨
void MCES::on_modelSelection_currentIndexChanged(const int index)
{
	//�л�����Ⱥ�㷨
	if (index == 1)
	{
		ui.policySelection->setEnabled(0);
		ui.policyDescribe->setText(QStringLiteral(
			"���Ȳ��Խ���������ʵ����ģ�⣬����������Ⱥ���ȡ�"));
	}
	//�л�����ʵģ���㷨
	else if (index == 0)
	{
		ui.policySelection->setEnabled(1);

		switch (ui.policySelection->currentIndex())
		{
		case 0:
			ui.policyDescribe->setText(QStringLiteral(
				"���⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
			break;
		case 1:
			ui.policyDescribe->setText(QStringLiteral(
				"���󲿷ֳ˿�Ҫ��¥��һ�������ϰ�ʱ��"));
			break;
		case 2:
			ui.policyDescribe->setText(QStringLiteral(
				"���󲿷ֳ˿�Ҫ��¥��һ�������°�ʱ��"));
			break;
		case 3:
			ui.policyDescribe->setText(QStringLiteral(
				"�˿ͼ���ǰ��ĳһ¥�㣬һ�����ڿ��ᡢ�Է�ʱ��"));
			break;
		default:
			ui.policyDescribe->setText(QStringLiteral(
				"ƽ��״̬�����⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
			break;
		}
	}
}

//��� ��ʼģ��
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
	ui.currentStatus->setText(QStringLiteral("��������"));
}
//��� ��ͣģ��
void MCES::on_pauseSimulateButton_clicked()
{
	mpBarTimer->stop();
	mpCountTimer->stop();
	ui.currentStatus->setText(QStringLiteral("����ͣ"));
}

//�������ͼ�ʱ�����
void MCES::updateSimulateProgress()
{
	int _nCurrentValue = ui.simulateProcess->value();
	_nCurrentValue++;
	if (_nCurrentValue >= 100)
	{
		mpBarTimer->stop();
		mpCountTimer->stop();
		ui.currentStatus->setText(QStringLiteral("�ѽ���"));
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