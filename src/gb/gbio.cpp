#include <gameboy.hpp>
#include <globals.hpp>

GameboyIO::GameboyIO(void):
	_p1(0),
	_sb(0),
	_sc(0)
{}

GameboyIO::~GameboyIO(void)
{}

void		GameboyIO::init(void)
{
	this->_p1 = 0;
	this->_sc = 0;
	this->_sb = 0;
}

void		GameboyIO::exit(void)
{

}

void		GameboyIO::reload(void)
{
	this->_p1 = *(gbram + 0xFF00);
	this->_sb = *(gbram + 0xFF01);
	this->_sc = *(gbram + 0xFF02);
}

bool		GameboyIO::isConnected(void)
{
	return (false);
}

unsigned char	GameboyIO::getUserInput(void)
{
	return (this->_p1 & 0x3F);
}

void		GameboyIO::serialTransfer(void)
{
	if (this->_sc & (1 << 7))
	{
		if (gbuser->isConnected())
		{
			if (this->_sc & 1)
				gbuser->getCpu()->setClock(gbuser2->getCpu()->getClock());
			else
				gbuser2->getCpu()->setClock(gbuser->getCpu()->getClock());
			gbuser->transfer();
		}
		else
			std::cerr << "Serial Error" << std::endl;
	}
}
