#include <gameboy.hpp>
#include <globals.hpp>

unsigned int	GameboyLcd::RGB(unsigned char r, unsigned char g, unsigned char b)
{
	return (r | ((g << 5) | (b << 10)));
}

void			GameboyLcd::initMainRegisters(void)
{
	if (gbram)
	{
		this->_ly = *(gbram + 0xFF44);
		this->_lyc = *(gbram + 0xFF45);
		this->_lcdc = *(gbram + 0xFF40);
		this->_stat = *(gbram + 0xFF41);
		this->_scy = *(gbram + 0xFF42);
		this->_scx = *(gbram + 0xFF43);
		this->_wy = *(gbram + 0xFF4A);
		this->_wx = *(gbram + 0xFF4B);

		this->_oam = gbram + 0xFE00;

		this->_tiledata = (this->_lcdc & (1 << 4)) ?
							gbram + 0x8000 : gbram + 0x8800;
		this->_bgtilemap = (this->_lcdc & (1 << 3)) ?
							gbram + 0x9C00 : gbram + 0x9800;
		this->_wintilemap = (this->_lcdc & (1 << 6)) ?
							gbram + 0x9C00 : gbram + 0x9800;
	}
	else
	{
		this->_ly = 0;
		this->_lyc = 0;
		this->_lcdc = 0;
		this->_stat = 0;
		this->_oam = 0;
		this->_scx = 0;
		this->_scy = 0;
		this->_wx = 0;
		this->_wy = 0;
	}
}

void			GameboyLcd::init(void)
{
	bzero(this->_output, 360);
	this->_current_line = 0;
	bzero(&this->_buffer[0], sizeof(this->_buffer[0]));
	bzero(&this->_buffer[1], sizeof(this->_buffer[1]));
	this->_selectedBuffer = this->_buffer[0];
	this->_greypal[0] = GameboyLcd::RGB(31, 31, 31);
	this->_greypal[1] = GameboyLcd::RGB(21, 21, 21);
	this->_greypal[2] = GameboyLcd::RGB(10, 10, 10);
	this->_greypal[3] = GameboyLcd::RGB(0, 0, 0);
	this->initMainRegisters();
}

void			GameboyLcd::exit(void)
{

}

void			GameboyLcd::reload(void)
{
	this->initMainRegisters();
}

void			GameboyLcd::setCallback(std::function<void(unsigned char*)> fct)
{
	this->_callback = fct;
}

GameboyLcdDMG::GameboyLcdDMG(void):
	_bgp(0),
	_obp0(0),
	_obp1(0)
{
	this->init();
}

GameboyLcdDMG::~GameboyLcdDMG(void)
{

}

void			GameboyLcdDMG::init(void)
{
	this->_bgp = 0;
	this->_obp0 = 0;
	this->_obp1 = 0;

	this->_pal[0] = 0;
	this->_pal[1] = 0;
	this->_pal[2] = 0;
	this->_pal[3] = 0;

	this->_pal0[0] = 0;
	this->_pal0[1] = 0;
	this->_pal0[2] = 0;
	this->_pal0[3] = 0;

	this->_pal1[0] = 0;
	this->_pal1[1] = 0;
	this->_pal1[2] = 0;
	this->_pal1[3] = 0;

	this->initMainRegisters();
}

void			GameboyLcdDMG::reload(void)
{
	this->initMainRegisters();

	this->_bgp = gbram[0xFF47];
	this->_obp0 = gbram[0xFF48];
	this->_obp1 = gbram[0xFF49];

	this->_pal[0] = this->_greypal[(this->_bgp & 0x3) & 0x3];
	this->_pal[1] = this->_greypal[(this->_bgp >> 2) & 0x3];
	this->_pal[2] = this->_greypal[(this->_bgp >> 4) & 0x3];
	this->_pal[3] = this->_greypal[(this->_bgp >> 6) & 0x3];

	this->_pal0[0] = this->_greypal[(this->_obp0 & 0x3) & 0x3];
	this->_pal0[1] = this->_greypal[(this->_obp0 >> 2) & 0x3];
	this->_pal0[2] = this->_greypal[(this->_obp0 >> 4) & 0x3];
	this->_pal0[3] = this->_greypal[(this->_obp0 >> 6) & 0x3];

	this->_pal1[0] = this->_greypal[(this->_obp1 & 0x3) & 0x3];
	this->_pal1[1] = this->_greypal[(this->_obp1 >> 2) & 0x3];
	this->_pal1[2] = this->_greypal[(this->_obp1 >> 4) & 0x3];
	this->_pal1[3] = this->_greypal[(this->_obp1 >> 6) & 0x3];
}

unsigned char*	GameboyLcdDMG::getOutput(void)
{
	this->drawLine(*(gbram + 0xFF44));
	return ((unsigned char*)this->_output);
}

void			GameboyLcdDMG::applyTileRow(unsigned char upper, unsigned char lower, unsigned char* out)
{
	size_t			i = 0;
	unsigned int	color = 0;

	while (i < 8)
	{
		color = (upper >> (i - 1)) | (lower >> i);
		if (color > 3)
			std::cerr << "WTF" << std::endl;
		else
			memcpy(out, &this->_greypal[color], 4);
		i++;
	}
}

unsigned char*	GameboyLcd::getBufferOffset(unsigned int x, unsigned int y)
{
	return (this->_output + (this->_scx - x) + ((this->_scy - y) * 8));
}

void			GameboyLcdDMG::drawLine(unsigned int y)
{
	unsigned char		chr = 0;
	unsigned int		h_dots = 8;
	unsigned char		gbtile[32];

	this->reload();
	h_dots = (this->_lcdc & (1 << 2)) ? 8 : 16;

	if (y == this->_scy)
		*(gbram + 0xFF41) |= (1 << 2);
	else
		*(gbram + 0xFF41) &= ~(1 << 2);

	if (this->_lyc == this->_ly)
		*(gbram + 0xFF41) |= 1 << 6;

	if (this->_scy < y || (unsigned char)(this->_scy + 18) > y)
		return ;

	for (unsigned int x = 0; x < 256; x++)
	{
		if (this->_scx < x || (unsigned char)(this->_scx + 20) > x)
			continue ;
		chr = *this->_bgtilemap;
		memcpy(gbtile, this->_tiledata + chr * 16, h_dots * 2);

		for (unsigned int i = 0; i < h_dots; i++)
			this->applyTileRow(gbtile[1], gbtile[0], this->getBufferOffset(x, y));

		this->_tiledata += (h_dots * 2);
	}
	*(gbram + 0xFF44) = (unsigned char)y + 1;
}
