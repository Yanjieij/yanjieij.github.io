#include "MCES.h"

MCES::MCES(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//初始化工作
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
	ui.policyDescribe->setText(QStringLiteral("均衡交通，是正常时间使用的控制策略"));
	ui.simulateProcess->setRange(0, 100);
	ui.simulateProcess->setValue(0);

	mpVerticalLayoutList = nullptr;

	mpEachFloorWaitingPassengerNum = nullptr;
	mpEachFloorRequestStatus = NONE;

	//参数
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
	//创建电梯
	for (int i = 0; i < mnElevatorNum; i++)
	{
		elevator _ele(mnCabinVolume, i);
		mvecElevatorVec.push_back(_ele);
	}
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
			mpVerticalLayoutList[i].addWidget(&(mpElevatorVerticalLayoutList[i][cnt]));
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
	//找最近的电梯进行承载
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
	//切换到蚁群算法
	if (index == ACO)
	{
		ui.policySelection->setEnabled(0);
		ui.policyDescribe->setText(QStringLiteral(
			"调度策略仅适用于现实调度模拟，不适用于蚁群调度。"));
		mnSimulateModel = ACO;
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
	
	initialize_simulate_graph();

	mpEachFloorRequestStatus = new int[mnMaxFloorHeight];
	mpEachFloorWaitingPassengerNum = new int[mnMaxFloorHeight];

	mctrlBarTimer->start();
	mctrlCountTimer->start();
}
//点击 暂停模拟
void MCES::on_pauseSimulateButton_clicked()
{
	mctrlBarTimer->stop();
	mctrlCountTimer->stop();
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
		ui.currentStatus->setText(QStringLiteral("已结束"));
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