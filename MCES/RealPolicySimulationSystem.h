#pragma once
#include"elevator.hpp"

/// ���ֲ���
/// ƽ�⽻ͨ ��������²���
/// ���и߷�/���и߷� �˿ͼ�����¥/��¥
/// ��·��ͨ �˿ͼ���ǰ��ĳһ��
 
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