#include "MCES.h"

MCES::MCES(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//初始化工作
	mnTimePassed = 0;
	mnTimerInterval = 2000;	//1000ms

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
	//ui.policyDescribe->setText(QStringLiteral("均衡交通，是正常时间使用的控制策略"));
	ui.policySelection->setEnabled(0);
	ui.policyDescribe->setText(QStringLiteral(
		"调度策略仅适用于现实调度模拟，不适用于简单调度。"));
	ui.simulateProcess->setRange(0, 100);
	ui.simulateProcess->setValue(0);
	ui.ARC->setText(QString::number(0));
	ui.ATD->setText(QString::number(0));
	ui.AWD->setText(QString::number(0));

	mpVerticalLayoutList = nullptr;

	mpEachFloorWaitingPassengerNum = nullptr;

	//参数
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
	//创建hbox容器
	mpVerticalLayoutList = new QVBoxLayout[mnElevatorNum]();
	//创建lable
	for (int i = 0; i < mnElevatorNum; i++)
	{
		QLabel* _label;
		mpElevatorVerticalLayoutList.append(_label);
		mpElevatorVerticalLayoutList[i] = new QLabel[mnMaxFloorHeight]();
	}
	//调整label
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
	//完成布局
	//ui.simulateGraph->setSpacing(8);
	for (int i = 0; i < mnElevatorNum; i++)
	{
		ui.simulateGraph->addLayout(&(mpVerticalLayoutList[i]));
	}
	return 1;
}

int MCES::simulate_passenger_request()
{
	int _exist = rand() % 20;
	if (_exist > 9)
	{
		return 0;
	}
	else
	{
		int _num = rand() % 2;
		int _cnt = _num;
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
			if(mnSimulateModel==EASY)
				_r.passengerNum = (rand() % 12) + 1;
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
	return 0;
}
int MCES::uppeak_elevator_select(request _r, int excep)
{
	//所有电梯在执行完任务都回到一楼
	if (_r.status == UP)
	{
		
	}
	return 0;
}
int MCES::downpeak_elevator_select(request _r, int excep)
{
	//所有电梯都到达顶层再开始向下运行
	return 0;
}
int MCES::twoway_elevator_select(request _r, int excep)
{
	return 0;
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
			//if (!mvecElevatorVec[_elevatorSelected].access_request(cur_r))
				//_elevatorSelected = balanced_elevator_select(cur_r, _elevatorSelected);
			break;
		}
		case POLICY_UPPEAK:
		{
			_elevatorSelected = uppeak_elevator_select(cur_r);
			//if (!mvecElevatorVec[_elevatorSelected].access_request(cur_r))
				//_elevatorSelected = uppeak_elevator_select(cur_r, _elevatorSelected);
			break;
		}
		case POLICY_DOWNPEAK:
		{
			_elevatorSelected = downpeak_elevator_select(cur_r);
			//if (!mvecElevatorVec[_elevatorSelected].access_request(cur_r))
				//_elevatorSelected = downpeak_elevator_select(cur_r, _elevatorSelected);
			break;
		}
		case POLICY_TWOWAY:
		{
			_elevatorSelected = twoway_elevator_select(cur_r);
			//if (!mvecElevatorVec[_elevatorSelected].access_request(cur_r))
				//_elevatorSelected = twoway_elevator_select(cur_r, _elevatorSelected);
			break;
		}
		default:
			break;
		}
		mvecElevatorVec[_elevatorSelected].add_request(cur_r);

		mqueRequestList.pop();
	}
	refresh_simulate_graph();
}

int MCES::easy_elevator_select(request _r, int excep)
{
	//找最近的电梯进行承载
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
void MCES::EASY_request_respond()
{
	//int cnt = 0;
	while (!mqueRequestList.empty())
	{
		//cnt++;
		auto cur_r = mqueRequestList.front();
		int _elevatorSelected = rand() % mnElevatorNum;
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
		mpElevatorVerticalLayoutList[i][6].setText(QString::number(mvecElevatorVec[i].mqueRequestQueue.size()));
		mpElevatorVerticalLayoutList[i][mnMaxFloorHeight-1].setText(QString::number(mvecElevatorVec[i].mnCurFloor));
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnCurFloor].setText(mvecElevatorVec[i].generate_info_display());
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnCurFloor].setStyleSheet("border: 2px solid black;");
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnLastFloor].setText("");
		mpElevatorVerticalLayoutList[i][mvecElevatorVec[i].mnLastFloor].setStyleSheet("border: 1px dashed black;");
	}
}

//选择策略
void MCES::on_policySelection_currentIndexChanged(const int index)
{
	switch (index)
	{
	case 0:
		ui.policyDescribe->setText(
			QStringLiteral("均衡交通，是正常时间使用的控制策略"));
		mnSimulatePolicy = POLICY_BALANCED;
		break;
	case 1:
		ui.policyDescribe->setText(
			QStringLiteral("绝大部分乘客要上楼，一般用于上班时间"));
		mnSimulatePolicy = POLICY_UPPEAK;
		break;
	case 2:
		ui.policyDescribe->setText(
			QStringLiteral("绝大部分乘客要下楼，一般用于下班时间"));
		mnSimulatePolicy = POLICY_DOWNPEAK;
		break;
	case 3:
		ui.policyDescribe->setText(
			QStringLiteral("乘客集中前往某一楼层，一般用于开会、吃饭时间"));
		srand((unsigned)time(NULL));
		mnTwoWayTargetFloor = rand() % mnMaxFloorHeight;
		mnSimulatePolicy = POLICY_TWOWAY;
		break;
	default:
		ui.policyDescribe->setText(
			QStringLiteral("平衡状态：均衡交通，是正常时间使用的控制策略"));
		mnSimulatePolicy = POLICY_BALANCED;
		break;
	}
	//RealPolicySimulationSystem::set_policy(index);
}

//选择模拟算法
void MCES::on_modelSelection_currentIndexChanged(const int index)
{
	//切换到简单算法
	if (index == EASY)
	{
		ui.policySelection->setEnabled(0);
		ui.policyDescribe->setText(QStringLiteral(
			"调度策略仅适用于现实调度模拟，不适用于简单调度。"));
		mnSimulateModel = EASY;
	}
	//切换到现实模拟算法
	else if (index == REAL)
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
		mnSimulateModel = REAL;
	}
}

//点击 开始模拟
void MCES::on_startSimulateButton_clicked()
{
	//参数更新
	on_cabinVolume_textChanged();
	on_elevatorNum_textChanged();
	on_maxHeight_textChanged();
	on_simulateDuration_textChanged();
	
	srand((unsigned)time(NULL));

	//状态栏更新
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
	ui.currentStatus->setText(QStringLiteral("正在运行"));

	if (mvecElevatorVec.empty())
	{
		//创建电梯
		for (int i = 0; i < mnElevatorNum; i++)
		{
			elevator _ele(mnCabinVolume, i);
			_ele.mnCurPolicy = mnSimulatePolicy;
			mvecElevatorVec.push_back(_ele);
		}

		initialize_simulate_graph();

		mpEachFloorWaitingPassengerNum = new int[mnMaxFloorHeight];
	}

	mctrlBarTimer->start();
	mctrlCountTimer->start();
	mctrlCallTimer->start();
	mctrlStaticTimer->start();
}
//点击 暂停模拟
void MCES::on_pauseSimulateButton_clicked()
{
	mctrlBarTimer->stop();
	mctrlCountTimer->stop();
	mctrlCallTimer->stop();
	mctrlStaticTimer->stop();
	ui.currentStatus->setText(QStringLiteral("已暂停"));
}

//进度条和计时器配合
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
		ui.currentStatus->setText(QStringLiteral("已结束"));
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
	int AWD = 0, ATD = 0, ARC = 0;
	for (int i = 0; i < mnElevatorNum; i++)
	{
		mvecElevatorVec[i].refresh_static_info();
		AWD += mvecElevatorVec[i].mnGoPickDuration;
		ATD += mvecElevatorVec[i].mnGoDestDuration;
		ARC += mvecElevatorVec[i].mnCurLoad;
	}
	ARC *= 100;
	AWD /= mnElevatorNum;
	ATD /= mnElevatorNum;
	ARC /= 100 * mnCabinVolume * mnElevatorNum;
	ui.AWD->setText(QString::number(AWD));
	ui.ATD->setText(QString::number(ATD));
	ui.ARC->setText(QString::number(ARC) + "%");
}

//参数更新
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