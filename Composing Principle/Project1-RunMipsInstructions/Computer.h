#pragma once

#include <string>
using namespace std;

#undef mips								/* gcc already has a def for mips */

const int MAX_NUM_INSTRS = 1024;		/* max # instrs in a program */
const int MAX_NUM_DATA   = 3072;		/* max # data words */


// ģ�������Ļ���Ӳ����ɲ��֣�����32��ͨ�üĴ���
// ָ���ڴ�������ڴ棬pc�Ĵ�����
// 4����ǣ��Ƿ��ӡ�Ĵ������Ƿ��ӡ�ڴ棬�Ƿ񽻻����У��Ƿ��������
struct SimulatedComputer {
	int memory[MAX_NUM_INSTRS + MAX_NUM_DATA];
	int registers[32];
	int pc;
	int printingRegisters, printingMemory, interactive, debugging;
};


class SimMipsComputer
{
public:
	SimMipsComputer(FILE*, int printingRegisters, int printingMemory, 
		int debugging, int interactive);

	unsigned int GetMemoryContents(int addr);
	void PrintInfo(int changedReg, int changedMem);
	void Simulate();

	// ģ��һ��MIPSָ���ִ��
	void SimulateInstr(unsigned int instr, int* changedReg, int* changedMem);
	// �Ի�������ָ����з���࣬���ػ������ָ���ַ���
	string Disassemble(unsigned int instr, unsigned int pc);

private:
	SimulatedComputer mipsComupter;
};
