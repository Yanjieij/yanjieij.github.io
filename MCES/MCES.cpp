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
	mctrlCallTimer = new QTimer(this);
	mctrlCallTimer->setInterval(1000);
	mctrlCountTimer = new QTimer(this);
	mctrlCountTimer->setInterval(mnTimerInterval);
	mctrlStaticTimer = new QTimer(this);
	mctrlStaticTimer->setInterval(mnTimerInterval);

	connect(mctrlCallTimer, SIGNAL(timeout()), this, SLOT(functions_call()));
	connect(mctrlBarTimer, SIGNAL(timeout()), this, SLOT(update_simulate_progress()));
	connect(mctrlCountTimer, SIGNAL(timeout()), this, SLOT(update_left_time_counter()));
	connect(mctrlStaticTimer, SIGNAL(timeout()), this, SLOT(update_static_info()));
	//connect(mctrlCountTimer, SIGNAL(timeout()), this, SLOT(refresh_simulate_graph());
	//ui.policyDescribe->setText(QStringLiteral("���⽻ͨ��������ʱ��ʹ�õĿ��Ʋ���"));
	ui.policySelection->setEnabled(0);
	ui.policyDescribe->setText(QStringLiteral(
		"���Ȳ��Խ���������ʵ����ģ�⣬�������ڼ򵥵��ȡ�"));
	ui.simulateProcess->setRange(0, 100);
	ui.simulateProcess->setValue(0);
	ui.ARC->setText(QString::number(0));
	ui.ATD->setText(QString::number(0));
	ui.AWD->setText(QString::number(0));
	mnSumARC = 0;
	mnSumATD = 0;
	mnSumAWD = 0;
	mnSumMovedElevatorNum = 0;

	mpVerticalLayoutList = nullptr;
	mpEachFloorWaitingPassengerNum = nullptr;

	//����
	mnSimulatePolicy = POLICY_EASY;
	mnSimulateModel = EASY;
	mnElevatorNum = 0;
	mnMaxFloorHeight = 0;
	mnCabinVolume = 0;
	mnSimulateDuration = 0;
	mnTwoWayTargetFloor = 0;
}

MCES::~MCES()
{
	delete[] mpEachFloorWaitingPassengerNum;
	for (int i = 0; i < mnElevatorNum; i++)
	{
		delete[] mpElevatorVerticalLayoutList[i];
	}
	delete[] mpVerticalLayoutList;
}

void MCES::functions_call()
{
	simulate_passenger_request();
	if (mnSimulateModel == REAL)
		REAL_request_respond();
	else
		EASY_request_respond();
	activate_elevators_move();
	refresh_simulate_graph();
}

bool MCES::initialize_simulate_graph()
{
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
			mpVerticalLayoutList[i].insertWidget(0, &(mpElevatorVerticalLayoutList[i][cnt]));
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
	int _exist = rand() % 14;
	if (_exist > 9)
	{
		return 0;
	}
	else
	{
		int _num = rand() % mnElevatorNum + 1;
		while (_num > 0)
		{
			_num--;			
			request _r;
			int _from = 0;
			int _to = 0;
			switch (mnSimulatePolicy)
			{
			case POLICY_EASY:
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
				_from = rand() % (3 * mnMaxFloorHeight);
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
				_from = rand() % (mnMaxFloorHeight - 4) + 2;
				while (_from == _to)
				{
					_from = rand() % mnMaxFloorHeight;
				}
				break;
			}
			default:
				break;
			}
			_r.passengerNum = rand() % (mnCabinVolume / 2) + 1;
			if (mnSimulateModel == EASY)
				_r.passengerNum = rand() % (mnCabinVolume - 2) + 1;
			_r.curFloor = _from;
			_r.targetFloor = _to;
			if (_from > _to)
				_r.status = DOWN;
			else
				_r.status = UP;
			mqueRequestList.push(_r);
		}
		/*
		if (mnSimulateModel == REAL)
			REAL_request_respond(_num);
		else if (mnSimulateModel == EASY)
			EASY_request_respond(_num);
		*/
		return _num;
	}
}


int MCES::balanced_elevator_select(request _r, int excep)
{
	//������ĵ��ݽ��г���
	int pickIndex = -1;
	if (excep == -1)
	{
		int _minDist = mnMaxFloorHeight;
		for (int i = 0; i < mvecElevatorVec.size(); i++)
		{
			int _dist = abs(_r.curFloor - mvecElevatorVec[i].mnCurFloor);
			if (_dist < _minDist)
			{
				_minDist = _dist;
				pickIndex = i;
			}
		}
		for (int i = 0; i < mnElevatorNum; i++)
		{
			if (mvecElevatorVec[i].mvecUnpickedRequestVector.size() == 0)
			{
				return i;
			}
		}
	}
	else
	{
		int _exceptDist = abs(_r.curFloor - mvecElevatorVec[excep].mnCurFloor);
		int _minDist = mnMaxFloorHeight;
		for (int i = 0; i < mvecElevatorVec.size(); i++)
		{
			if (i == excep)
				continue;
			int _dist = abs(_r.curFloor - mvecElevatorVec[i].mnCurFloor);
			if (_dist < _exceptDist)
				continue;
			if (_dist < _minDist)
			{
				_minDist = _dist;
				pickIndex = i;
			}
		}
	}
	return pickIndex;
}


void MCES::REAL_request_respond()
{
	//int cnt = 0;
	while (!mqueRequestList.empty())
	{
		//cnt++;
		auto cur_r = mqueRequestList.front();
		int _elevatorSelected = -1;
		switch (mnSimulatePolicy)
		{
		case POLICY_BALANCED:
		{
			_elevatorSelected = balanced_elevator_select(cur_r);
			mvecElevatorVec[_elevatorSelected].add_request(cur_r);
			break;
		}
		case POLICY_UPPEAK:
		{
			_elevatorSelected = rand() % mnElevatorNum;
			for (int i = 0; i < mnElevatorNum; i++)
			{
				if (mvecElevatorVec[i].mvecUnpickedRequestVector.size() == 0)
				{
					_elevatorSelected = i;
					break;
				}
			}
			mvecElevatorVec[_elevatorSelected].add_request(cur_r);
			break;
		}
		case POLICY_DOWNPEAK:
		{
			_elevatorSelected = rand() % mnElevatorNum;
			for (int i = 0; i < mnElevatorNum; i++)
			{
				if (mvecElevatorVec[i].mvecUnpickedRequestVector.size() == 0)
				{
					_elevatorSelected = i;
					break;
				}
			}
			mvecElevatorVec[_elevatorSelected].add_request(cur_r);
			break;
		}
		case POLICY_TWOWAY:
		{
			_elevatorSelected = rand() % mnElevatorNum;
			for (int i = 0; i < mnElevatorNum; i++)
			{
				if (mvecElevatorVec[i].mvecUnpickedRequestVector.size() == 0)
				{
					_elevatorSelected = i;
					break;
				}
			}
			mvecElevatorVec[_elevatorSelected].add_request(cur_r);
			break;
		}
		default:
			break;
		}
		mqueRequestList.pop();
	}
	refresh_simulate_graph();
}

void MCES::EASY_request_respond()
{
	//int cnt = 0;
	while (!mqueRequestList.empty())
	{
		//cnt++;
		auto cur_r = mqueRequestList.front();
		int _elevatorSelected = rand() % mnElevatorNum;
		for (int i = 0; i < mnElevatorNum; i++)
		{
			if (mvecElevatorVec[i].mqueRequestQueue.empty())
			{
				_elevatorSelected = i;
				break;
			}
		}
		mvecElevatorVec[_elevatorSelected].add_request(cur_r);
		mqueRequestList.pop();
	}
	//activate_elevators_move();
	//refresh_simulate_graph();
}

void MCES::activate_elevators_move()
{
	for (int i = 0; i < mnElevatorNum; i++)
	{
		mvecElevatorVec[i].execute_moving();
	}
}

void MCES::refresh_simulate_graph()
{
	for (int i = 0; i < mnElevatorNum; i++)
	{
		/*
		if(mnSimulatePolicy == POLICY_EASY)
			mpElevatorVerticalLayoutList[i][mnMaxFloorHeight-1].setText(QString::number(mvecElevatorVec[i].mqueRequestQueue.size()));
		else 
			mpElevatorVerticalLayoutList[i][mnMaxFloorHeight - 1].setText(QString::number(mvecElevatorVec[i].mvecUnpickedRequestVector.size()));
		*/
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnCurFloor].setText(mvecElevatorVec[i].generate_info_display());
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnCurFloor].setStyleSheet("border: 2px solid black;");
		if (mvecElevatorVec[i].mnLastFloor == -1)
			continue;
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnLastFloor].setText("");
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnLastFloor].setStyleSheet("border: 1px dashed black;");
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
	//�л������㷨
	if (index == EASY)
	{
		ui.policySelection->setEnabled(0);
		ui.policyDescribe->setText(QStringLiteral(
			"���Ȳ��Խ���������ʵ����ģ�⣬�������ڼ򵥵��ȡ�"));
		mnSimulateModel = EASY;
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
		mnSimulatePolicy = POLICY_BALANCED;
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
	mnTwoWayTargetFloor = mnMaxFloorHeight / 2;

	if (mvecElevatorVec.empty())
	{
		//��������
		for (int i = 0; i < mnElevatorNum; i++)
		{
			elevator _ele(mnCabinVolume, i);
			_ele.mnCurPolicy = mnSimulatePolicy;
			_ele.mnMaxFloor = mnMaxFloorHeight;
			mvecElevatorVec.push_back(_ele);
		}

		initialize_simulate_graph();

		mpEachFloorWaitingPassengerNum = new int[mnMaxFloorHeight];
	}

	if (mnSimulatePolicy == POLICY_UPPEAK)
	{
		for (int i = 0; i < mnElevatorNum; i++)
		{
			mvecElevatorVec[i].mnCurFloor = mnMaxFloorHeight - 1;
		}
	}

	mctrlBarTimer->start();
	mctrlCountTimer->start();
	mctrlCallTimer->start();
	mctrlStaticTimer->start();
}
//��� ��ͣģ��
void MCES::on_pauseSimulateButton_clicked()
{
	mctrlBarTimer->stop();
	mctrlCountTimer->stop();
	mctrlCallTimer->stop();
	mctrlStaticTimer->stop();
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
		mctrlCallTimer->stop();
		mctrlStaticTimer->stop();
		ui.currentStatus->setText(QStringLiteral("�ѽ���"));
	}
	ui.simulateProcess->setValue(_nCurrentValue);
}
void MCES::update_left_time_counter()
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

void MCES::update_static_info()
{
	int AWD = 0, ATD = 0;
	int ARC = 0;
	int cnt_move = 0;
	int _vec_size = 0;

	for (int i = 0; i < mnElevatorNum; i++)
	{
		mvecElevatorVec[i].refresh_static_info();
		if (mvecElevatorVec[i].mnCurMovingState == PICKING_STOPING)
			continue;
		if (mnSimulatePolicy == POLICY_EASY)
			_vec_size = mvecElevatorVec[i].mqueRequestQueue.size();
		else
			_vec_size = mvecElevatorVec[i].mvecUnpickedRequestVector.size();
		AWD += _vec_size * mvecElevatorVec[i].mnGoPickDuration;
		ATD += mvecElevatorVec[i].mnGoDestDuration;
		ARC += mvecElevatorVec[i].mnCurLoad;
		cnt_move++;
	}
	if (cnt_move == 0)
		return;
	mnSumMovedElevatorNum += cnt_move;
	mnSumATD += ATD;
	mnSumAWD += AWD;
	mnSumARC += ARC;
	float _awd = (float)mnSumAWD / (float)mnSumMovedElevatorNum;
	float _atd = (float)mnSumATD / (float)mnSumMovedElevatorNum;
	float _arc = (float)mnSumARC / (float)(mnSumMovedElevatorNum * mnSumMovedElevatorNum);
	ui.AWD->setText(QString::number(_awd));
	ui.ATD->setText(QString::number(_atd));
	ui.ARC->setText(QString::number(100.0 *_arc) + "%");
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