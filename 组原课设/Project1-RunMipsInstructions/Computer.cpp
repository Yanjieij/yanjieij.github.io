#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include<sstream>
#include <iomanip>
#include "Computer.h"

using namespace std;

char g_str_instruction[50];
/*
 *  Return an initialized computer with the stack pointer set to the
 *  address of the end of data memory, the remaining registers initialized
 *  to zero, and the instructions read from the given file.
 *  The other arguments all govern how the program interacts with the user.
 */
SimMipsComputer::SimMipsComputer(FILE* filein, int printingRegisters, 
	int printingMemory, int debugging, int interactive) 
{
	memset(&mipsComupter, 0, sizeof(SimulatedComputer));
	int i;
	unsigned int instr;

	for (i = 0; i < 32; i++) 
		mipsComupter.registers[i] = 0;

	/* stack pointer starts at end of data area and grows down */
	mipsComupter.registers[29] = 0x00400000 + (MAX_NUM_INSTRS + MAX_NUM_DATA) * 4;

	i = 0;
	while (1) 
	{
		instr = 0;
		unsigned char temp = 0;
		int k;
		for (k = 0; k < 4; k++) 
		{
			if (!fread(&temp, 1, 1, filein)) 
			{
				i = -1;
				break;
			}
			instr |= temp << (k << 3);
		}
		if (i == -1) break;
		mipsComupter.memory[i] = instr;
		i++;

		if (i > MAX_NUM_INSTRS) 
		{
			fprintf(stderr, "Program too big (%d instructions, only %d allowed).\n", i, MAX_NUM_INSTRS);
			exit(1);
		}
	}

	mipsComupter.printingRegisters = printingRegisters;
	mipsComupter.printingMemory = printingMemory;
	mipsComupter.interactive = interactive;
	mipsComupter.debugging = debugging;
}


/*
 *  Return the contents of memory at the given address.
 */
unsigned int SimMipsComputer::GetMemoryContents(int addr) 
{
	int index = (addr - 0x00400000) / 4;
	if (((addr & 0x3) != 0) || (index < 0) 
		|| (index >= (MAX_NUM_INSTRS + MAX_NUM_DATA))) 
	{
		printf("Invalid Address: %8.8x", addr);
		exit(0);
	}

	return mipsComupter.memory[index];
}

/*
 *  Print relevant information about the state of the computer.
 *  changedReg is the index of the register changed by the instruction
 *  being simulated.  If changedReg is -1, no register was updated by
 *  the simulated instruction, i.e. it was a branch, a jump, or a store).
 *  changedMem is the address of the memory location changed by the
 *  simulated instruction.  For anything but a store, changedMem will be -1.
 *  Previously initialized flags indicate whether to print all the
 *  registers or just the one that changed, and whether to print
 *  all the nonzero memory or just the memory location that changed.
 */
void SimMipsComputer::PrintInfo(int changedReg, int changedMem) 
{
	int i, addr;
	printf("New pc = %8.8x\n", mipsComupter.pc);

	if (!mipsComupter.printingRegisters && changedReg == -1) 
		printf("No register was updated.\n");
	else if (!mipsComupter.printingRegisters) 
		printf("Updated r%d to %8.8x\n", changedReg, mipsComupter.registers[changedReg]);
	else 
	{
		for (i = 0; i < 32; i++) 
		{
			printf("r%2d: %8.8x  ", i, mipsComupter.registers[i]);
			if ((i + 1) % 4 == 0) printf("\n");
		}
	}

	if (!mipsComupter.printingMemory && changedMem == -1) 
		printf("No memory location was updated.\n");
	else if (!mipsComupter.printingMemory) 
		printf("Updated memory at address %8.8x to %8.8x\n", changedMem, GetMemoryContents(changedMem));
	else 
	{
		printf("Nonzero memory\n");
		printf("ADDR     CONTENTS\n");

		for (addr = 0x00400000 + (MAX_NUM_INSTRS << 2); 
			addr < 0x00400000 + ((MAX_NUM_INSTRS + MAX_NUM_DATA) << 2); 
			addr = addr + 4) 
		{
			if (GetMemoryContents(addr) != 0) 
				printf("%8.8x  %8.8x\n", addr, GetMemoryContents(addr));
		}
	}
	printf("\n");
}



/*
 *  Run the simulation.
 */
void SimMipsComputer::Simulate() 
{
	char s[10];  /* used for handling interactive input */
	unsigned int instr;
	int changedReg, changedMem;

	mipsComupter.pc = 0x00400000;
	while (1) 
	{
		if (mipsComupter.interactive) 
		{
			printf("> ");
			fgets(s, 10, stdin);
			if (s[0] == 'q') return;
		}

		instr = GetMemoryContents(mipsComupter.pc);
		printf("Executing instruction at %8.8x: %8.8x\n", mipsComupter.pc, instr);
		cout << "\t" << Disassemble(instr, mipsComupter.pc) << "\n";
		SimulateInstr(instr, &changedReg, &changedMem);
		PrintInfo(changedReg, changedMem);
	}
}


/*
 *  Return a string containing the disassembled version of the given
 *  instruction.  You absolutely MUST follow the formatting instructions
 *  given in the assignment.  Note that the string your return should
 *  not include a tab at the beginning or a newline at the end since
 *  those are added in Simulate() function where this function is called.
 */
string SimMipsComputer::Disassemble(unsigned int instr, unsigned int pc)
{
	// Your program must exit when an unsupported instruction is detected
	//if (/*instruction isn't supported */) exit (0); 

	//将指令转化为二进制，存储在biStr中
	int biStr[32];
	int _temp = instr;
	for (int i = 31; i >= 0; i--)
	{
		biStr[i] = _temp % 2;
		_temp = _temp / 2;
	}
	//将机器指令进行拆分，判断种类
	int opcode = 0;
	int type = -1; //0 for R,1 for I,2 for J
	for (int i = 0; i <= 5; i++)
		opcode += biStr[5 - i] * pow(2, i);
	if (!opcode)
		type = 0;
	else if (opcode == 3 || opcode == 2)
		type = 2;
	else
		type = 1;
	//根据不同类型，判断指令
	string output;
	switch (type)
	{
	case 0: //R
	{
		int rs = 0, rt = 0, rd = 0, shamt = 0, funct = 0;
		for (int i = 10; i >= 6; i--)
			rs += biStr[i] * pow(2, 10 - i);
		for (int i = 15; i >= 11; i--)
			rt += biStr[i] * pow(2, 15 - i);
		for (int i = 20; i >= 16; i--)
			rd += biStr[i] * pow(2, 20 - i);
		for (int i = 25; i >= 21; i--)
			shamt += biStr[i] * pow(2, 20 - i);
		for (int i = 31; i >= 26; i--)
			funct += biStr[i] * pow(2, 31 - i);
		//判断具体语句
		switch (funct)
		{
		case 33:
			output.append("addu");
			break;
		case 35:
			output.append("subu");
			break;
		case 0:
			output.append("sll");
			break;
		case 2:
			output.append("srl");
			break;
		case 36:
			output.append("and");
			break;
		case 37:
			output.append("or");
			break;
		case 42:
			output.append("slt");
			break;
		case 8:
			output.append("jr");
			break;
		default:
			exit(0);
		}
		//完善输出字符串
		if (funct == 0 || funct == 2)	//sll,srl
		{
			output.append("\t");
			output.append("$");
			output.push_back(rd + 48);
			output.append(", $");
			output.push_back(rt + 48);
			output.append(", ");
			output.push_back(shamt + 48);
		}
		else if (funct == 8)
		{
			stringstream ss;
			ss << hex << mipsComupter.registers[rs];
			output.append(ss.str());
			break;
		}
		else
		{
			output.append("\t");
			output.append("$");
			output.push_back(rd + 48);
			output.append(", $");
			output.push_back(rs + 48);
			output.append(", $");
			output.push_back(rt + 48);
		}
		break;
	}
	case 1: //I
	{
		int rs = 0, rt = 0;
		long imm = 0;
		for (int i = 10; i >= 6; i--)
			rs += biStr[i] * pow(2, 10 - i);
		for (int i = 15; i >= 11; i--)
			rt += biStr[i] * pow(2, 15 - i);
		for (int i = 31; i >= 17; i--)
		{
			imm += biStr[i] * pow(2, 31 - i);
			imm -= biStr[16] * pow(2, 15);
		}
		switch (opcode)
		{
		case 9:
			output.append("addiu");
			break;
		case 12:
			output.append("andi");
			break;
		case 13:
			output.append("ori");
			break;
		case 15:
			output.append("lui");
			break;
		case 4:
			output.append("beq");
			break;
		case 5:
			output.append("bne");
			break;
		default:
			exit(0);
		}
		if (opcode == 12 || opcode == 13 || opcode == 15)	//addi,ori,lui
		{
			output.append("\t");
			output.append("$");
			output.push_back(rt + 48);
			output.append(", $");
			output.push_back(rs + 48);
			output.append(", ");
			string hex("0x");
			for (int x = 0; x < 4; x++)
				for (int y = 16 + x * 4 + 3; y >= 16 + x * 4; y--)
				{
					int sum = 0;
					sum += biStr[y] * pow(2, 16 + x * 4 + 3 - y);
					if (sum >= 10)
						hex.push_back(sum + 87);
					else
						hex.push_back(sum + 48);
				}
			output.append(hex);
		}
		else if (opcode == 4 || opcode == 5)	//beq,bne
		{
			output.append("\t");
			output.append("$");
			output.push_back(rt + 48);
			output.append(", $");
			output.push_back(rs + 48);
			output.append(", ");
			stringstream ss;
			/*for (int x = 0; x < 8; x++)
				for (int y = 7 + x * 4 + 3; y >= 7 + x * 4; y--)
				{
					int sum = 0;
					sum += biStr[y] * pow(2, 16 + x * 4 + 3 - y);
					sum += pc;
					if (sum >= 10)
						hex.push_back(sum + 87);
					else
						hex.push_back(sum + 48);
				}*/
			long addr = pc + imm;
			ss << hex << addr;
			output.append(ss.str());
		}
		else    //addiu
		{
			if (imm < 0)	//unsigned
				imm = pow(2, 16) + imm;
			output.append("\t");
			output.append("$");
			output.push_back(rt + 48);
			output.append(", $");
			output.push_back(rs + 48);
			output.append(", ");
			stringstream ss;
			ss << imm;
			output.append(ss.str());
		}
	}
		break;
	case 2: //J
	{
		long addr = 0;
		for (int i = 31; i >= 7; i--)
		{
			addr += biStr[i] * pow(2, 31 - i);
		}
		switch (opcode)
		{
		case 2:	//j

		{
			output.append("j/t");
			stringstream ss;
			ss << hex << addr;
			output.append(ss.str());
			break;
		}
		case 3:	//jal
		{
			output.append("jal/t");
			stringstream ss;
			ss << hex << addr;
			output.append(ss.str());
			break;
		}
		default:
			exit(0);
		}
	}
	default: //ERROR!
		exit(0);
	}
	return output;
}

/*
 *  Simulate the execution of the given instruction, updating the
 *  pc appropriately.
 *
 *  If the instruction modified a register--i.e. if it was lw,
 *  addu, addiu, subu, sll, srl, and, andi, or, ori, lui, or slt
 *  to list a few examples-- return the index of the modified
 *  register in *changedReg, otherwise return -1 in *changedReg.
 *  Note that you should never return 0 in *changedReg, since
 *  $0 cannot be changed!  Note that even if the instruction
 *  changes the register back to it's old value
 *  (e.g. addu $3, $3, $0) the destination register ($3 in the
 *  example) should be marked changed!
 *
 *  If the instruction was sw, return the address of the
 *  updated memory location in *changedMem, otherwise return -1
 *  in *changedMem.
 */
void SimMipsComputer::SimulateInstr(unsigned int instr, int* changedReg, int* changedMem) {
	/* You replace this code by the right stuff. */
	mipsComupter.pc = mipsComupter.pc + 4;
	*changedReg = -1;
	*changedMem = -1;
}

