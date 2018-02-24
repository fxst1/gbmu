#include <gameboy.hpp>
#include <globals.hpp>

void	GameboyCpu::ld16_sp_hl(void)
{
	this->_sp = this->_hl;
	this->_pc++;
	this->waitCycles(8);
}

void	GameboyCpu::ld16_hl(unsigned char* code)
{
	unsigned short		value;

	memcpy(&value, code + 1, 2);
	value += this->_sp;
	this->_hl = value;

	this->setFlagZ(false);
	this->setFlagN(false);
	this->calcFlagH(value);
	this->calcFlagC(value);

	this->_pc += 2;
	this->waitCycles(12);
}

void	GameboyCpu::ld16_address(unsigned char *code)
{
	unsigned short		value;

	memcpy(&value, code + 1, 2);
	value = *(code + 1) + 0xFF00;
	if (this->accessRam(value, false))
	{
		*(gbram + value) =  (this->_sp & 0xFF);
		*(gbram + value + 1) =  ((this->_sp & 0xFF00) >> 8);
	}
	this->_pc += 3;
	this->waitCycles(20);
}

void	GameboyCpu::ld16_read(unsigned char *code)
{
	unsigned short		value;

	memcpy(&value, code + 1, 2);
	if (this->accessRam(value, true))
		this->_af = (*(gbram + value) << 8) | (this->_af & 0x00FF);
	this->_pc += 3;
	this->waitCycles(16);
}

void	GameboyCpu::ld16_write(unsigned char *code)
{
	unsigned short		value;

	memcpy(&value, code + 1, 2);
	if (this->accessRam(value, false))
		*(gbram + value) =  ((this->_af & 0xFF00) >> 8);
	this->_pc += 3;
	this->waitCycles(16);
}

void	GameboyCpu::ld16_in(unsigned char *code)
{
	unsigned short		value;

	value = *(code + 1) + 0xFF00;
	if (this->accessRam(value, true))
		*(gbram + value) =  ((this->_af & 0xFF00) >> 8);
	this->_pc += 2;
	this->waitCycles(12);
}

void	GameboyCpu::ld16_out(unsigned char *code)
{
	unsigned char		value;

	value = *(code + 1) + 0xFF00;
	if (this->accessRam(value, false))
		this->_af = (*(gbram + value)) | (this->_af & 0xFF);
	this->_pc += 2;
	this->waitCycles(12);
}

void	GameboyCpu::ld16(unsigned short *reg, unsigned char *code)
{
	std::cout << "LD16" << std::endl;
	unsigned short		value;

	value = (*(code + 1) << 8) | (*(code + 2));
	*reg = value;
	this->_pc += 3;
	this->waitCycles(12);
}

void	GameboyCpu::ld8_data(unsigned short *reg, unsigned char* code, bool access)
{
	this->_pc += 2;
	if (!access)
	{
		memcpy(reg, code, 1);
		this->waitCycles(8);
	}
	else if (this->accessRam(*reg, false))
	{
		memcpy(gbram + *reg, code, 1);
		this->waitCycles(12);
	}
}

void	GameboyCpu::ld8_in(void)
{
	unsigned short		value;

	value = (this->_bc & 0x00FF) + 0xFF00;
	if (this->accessRam(value, true))
		*(gbram + value) =  ((this->_af >> 8) & 0xFF);
	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::ld8_out(void)
{
	unsigned char		value;

	value = (this->_bc & 0x00FF) + 0xFF00;
	if (this->accessRam(value, false))
		this->_af = (*(gbram + value)) | (this->_af & 0xFF);
	this->_pc += 2;
	this->waitCycles(8);
}

void	GameboyCpu::ld8_from_address(unsigned short *reg, int delta)
{
	if (accessRam(*reg, true))
		this->_af = (*reg << 8) | (this->_af & 0x00FF);
	*reg += delta;
	this->_pc++;
	this->waitCycles(8);
}

void	GameboyCpu::ld8_address(unsigned short *reg, int delta)
{
	if (accessRam(*reg, false))
		*(gbram + *reg) = (this->_af & 0xFF00) >> 8;
	*reg += delta;
	this->_pc++;
	this->waitCycles(8);
}

void	GameboyCpu::ld8(unsigned char *code)
{
	unsigned char	tmp = 0;
	unsigned char*	reg_a = &tmp;
	unsigned char*	reg_b = &tmp;
	unsigned short	cycles;

	cycles = 4;
	if (*code & 0x40)
		reg_a = (unsigned char*)&this->_bc + ((*code & 0x0F) >= 0x08);
	else if (*code & 0x50)
		reg_a = (unsigned char*)&this->_de + ((*code & 0x0F) >= 0x08);
	else if (*code & 0x60)
		reg_a = (unsigned char*)&this->_hl + ((*code & 0x0F) >= 0x08);
	else if (*code & 0x70)
	{
		if (*code >= 0x78)
			reg_a = (unsigned char*)&this->_hl;
		else
		{
			cycles = 8;
			reg_a = gbram + this->_hl;	
		}
	}

	tmp = *code >> 4;
	if (tmp == 0x0)
		reg_b = (unsigned char*)&this->_bc;
	else if (tmp == 0x1)
		reg_b = ((unsigned char*)&this->_bc) + 1;
	else if (tmp == 0x2)
		reg_b = (unsigned char*)&this->_de;
	else if (tmp == 0x3)
		reg_b = ((unsigned char*)&this->_de) + 1;
	else if (tmp == 0x4)
		reg_b = (unsigned char*)&this->_hl;
	else if (tmp == 0x5)
		reg_b = ((unsigned char*)&this->_hl) + 1;
	else if (tmp == 0x6)
	{
		cycles = 8;
		reg_b = gbram + this->_hl;
	}
	else if (tmp == 0x7)
		reg_b = ((unsigned char*)&this->_af);

	*reg_a = *reg_b;
	this->_pc++;
	this->waitCycles(cycles);
}
