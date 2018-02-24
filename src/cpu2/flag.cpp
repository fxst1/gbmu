#include <gameboy.hpp>
#include <globals.hpp>

void			GameboyCpu::setFlagZ(bool f)
{
	if (f)
		this->_af |= 1 << 7;
	else
		this->_af &= ~(1 << 7);
}

void			GameboyCpu::setFlagN(bool f)
{
	if (f)
		this->_af |= 1 << 6;
	else
		this->_af &= ~(1 << 6);
}

void			GameboyCpu::setFlagH(bool f)
{
	if (f)
		this->_af |= 1 << 5;
	else
		this->_af &= ~(1 << 5);
}

void			GameboyCpu::setFlagC(bool f)
{
	if (f)
		this->_af |= 1 << 4;
	else
		this->_af &= ~(1 << 4);
}

bool			GameboyCpu::getFlagZ(void)
{
	return (this->_af & (1 << 7));
}

bool			GameboyCpu::getFlagN(void)
{
	return (this->_af & (1 << 6));
}

bool			GameboyCpu::getFlagH(void)
{
	return (this->_af & (1 << 5));
}

bool			GameboyCpu::getFlagC(void)
{
	return (this->_af & (1 << 4));
}

void			GameboyCpu::calcFlagZ(unsigned short value)
{
	this->setFlagZ(value == 0);
}

void			GameboyCpu::calcFlagN(unsigned short value)
{
	this->setFlagN(((signed short)value) > 0);
}

void			GameboyCpu::calcFlagH(unsigned short value)
{
	this->setFlagN(value >= (1 << 3));
}

void			GameboyCpu::calcFlagC(unsigned short value)
{
	this->setFlagN(value >= (1 << 7));
}

bool			GameboyCpu::matchFlag(unsigned char ftype)
{
	if (ftype == GameboyCpu::FLAG_C)
		return (this->getFlagC());
	else if (ftype == GameboyCpu::FLAG_Z)
		return (this->getFlagZ());
	else if (ftype == GameboyCpu::FLAG_NC)
		return (!this->getFlagC());
	else if (ftype == GameboyCpu::FLAG_NZ)
		return (!this->getFlagZ());
	return (false);
}
