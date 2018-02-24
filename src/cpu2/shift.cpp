#include <gameboy.hpp>
#include <globals.hpp>

void		GameboyCpu::swap(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned char	*reg = NULL;

	if (*code == 0x30)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x31)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x32)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x33)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x34)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x35)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x36)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x37)
		reg = REGISTER_8(&this->_af, 0);	

	*reg = ((*reg & 0x0F) << 4) | ((*reg & 0xF0) >> 4);

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(false);

	this->_pc += 2;
	this->waitCycles(cycles);
}


void		GameboyCpu::sla(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned short	value = 0;
	unsigned char	*reg = NULL;
	bool			flag;

	if (*code == 0x20)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x21)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x22)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x23)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x24)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x25)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x26)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x27)
		reg = REGISTER_8(&this->_af, 0);	

	value = *reg;
	flag = value & (1 << 7);
	*reg = (*reg << 1);

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc += 2;
	this->waitCycles(cycles);
}

void		GameboyCpu::sra(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned short	value = 0;
	unsigned char	*reg = NULL;
	bool			flag;

	if (*code == 0x28)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x29)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x2A)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x2B)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x2C)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x2D)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x2E)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x2F)
		reg = REGISTER_8(&this->_af, 0);

	value = *reg;
	flag = value & 1;
	*reg = (*reg >> 1) | (*reg & (1 << 7));

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc += 2;
	this->waitCycles(cycles);
}

void		GameboyCpu::srl(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned short	value = 0;
	unsigned char	*reg = NULL;
	bool			flag;

	if (*code == 0x38)
		reg = REGISTER_8(&this->_bc, 0);
	else if (*code == 0x39)
		reg = REGISTER_8(&this->_bc, 1);
	else if (*code == 0x3A)
		reg = REGISTER_8(&this->_de, 0);
	else if (*code == 0x3B)
		reg = REGISTER_8(&this->_de, 1);
	else if (*code == 0x3C)
		reg = REGISTER_8(&this->_hl, 0);
	else if (*code == 0x3D)
		reg = REGISTER_8(&this->_hl, 1);
	else if (*code == 0x3E)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (*code == 0x3F)
		reg = REGISTER_8(&this->_af, 0);

	value = *reg;
	flag = value & 1;
	*reg = (*reg >> 1);

	this->calcFlagZ(*reg);
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(flag);

	this->_pc += 2;
	this->waitCycles(cycles);
}
