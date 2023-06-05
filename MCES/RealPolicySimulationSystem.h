#pragma once
#include"elevator.hpp"

/// 四种策略
/// 平衡交通 正常情况下采用
/// 上行高峰/下行高峰 乘客集中上楼/下楼
/// 二路交通 乘客集中前往某一层
 
#define POLICY_BALANCED 0
#define POLICY_UPPEAK 1
#define POLICY_DOWNPEAK 2
#define POLICY_TWOWAY 3

class RealPolicySimulationSystem
{
public:
	RealPolicySimulationSystem();
	~RealPolicySimulationSystem();
private:
	int mnCurrentPolicy;
};