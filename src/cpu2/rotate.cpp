#include <gameboy.hpp>
#include <globals.hpp>

void		GameboyCpu::rlca(void)
{
	unsigned char		a;
	bool				flag;

	a = (this->_af & 0xFF00) >> 8;
	flag = a & 0x8;
	this->_af = ((a << 1) & flag) << 8;

	this->setFlagZ(false);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc++;
	this->waitCycles(1);
}

void		GameboyCpu::rla(void)
{
	unsigned char		a;
	bool				flag_a;
	bool				flag_cy;

	a = (this->_af & 0xFF00) >> 8;
	flag_a = a & 0x8;
	flag_cy = this->getFlagC();
	this->_af = ((a << 1) & flag_cy) << 8;

	this->setFlagZ(false);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag_a);

	this->_pc++;
	this->waitCycles(1);
}

void		GameboyCpu::rrca(void)
{
	unsigned char		a;
	bool				flag;

	a = (this->_af & 0xFF00) >> 8;
	flag = a & 0x1;
	this->_af = ((a >> 1) & flag) << 8;

	this->setFlagZ(false);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc++;
	this->waitCycles(1);
}

void		GameboyCpu::rra(void)
{
	unsigned char		a;
	bool				flag_a;
	bool				flag_cy;

	a = (this->_af & 0xFF00) >> 8;
	flag_a = a & 0x1;
	flag_cy = this->getFlagC();
	this->_af = ((a >> 1) & flag_cy) << 8;

	this->setFlagZ(false);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag_a);

	this->_pc++;
	this->waitCycles(1);
}

void		GameboyCpu::rlc(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned short	value = 0;
	unsigned char	*reg = NULL;
	bool			flag;

	if (*code == 0x00)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x01)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x02)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x03)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x04)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x05)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x06)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x07)
		reg = REGISTER_8(&this->_af, 0);	

	value = *reg;
	flag = value & (1 << 7);
	*reg = (*reg << 1) | flag;

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc += 2;
	this->waitCycles(cycles);
}

void		GameboyCpu::rrc(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned short	value = 0;
	unsigned char	*reg = NULL;
	bool			flag;

	if (*code == 0x08)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x09)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x0A)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x0B)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x0C)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x0D)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x0E)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x0F)
		reg = REGISTER_8(&this->_af, 0);

	value = *reg;
	flag = value & 1;
	*reg = (*reg >> 1) | (flag << 7);

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc += 2;
	this->waitCycles(cycles);
}

void		GameboyCpu::rl(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned short	value = 0;
	unsigned char	*reg = NULL;
	bool			flag;

	if (*code == 0x10)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x11)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x12)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x13)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x14)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x15)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x16)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x17)
		reg = REGISTER_8(&this->_af, 0);	

	value = *reg;
	flag = value & (1 << 7);
	*reg = (*reg << 1) | this->getFlagC();

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc += 2;
	this->waitCycles(cycles);
}

void		GameboyCpu::rr(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned short	value = 0;
	unsigned char	*reg = NULL;
	bool			flag;

	if (*code == 0x08)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x09)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x0A)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x0B)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x0C)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x0D)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x0E)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x0F)
		reg = REGISTER_8(&this->_af, 0);

	value = *reg;
	flag = value & 1;
	*reg = (*reg >> 1) | (this->getFlagC() << 7);

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc += 2;
	this->waitCycles(cycles);
}
