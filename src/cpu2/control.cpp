#include <gameboy.hpp>
#include <globals.hpp>

void	GameboyCpu::daa(void)
{
	unsigned char	old_a = (this->_af & 0xFF00) >> 8;
	bool			old_cf = false;

	this->setFlagC(false);
	if (((old_a & 0x0F) > 9) || this->getFlagH())
	{
		this->_af += 0x0600;
		this->calcFlagC((this->_af & 0xFF00) >> 8);
		this->setFlagC(old_cf || this->getFlagC());
		this->setFlagH(true);
	}
	else
		this->setFlagH(true);

	if ((old_a > 0x99) || old_cf)
	{
		this->_af += 0x6000;
		this->setFlagC(true);
	}
	else
		this->setFlagC(false);

	this->setFlagZ((this->_af & 0xFF00) >> 8);
	this->setFlagH(false);
	this->setFlagC((this->_af & 0xFF00) >> 8);
	this->_pc++;
	this->waitCycles(4);
}

void	GameboyCpu::ccf(void)
{
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(!this->getFlagC());

	this->_pc++;
	this->waitCycles(4);
}

void	GameboyCpu::scf(void)
{
	this->setFlagN(false);
	this->setFlagH(false);
	this->setFlagC(true);

	this->_pc++;
	this->waitCycles(4);
}

void	GameboyCpu::ei(void)
{
	this->_ime = true;
	this->_pc++;
	this->waitCycles(1);
}

void	GameboyCpu::di(void)
{
	this->_ime = false;
	this->_pc++;
	this->waitCycles(1);
}

void	GameboyCpu::push(unsigned short value)
{
	*(gbram + (this->_sp - 1)) = (value >> 8) & 0xFF;
	*(gbram + (this->_sp - 2)) = value & 0xFF;
	this->_sp -= 2;
	this->_pc++;
	this->waitCycles(16);
}

void	GameboyCpu::pop(unsigned short* value)
{
	unsigned short	tmp = 0;

	tmp = *(gbram + this->_sp) & 0xFF;
	tmp |= (*(gbram + this->_sp + 1) << 8);
	*value = tmp;
	this->_sp += 2;
	if (&this->_af == value)
	{
		this->calcFlagZ(*value);
		this->calcFlagN(*value);
		this->calcFlagH(*value);
		this->calcFlagC(*value);
	}
	this->_pc++;
	this->waitCycles(12);
}

void	GameboyCpu::reti(void)
{
	this->_pc++;
	this->_pc = ((*(gbram + this->_sp + 1)) << 8) | *(gbram + this->_sp);
	this->_sp += 2;
	this->waitCycles(16);
}

void	GameboyCpu::ret(void)
{
	this->_pc++;
	this->_pc = ((*(gbram + this->_sp + 1)) << 8) | *(gbram + this->_sp);
	this->_sp += 2;
	this->waitCycles(16);
}

void	GameboyCpu::ret(unsigned char ftype)
{
	this->_pc++;
	if (this->matchFlag(ftype))
	{
		this->_pc = ((*(gbram + this->_sp + 1)) << 8) | *(gbram + this->_sp);
		this->_sp += 2;
		this->waitCycles(20);
	}
	else
		this->waitCycles(8);
}

void	GameboyCpu::rst(unsigned char code)
{
	*(gbram + this->_sp - 1) = (this->_pc & 0xFF00) >> 8;
	*(gbram + this->_sp - 2) = (this->_pc & 0xFF);

	this->_sp -= 2;
	this->_pc = code;
	this->waitCycles(4);
}

void	GameboyCpu::call(unsigned char* code)
{
	this->_pc += 3;
	*(gbram + this->_sp - 1) = (this->_pc & 0xFF00) >> 8;
	*(gbram + this->_sp - 2) = (this->_pc & 0xFF);
	this->_pc = ((*(gbram + this->_sp + 1)) << 8) | *(gbram + this->_sp);
	memcpy(&this->_pc, code + 1, 2);
	this->_sp -= 2;
	this->waitCycles(24);
}

void	GameboyCpu::call(unsigned char ftype, unsigned char* code)
{
	this->_pc += 3;
	if (this->matchFlag(ftype))
	{
		*(gbram + this->_sp - 1) = (this->_pc & 0xFF00) >> 8;
		*(gbram + this->_sp - 2) = (this->_pc & 0xFF);
		this->_pc = ((*(gbram + this->_sp + 1)) << 8) | *(gbram + this->_sp);
		memcpy(&this->_pc, code + 1, 2);
		this->_sp -= 2;
		this->waitCycles(24);
	}
	else
		this->waitCycles(12);
}

void	GameboyCpu::jp(unsigned char ftype, unsigned char *code)
{
	unsigned short	offset = 0;

	this->_pc += 3;
	memcpy(&offset, code + 1, 2);
	if (this->matchFlag(ftype))
	{
		this->_pc = offset;
		this->waitCycles(16);
	}
	else
		this->waitCycles(8);
}

void	GameboyCpu::jp(unsigned char *code)
{
	unsigned short	offset = 0;

	this->_pc += 3;
	memcpy(&offset, code + 1, 2);
	this->_pc = offset;
	this->waitCycles(16);
}

void	GameboyCpu::jp(void)
{
	if (this->accessRam(this->_hl, true))
	{

	}

	memcpy(&this->_pc, gbram + this->_hl, 2);
	this->waitCycles(4);
}

void	GameboyCpu::jr(unsigned char ftype, unsigned char* code)
{
	signed char	offset = *((signed char*)(code + 1));

	printf("JR %d %x %x %x\n", (int)offset, *code, *(code + 1), *(code + 2));

	this->_pc += 2;
	if (this->matchFlag(ftype))
	{
		this->_pc += offset;
		this->waitCycles(12);
	}
	else
		this->waitCycles(8);
}

void	GameboyCpu::jr(unsigned char *code)
{
	this->_pc += 2;
	this->_pc += *((signed char*)code + 1);
	this->waitCycles(12);
}


void	GameboyCpu::stop(void)
{
	this->_pc += 2;
	this->waitCycles(4);
}

void	GameboyCpu::nop(void)
{
	this->_pc++;
	this->waitCycles(4);
}

void	GameboyCpu::halt(void)
{
	this->_pc++;
	this->waitCycles(4);
}
