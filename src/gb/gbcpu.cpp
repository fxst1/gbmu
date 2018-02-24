#include <gameboy.hpp>
#include <globals.hpp>

GameboyCpu::GameboyCpu(void):
	_doDma(false),
	_dma_src(0),
	_dma_dst(0),
	_clockfreq(0),
	_af(0),
	_bc(0),
	_de(0),
	_hl(0),
	_pc(0),
	_sp(0),
	_div(0),
	_tima(0),
	_tma(0),
	_tac(0),
	_if(0),
	_ie(0)
{
	this->_dma_size = 32;
	this->setSpeed(1);
	this->setFrameSkip(0);
}

GameboyCpu::~GameboyCpu(void)
{}

void	GameboyCpu::setSpeed(unsigned int speed)
{
	this->_speed = speed;
}

void	GameboyCpu::setFrameSkip(unsigned int frames)
{
	this->_frameskip = frames;
}

void	GameboyCpu::frameskip(void)
{
	unsigned int	frame = 0;

	while (frame < this->_frameskip)
	{
		//usleep(1);
		frame++;
	}
}

void	GameboyCpu::waitCycles(unsigned char cycles)
{
	(void)cycles;
}

bool	GameboyCpu::accessRam(unsigned short addr, bool read)
{
	//DMA
	if (addr == 0xFF46)
	{
		this->_doDma = true;
		return (!read);
	}
	//LY
	else if (addr == 0xFF44)
		return (read);
	//BGP, OBP0, OBP1
	else if (addr == 0xFF47 || addr == 0xFF48 || addr == 0xFF49)
		return (!read);
	//HDMA1 - HDMA5
	else if (addr >= 0xFF51 && addr <= 0xFF55)
		return (!read);

	return (true);
}

void	GameboyCpu::init(void)
{
	this->_clockfreq = 1;
	this->_af = 0;
	this->_bc = 0;
	this->_de = 0;
	this->_hl = 0;
	this->_pc = 0;
	this->_sp = 0xFFFE;
	this->_div = 0;
	this->_tima = 0;
	this->_tma = 0;
	this->_tac = 0;
	this->_ime = true;
	this->_if = 0;
	this->_ie = 0xD1;
}

void	GameboyCpu::pause(void)
{
	this->_clockfreq = 0;
}

void	GameboyCpu::exit(void)
{
	this->_clockfreq = 0;
}

void	GameboyCpu::reload(void)
{
	this->_div = *(gbram + 0xFF04);
	this->_tima = *(gbram + 0xFF05);
	this->_tma = *(gbram + 0xFF06);
	this->_tac = *(gbram + 0xFF07);
	this->_if = *(gbram + 0xFF0F);
	this->_ie = *(gbram + 0xFFFF);
}

unsigned int	GameboyCpu::getClock(void)
{
	return (this->_clockfreq);
}

void			GameboyCpu::setClock(unsigned int freq)
{
	this->_clockfreq = freq;
}

void			GameboyCpu::execute(unsigned char* bios)
{
	printf("Bios (%d) %x %x\n", this->_pc, *(bios + this->_pc), *(bios + this->_pc + 1));
	this->instruction_parser(bios + this->_pc);
}

void			GameboyCpu::execute(void)
{
	std::cout << "Code " << std::to_string(*(gbram + this->_pc)) << std::endl;
	std::cout << "PC " << std::to_string(this->_pc) << std::endl;
	this->instruction_parser(gbram + this->_pc);
}

void			GameboyCpu::interrupt(void)
{
	unsigned short	address = 0;

	if (this->_if & (1 << GameboyCpu::VBLANK_INTERRUPT))
		address = 0x0040;
	else if (this->_if & (1 << GameboyCpu::LCDC_INTERRUPT))
		address = 0x0048;
	else if (this->_if & (1 << GameboyCpu::TIMER_INTERRUPT))
		address = 0x0050;
	else if (this->_if & (1 << GameboyCpu::SERIAL_INTERRUPT))
		address = 0x0058;
	else if (this->_if & (1 << GameboyCpu::INPUT_INTERRUPT))
		address = 0x0060;

	this->_sp--;
	*(gbram + this->_sp) = this->_pc;
	this->_pc = address;
}

void			GameboyCpu::interrupt(unsigned char bit)
{
	this->_if = *(gbram + 0xFF0F) | (1 << bit);
	this->interrupt();
}
