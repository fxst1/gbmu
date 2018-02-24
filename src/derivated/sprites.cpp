#include <guiderivated.hpp>
#include <globals.hpp>
#include <gtkmm/eventbox.h>

SpriteBank::SpriteBank(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
			bool bank, EmuPanel *panel):
	Gtk::Box(cobject),
	_refGlade(builder),
	_bank(bank),
	_sprites(std::vector<Gtk::EventBox*>())
{
	for (unsigned int	v = 0x8000; v < 0x9800; v += 0x10)
	{
		EmuPanel::t_evarg	arg;
		Gtk::EventBox*		evbox = new Gtk::EventBox();
		Gtk::Image*			img = new Gtk::Image();

		arg._img = img;
		arg._v = v;

		evbox->add(*img);
		this->pack_start(*evbox, false, false);
		this->load(img, v);
		evbox->add(*img);
		evbox->set_events(Gdk::BUTTON_PRESS_MASK);
		evbox->signal_button_press_event().connect(
			sigc::bind<EmuPanel::t_evarg>(
				sigc::mem_fun(*panel, &EmuPanel::eventClickImage),
				arg
			)
		);

		v += 0x10;
	}
}

SpriteBank::~SpriteBank(void)
{
	for (auto it = this->get_children().begin(); it != this->get_children().end(); it++)
		delete (*it);
}

void		SpriteBank::load(Gtk::Image* img, unsigned int v)
{
	Glib::RefPtr<Gdk::Pixbuf>	ref;
	unsigned char				buffer[8*8*3];
	unsigned int				color;
	unsigned char				upper;
	unsigned char				lower;
	unsigned int				i;
	size_t						offset;

	offset = 0;
	bzero(buffer, sizeof(buffer));
	for (unsigned int y = 0; y < 16; y += 2)
	{
		i = 1;
		upper = *(gbram + v + y);
		lower = *(gbram + v + y + 1);
		while (i < 8)
		{
			color = (upper >> (i - 1)) | (lower >> i);
			if (color == 3)
				color = 0x1f1f1f;
			else if (color == 2)
				color = 0x151515;
			else if (color == 1)
				color = 0x0A0A0A;
			else
				color = 0x0;
			memcpy(&buffer[offset], &color, 3);
			offset += 3;
		}
	}
	ref = Gdk::Pixbuf::create_from_data(
			(unsigned char*)buffer,
			Gdk::Colorspace::COLORSPACE_RGB,
			true,
			8,
			8,
			8,
			0
	);
	img->set(ref);
}

Sprite::Sprite(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
			unsigned int code, intptr_t addr, bool bank):
	Gtk::Box(cobject),
	_refGlade(builder),
	_btns(std::vector<Gtk::ColorButton*>()),
	_refAddr(addr),
	_refCode(code),
	_refBank(bank)
{
	off_t				i = 0;
	char				buf[32] = {0};

	this->_refGlade->get_widget("sprite-name", this->_addr);
	this->_refGlade->get_widget("sprite-code", this->_code);
	this->_refGlade->get_widget("sprite-content", this->_dots);

	sprintf(buf, "0x%hx", (unsigned int)this->_refAddr);
	this->_addr->set_text(std::string(buf));
	sprintf(buf, "0x%hhx", (unsigned int)this->_refCode);
	this->_code->set_text(std::string(buf));

	for (auto it = this->_btns.begin();
		it != this->_btns.end();
		it++)
	{
		Gtk::ColorButton*	tmp = (Gtk::ColorButton*)(*it);

		this->_btns.push_back(tmp);
		if (*(gbram + this->_refAddr) & (1 << i))
			tmp->set_rgba( Gdk::RGBA("rgb(255, 0, 0)") );
		else
			tmp->set_rgba( Gdk::RGBA("rgb(0, 255, 0)") );

		tmp->signal_color_set().connect(
			sigc::bind<Gtk::ColorButton*, off_t>(
				sigc::mem_fun(this, &Sprite::eventChangeColor),
				tmp,
				this->_refAddr + i
			)
		);
		i++;
	}
	this->show_all();
}

Sprite::~Sprite(void)
{
	delete	this->_addr;
	delete	this->_code;
	delete	this->_dots;
}

void			Sprite::eventChangeColor(Gtk::ColorButton *btn, off_t i)
{
//	Gdk::RGBA	color = btn->get_rgba();

//	*(gbram + i) = color.get_red_u() | (color.get_green_u() << 10) | (color.get_blue_u() << 20);
	(void)btn;
	(void)i;
}
