#include <gameboy.hpp>
#include <globals.hpp>

void	GameboyCpu::inc16(unsigned short *reg)
{
	(*reg)++;
	this->_pc++;
	this->waitCycles(8);
}

void	GameboyCpu::inc8(unsigned short *reg, bool access)
{
	unsigned short value = access ? *(gbram + *reg) + 1 : (*reg >> 8) + 1;

	if (!access || (access && this->accessRam(*reg, access)))
	{
		if (access)
			*(gbram + *reg) = value << 8;
		else
			*reg = value << 8;

		this->calcFlagZ(value);
		this->calcFlagN(false);
		this->calcFlagH(value);
	}

	this->_pc++;
	this->waitCycles(access ? 12 : 4);
}

void	GameboyCpu::dec16(unsigned short *reg)
{
	(*reg)--;
	this->_pc++;
	this->waitCycles(8);
}

void	GameboyCpu::dec8(unsigned short *reg, bool access)
{
	unsigned short value = access ? *(gbram + *reg) - 1 : (*reg >> 8) - 1;

	if (!access || (access && this->accessRam(*reg, access)))
	{
		if (access)
			*(gbram + *reg) = value << 8;
		else
			*reg = value << 8;

		this->calcFlagZ(value);
		this->calcFlagN(true);
		this->calcFlagH(value);
	}

	this->_pc++;
	this->waitCycles(access ? 12 : 4);
}

void	GameboyCpu::add8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) + value;
	this->_af = (value << 8) & 0xFF00;
	this->calcFlagZ(value);
	this->setFlagN(false);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::add8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) + value;
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::add16_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = this->_sp + value;
	this->_sp = value;

	this->calcFlagZ(false);
	this->setFlagN(false);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc += 2;
	this->waitCycles(16);
}

void	GameboyCpu::add16(unsigned short b)
{
	unsigned short	value = this->_hl + b;

	this->_hl = value;
	this->setFlagN(false);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc++;
	this->waitCycles(8);
}

void	GameboyCpu::adc8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) + value + this->getFlagC();
	this->_af = (value << 8) & 0xFF00;
	this->calcFlagZ(value);
	this->setFlagN(false);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::adc8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) + value + this->getFlagC();
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::sub8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) - value;
	this->_af = (value << 8) & 0xFF00;

	this->calcFlagZ(value);
	this->setFlagN(true);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::sub8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) - value;
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(true);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::sbc8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) - value - this->getFlagC();
	this->_af = (value << 8) & 0xFF00;

	this->calcFlagZ(value);
	this->setFlagN(true);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::sbc8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) - value - this->getFlagC();
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(true);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::and8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) & value;
	this->_af = (value << 8) & 0xFF00;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->setFlagH(true);
	this->setFlagC(false);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::and8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) & value;
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->setFlagH(true);
	this->setFlagC(false);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::or8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) | value;
	this->_af = (value << 8) & 0xFF00;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(false);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::or8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) | value;
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(false);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::xor8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) ^ value;
	this->_af = (value << 8) & 0xFF00;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(false);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::xor8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) ^ value;
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(false);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::cp8(unsigned char *code)
{
	unsigned short	value = 0;
	unsigned char	cycles;

	cycles = 4;
	if (*code == 0x80)
		value = (this->_bc & 0xFF00) >> 8;
	else if (*code == 0x81)
		value = (this->_bc) & 0xFF;
	else if (*code == 0x82)
		value = (this->_de & 0xFF00) >> 8;
	else if (*code == 0x83)
		value = (this->_de) & 0xFF;
	else if (*code == 0x84)
		value = (this->_hl & 0xFF00) >> 8;
	else if (*code == 0x85)
		value = (this->_hl & 0xFF);
	else if (*code == 0x86)
	{
		cycles = 8;
		value = *(gbram + this->_hl);
	}
	else if (*code == 0x87)
		value = (this->_af & 0xFF00) >> 8;

	value = ((this->_af & 0xFF00) >> 8) - value;

	this->calcFlagZ(value);
	this->setFlagN(true);
	this->setFlagH(value);
	this->setFlagC(value);

	this->_pc++;
	this->waitCycles(cycles);
}

void	GameboyCpu::cp8_data(unsigned char *code)
{
	unsigned short	value = 0;

	memcpy(&value, code, 1);
	value = ((this->_af & 0xFF00) >> 8) - value;
	this->_af = value << 8;

	this->calcFlagZ(value);
	this->setFlagN(true);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::cpl8(void)
{
	unsigned char	value = 0;

	value = ~((this->_af & 0xFF00) >> 8);
	this->_af = (value << 8) | (this->_af & 0x00FF);

	this->setFlagN(true);
	this->setFlagH(true);

	this->_pc++;
	this->waitCycles(4);
}
