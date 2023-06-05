#include "MCES.h"

MCES::MCES(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//��ʼ������
	mnTimePassed = 0;
	mnTimerInterval = 1000;	//1000ms
	mctrlBarTimer = new QTimer(this);
	mctrlBarTimer->setInterval(mnTimerInterval);
	mctrlCountTimer = new QTimer(this);
	mctrlCountTimer->setInterval(1000);
	connect(mctrlBarTimer, SIGNAL(timeout()), this, SLOT(updateSimulateProgress()));
	connect(mctrlCountTimer, SIGNAL(timeout()), this, SLOT(updateLeftTimeCounter()));
	ui.policyDescribe->setText(QStringLiteral("���⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
	ui.simulateProcess->setRange(0, 100);
	ui.simulateProcess->setValue(0);

	mpVerticalLayoutList = nullptr;

	//����
	mnSimulateModel = -1;
	mnElevatorNum = 0;
	mnMaxFloorHeight = 0;
	mnCabinVolume = 0;
	mnSimulateDuration = 0;
}


MCES::~MCES()
{
	for (int i = 0; i < mnElevatorNum; i++)
	{
		delete[] mpElevatorVerticalLayoutList[i];
	}
	delete[] mpVerticalLayoutList;
}


bool MCES::initialize_simulate_graph()
{
	//��������
	for (int i = 0; i < mnElevatorNum; i++)
	{
		elevator _ele(mnCabinVolume, i);
		mvecElevatorVec.push_back(_ele);
	}
	//����hbox����
	mpVerticalLayoutList = new QVBoxLayout[mnElevatorNum]();
	//����lable
	for (int i = 0; i < mnElevatorNum; i++)
	{
		QLabel* _label;
		mpElevatorVerticalLayoutList.append(_label);
		mpElevatorVerticalLayoutList[i] = new QLabel[mnMaxFloorHeight]();
	}
	for (int i = 0; i < mnElevatorNum; i++)
	{
		for (int cnt = 0; cnt < mnMaxFloorHeight; cnt++)
		{
			mpElevatorVerticalLayoutList[i][cnt].setMargin(5);
			QString _str = "11111";
			mpElevatorVerticalLayoutList[i][cnt].setText(_str);
			mpVerticalLayoutList[i].addWidget(&(mpElevatorVerticalLayoutList[i][cnt]));
		}
	}
	for (int i = 0; i < mnElevatorNum; i++)
	{
		ui.simulateGraph->addLayout(&(mpVerticalLayoutList[i]));
	}
	return 1;
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
	if (index == ACO)
	{
		ui.policySelection->setEnabled(0);
		ui.policyDescribe->setText(QStringLiteral(
			"���Ȳ��Խ���������ʵ����ģ�⣬����������Ⱥ���ȡ�"));
		mnSimulateModel = ACO;
	}
	//�л�����ʵģ���㷨
	else if (index == REAL)
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
		mnSimulateModel = REAL;
	}
}

//��� ��ʼģ��
void MCES::on_startSimulateButton_clicked()
{
	//��������
	on_cabinVolume_textChanged();
	on_elevatorNum_textChanged();
	on_maxHeight_textChanged();
	on_simulateDuration_textChanged();
	

	//״̬������
	if (ui.simulateProcess->value() == 100)
	{
		ui.simulateProcess->setValue(0);
		mnTimePassed = 0;
	}
	mctrlBarTimer->start();
	mctrlCountTimer->start();

	int _nTime = mnSimulateDuration;
	QString _str;
	_str += QString::number(_nTime / 60);
	_str += " : ";
	_str += QString::number(_nTime % 60);
	ui.leftTime->setText(_str);
	ui.currentStatus->setText(QStringLiteral("��������"));
	
	initialize_simulate_graph();

}
//��� ��ͣģ��
void MCES::on_pauseSimulateButton_clicked()
{
	mctrlBarTimer->stop();
	mctrlCountTimer->stop();
	ui.currentStatus->setText(QStringLiteral("����ͣ"));
}

//�������ͼ�ʱ�����
void MCES::updateSimulateProgress()
{
	int _nCurrentValue = ui.simulateProcess->value();
	_nCurrentValue++;
	if (_nCurrentValue >= 100)
	{
		mctrlBarTimer->stop();
		mctrlCountTimer->stop();
		ui.currentStatus->setText(QStringLiteral("�ѽ���"));
	}
	ui.simulateProcess->setValue(_nCurrentValue);
}
void MCES::updateLeftTimeCounter()
{
	mnTimePassed++;
	int _nTime = mnSimulateDuration - mnTimePassed;
	if (_nTime < 0)
		return;
	QString _str;
	_str += QString::number(_nTime / 60);
	_str += " : ";
	_str += QString::number(_nTime % 60);
	ui.leftTime->setText(_str);
}

//��������
void MCES::on_elevatorNum_textChanged()
{
	mnElevatorNum = ui.elevatorNum->text().toInt();
}
void MCES::on_maxHeight_textChanged()
{
	mnMaxFloorHeight = ui.maxHeight->text().toInt();
}
void MCES::on_cabinVolume_textChanged()
{
	mnCabinVolume = ui.cabinVolume->text().toInt();
}
void MCES::on_simulateDuration_textChanged()
{
	mnSimulateDuration = ui.simulateDuration->text().toInt();
	mnTimerInterval = mnSimulateDuration * 10;
	mctrlBarTimer->setInterval(mnTimerInterval);
}