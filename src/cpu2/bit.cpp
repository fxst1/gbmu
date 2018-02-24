#include <gameboy.hpp>
#include <globals.hpp>

void		GameboyCpu::bit(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned char	*reg = NULL;
	unsigned char	bit = 0;

	bit = *code & 0x7;
	if (bit == 0x00)
		reg = REGISTER_8(&this->_bc, 0);
	else if (bit == 0x01)
		reg = REGISTER_8(&this->_bc, 1);
	else if (bit == 0x02)
		reg = REGISTER_8(&this->_de, 0);
	else if (bit == 0x03)
		reg = REGISTER_8(&this->_de, 1);
	else if (bit == 0x04)
		reg = REGISTER_8(&this->_hl, 0);
	else if (bit == 0x05)
		reg = REGISTER_8(&this->_hl, 1);
	else if (bit == 0x06)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (bit == 0x7)
		reg = REGISTER_8(&this->_af, 0);	

	bit = *reg & (1 << ((*code & 0x38) >> 3));

	this->setFlagZ(!!bit);
	this->setFlagN(false);
	this->setFlagH(true);

	this->_pc += 2;
	this->waitCycles(cycles);
}

void		GameboyCpu::set(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned char	*reg = NULL;
	unsigned char	bit = 0;

	bit = *code & 0x7;
	if (bit == 0x00)
		reg = REGISTER_8(&this->_bc, 0);
	else if (bit == 0x01)
		reg = REGISTER_8(&this->_bc, 1);
	else if (bit == 0x02)
		reg = REGISTER_8(&this->_de, 0);
	else if (bit == 0x03)
		reg = REGISTER_8(&this->_de, 1);
	else if (bit == 0x04)
		reg = REGISTER_8(&this->_hl, 0);
	else if (bit == 0x05)
		reg = REGISTER_8(&this->_hl, 1);
	else if (bit == 0x06)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (bit == 0x7)
		reg = REGISTER_8(&this->_af, 0);

	*reg |= (1 << ((*code & 0x38) >> 3));

	this->_pc += 2;
	this->waitCycles(cycles);
}

void		GameboyCpu::res(unsigned char* code)
{
	unsigned int	cycles = 8;
	unsigned char	*reg = NULL;
	unsigned char	bit = 0;

	bit = *code & 0x7;
	if (bit == 0x00)
		reg = REGISTER_8(&this->_bc, 0);
	else if (bit == 0x01)
		reg = REGISTER_8(&this->_bc, 1);
	else if (bit == 0x02)
		reg = REGISTER_8(&this->_de, 0);
	else if (bit == 0x03)
		reg = REGISTER_8(&this->_de, 1);
	else if (bit == 0x04)
		reg = REGISTER_8(&this->_hl, 0);
	else if (bit == 0x05)
		reg = REGISTER_8(&this->_hl, 1);
	else if (bit == 0x06)
	{
		cycles = 16;
		this->accessRam(this->_hl, true);
		reg = (gbram + this->_hl);
	}
	else if (bit == 0x7)
		reg = REGISTER_8(&this->_af, 0);

	*reg &= ~(1 << ((*code & 0x38) >> 3));

	this->_pc += 2;
	this->waitCycles(cycles);
}
