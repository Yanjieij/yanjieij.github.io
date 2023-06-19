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
	connect(mctrlBarTimer, SIGNAL(timeout()), this, SLOT(update_simulate_progress()));
	connect(mctrlCountTimer, SIGNAL(timeout()), this, SLOT(updat_left_time_counter()));
	connect(mctrlCountTimer, SIGNAL(timeout()), this, SLOT(simulate_passenger_request()));
	//connect(mctrlCountTimer, SIGNAL(timeout()), this, SLOT(refresh_simulate_graph());
	ui.policyDescribe->setText(QStringLiteral("���⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
	ui.simulateProcess->setRange(0, 100);
	ui.simulateProcess->setValue(0);

	mpVerticalLayoutList = nullptr;

	mpEachFloorWaitingPassengerNum = nullptr;
	mpEachFloorRequestStatus = NONE;

	//����
	mnSimulatePolicy = POLICY_BALANCED;
	mnSimulateModel = REAL;
	mnElevatorNum = 0;
	mnMaxFloorHeight = 0;
	mnCabinVolume = 0;
	mnSimulateDuration = 0;
	mnTwoWayTargetFloor = 0;
}


MCES::~MCES()
{
	delete[] mpEachFloorRequestStatus;
	delete[] mpEachFloorWaitingPassengerNum;
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
	//����label
	for (int i = 0; i < mnElevatorNum; i++)
	{
		for (int cnt = 0; cnt < mnMaxFloorHeight; cnt++)
		{
			mpElevatorVerticalLayoutList[i][cnt].setMargin(5);
			mpElevatorVerticalLayoutList[i][cnt].setAlignment(Qt::AlignCenter);
			mpElevatorVerticalLayoutList[i][cnt].setStyleSheet("border: 1px dashed black;");
			QString _str = " ";
			mpElevatorVerticalLayoutList[i][cnt].setText(_str);
			mpVerticalLayoutList[i].addWidget(&(mpElevatorVerticalLayoutList[i][cnt]));
		}
	}
	//��ɲ���
	//ui.simulateGraph->setSpacing(8);
	for (int i = 0; i < mnElevatorNum; i++)
	{
		ui.simulateGraph->addLayout(&(mpVerticalLayoutList[i]));
	}
	return 1;
}

int MCES::simulate_passenger_request()
{
	int _exist = rand() % 11;
	if (_exist > 9)
	{
		return 0;
	}
	else
	{
		int _num = rand() % 3;
		int _cnt = _num;
		while (_num > 0)
		{
			_num--;			
			request _r;
			int _from = 0;
			int _to = 0;
			switch (mnSimulatePolicy)
			{
			case POLICY_BALANCED:
			{
				_from = rand() % (2 * mnMaxFloorHeight);
				if (_from >= mnMaxFloorHeight)
					_from = 0;
				_to = rand() % (2 * mnMaxFloorHeight);
				if (_to >= mnMaxFloorHeight)
					_to = 0;
				while (_from == _to)
				{
					_to = rand() % (2 * mnMaxFloorHeight);
					if (_to >= mnMaxFloorHeight)
						_to = 0;
				}
				break;
			}
			case POLICY_UPPEAK:
			{
				_from = rand() % (5 * mnMaxFloorHeight);
				if (_from >= mnMaxFloorHeight)
					_from = 0;
				_to = rand() % mnMaxFloorHeight;
				while (_from == _to)
				{
					_to = rand() % mnMaxFloorHeight;
				}
				break;
			}
			case POLICY_DOWNPEAK:
			{
				_to = rand() % (5 * mnMaxFloorHeight);
				if (_to >= mnMaxFloorHeight)
					_to = 0;
				_from = rand() % mnMaxFloorHeight;
				while (_from == _to)
				{
					_from = rand() % mnMaxFloorHeight;
				}
				break;
			}
			case POLICY_TWOWAY:
			{
				_to = rand() % (5 * mnMaxFloorHeight);
				if (_to >= mnMaxFloorHeight)
					_to = mnTwoWayTargetFloor;
				_from = rand() % mnMaxFloorHeight;
				while (_from == _to)
				{
					_from = rand() % mnMaxFloorHeight;
				}
				break;
			}
			default:
				break;
			}
			_r.passengerNum = (rand() % 5) + 1;
			_r.curFloor = _from;
			_r.targetFloor = _to;
			if (_from > _to)
				_r.status = DOWN;
			else
				_r.status = UP;
			mqueRequestList.push(_r);
		}
		if (mnSimulateModel == REAL)
			REAL_request_respond(_num);
		else if (mnSimulateModel == ACO)
			ACO_request_respond(_num);
		return _num;
	}
}

int MCES::balanced_elevator_select(request _r)
{
	//������ĵ��ݽ��г���
	int _minDist = mnMaxFloorHeight;
	int pickIndex = -1;
	for (int i = 0; i < mvecElevatorVec.size(); i++)
	{
		int _dist = abs(_r.targetFloor - mvecElevatorVec[i].mnCurFloor);
		if (_dist < _minDist)
		{
			_minDist = _dist;
			pickIndex = i;
		}
	}
	return pickIndex;
}
int MCES::uppeak_elevator_select(request _r)
{
	if (_r.status == UP)
	{

	}
	return 0;
}
int MCES::downpeak_elevator_select(request _r)
{
	return 0;
}
int MCES::twoway_elevator_select(request _r)
{
	return 0;
}

void MCES::REAL_request_respond(int num)
{
	//int cnt = 0;
	while (!mqueRequestList.empty())
	{
		//cnt++;
		auto cur = mqueRequestList.front();
		int _elevatorSelected = -1;
		switch (mnSimulatePolicy)
		{
		case POLICY_BALANCED:
		{
			_elevatorSelected = balanced_elevator_select(cur);
			break;
		}
		case POLICY_UPPEAK:
		{
			_elevatorSelected = uppeak_elevator_select(cur);
			break;
		}
		case POLICY_DOWNPEAK:
		{
			_elevatorSelected = downpeak_elevator_select(cur);
			break;
		}
		case POLICY_TWOWAY:
		{
			_elevatorSelected = twoway_elevator_select(cur);
			break;
		}
		default:
			break;
		}
		mvecElevatorVec[_elevatorSelected].add_request(cur);

		mqueRequestList.pop();
	}
	refresh_simulate_graph();
}

void MCES::ACO_request_respond(int num)
{
	int cnt = 0;
	while (cnt < num)
	{
		cnt++;
		auto cur = mqueRequestList.front();

		mqueRequestList.pop();
	}
}

void MCES::refresh_simulate_graph()
{
	for (int i = 0; i < mvecElevatorVec.size(); i++)
	{
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnCurFloor].setText(mvecElevatorVec[i].generate_info_display());
	}
}

//ѡ�����
void MCES::on_policySelection_currentIndexChanged(const int index)
{
	switch (index)
	{
	case 0:
		ui.policyDescribe->setText(
			QStringLiteral("���⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
		mnSimulatePolicy = POLICY_BALANCED;
		break;
	case 1:
		ui.policyDescribe->setText(
			QStringLiteral("���󲿷ֳ˿�Ҫ��¥��һ�������ϰ�ʱ��"));
		mnSimulatePolicy = POLICY_UPPEAK;
		break;
	case 2:
		ui.policyDescribe->setText(
			QStringLiteral("���󲿷ֳ˿�Ҫ��¥��һ�������°�ʱ��"));
		mnSimulatePolicy = POLICY_DOWNPEAK;
		break;
	case 3:
		ui.policyDescribe->setText(
			QStringLiteral("�˿ͼ���ǰ��ĳһ¥�㣬һ�����ڿ��ᡢ�Է�ʱ��"));
		srand((unsigned)time(NULL));
		mnTwoWayTargetFloor = rand() % mnMaxFloorHeight;
		mnSimulatePolicy = POLICY_TWOWAY;
		break;
	default:
		ui.policyDescribe->setText(
			QStringLiteral("ƽ��״̬�����⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
		mnSimulatePolicy = POLICY_BALANCED;
		break;
	}
	//RealPolicySimulationSystem::set_policy(index);
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
	
	srand((unsigned)time(NULL));

	//״̬������
	if (ui.simulateProcess->value() == 100)
	{
		ui.simulateProcess->setValue(0);
		mnTimePassed = 0;
	}

	int _nTime = mnSimulateDuration;
	QString _str;
	_str += QString::number(_nTime / 60);
	_str += " : ";
	_str += QString::number(_nTime % 60);
	ui.leftTime->setText(_str);
	ui.currentStatus->setText(QStringLiteral("��������"));
	
	initialize_simulate_graph();

	mpEachFloorRequestStatus = new int[mnMaxFloorHeight];
	mpEachFloorWaitingPassengerNum = new int[mnMaxFloorHeight];

	mctrlBarTimer->start();
	mctrlCountTimer->start();
}
//��� ��ͣģ��
void MCES::on_pauseSimulateButton_clicked()
{
	mctrlBarTimer->stop();
	mctrlCountTimer->stop();
	ui.currentStatus->setText(QStringLiteral("����ͣ"));
}

//�������ͼ�ʱ�����
void MCES::update_simulate_progress()
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
void MCES::updat_left_time_counter()
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