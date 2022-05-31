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
	for (int i = 0; i <= 4; i++)
		opcode += biStr[4 - i] * pow(2, i);
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
			output.append("/t");
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