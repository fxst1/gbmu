#include <guiderivated.hpp>
#include <globals.hpp>

EmuPanel::EmuPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
	GameboyRom* rom, GameboyCpu* cpu):
	Gtk::Window(cobject),
	_refGlade(builder),
	_refCpu(cpu),
	_refRom(rom)
{
	this->_refGlade->get_widget("rom-name", this->_rom_name);
	this->_refGlade->get_widget_derived("rom-title", this->_rom_title, (intptr_t)gbram + 0x134, 11, "([a-zA-Z0-9 ]){11}");
	this->_refGlade->get_widget_derived("rom-code", this->_rom_code, (intptr_t)gbram + 0x13F, 4, "[a-zA-Z0-9]{4}");
	this->_refGlade->get_widget_derived("rom-maker", this->_rom_maker, (intptr_t)gbram + 0x144, 2, "[a-zA-Z0-9]{2}");
	this->_refGlade->get_widget_derived("rom-cassette", this->_rom_cassette, (intptr_t)gbram + 0x147, 1, "[a-zA-Z0-9]");
	this->_refGlade->get_widget_derived("rom-dest", this->_rom_dest, (intptr_t)gbram + 0x14A, 1, "[a-zA-Z0-9]");
	this->_refGlade->get_widget_derived("rom-version", this->_rom_version, (intptr_t)gbram + 0x14C, 1, "[a-zA-Z0-9]");

	this->_refGlade->get_widget_derived("cpu-a", this->_cpu_a, (intptr_t)(&this->_refCpu->_af), 1, "[a-zA-Z0-9]");
	this->_refGlade->get_widget_derived("cpu-f", this->_cpu_f, (intptr_t)(&this->_refCpu->_af) + 1, 1, "[a-zA-Z0-9]");
	this->_refGlade->get_widget_derived("cpu-bc", this->_cpu_bc, (intptr_t)&this->_refCpu->_bc, 2, "[a-zA-Z0-9]{2}");
	this->_refGlade->get_widget_derived("cpu-de", this->_cpu_de, (intptr_t)&this->_refCpu->_de, 2, "[a-zA-Z0-9]{2}");
	this->_refGlade->get_widget_derived("cpu-hl", this->_cpu_hl, (intptr_t)&this->_refCpu->_hl, 2, "[a-zA-Z0-9]{2}");
	this->_refGlade->get_widget_derived("cpu-pc", this->_cpu_pc, (intptr_t)&this->_refCpu->_pc, 2, "[a-zA-Z0-9]{2}");
	this->_refGlade->get_widget_derived("cpu-sp", this->_cpu_sp, (intptr_t)&this->_refCpu->_sp, 2, "[a-zA-Z0-9]{2}");

	this->_refGlade->get_widget("cpu-z", this->_cpu_z);
	this->_refGlade->get_widget("cpu-n", this->_cpu_n);
	this->_refGlade->get_widget("cpu-h", this->_cpu_h);
	this->_refGlade->get_widget("cpu-cy", this->_cpu_cy);

	//this->_refGlade->get_widget_derived("oam-container", this->_oam_list);
	this->_refGlade->get_widget("sprite-view", this->_sprite_view);
	this->_refGlade->get_widget_derived("sprite-bank-0", this->_sprite0, false, this);
	this->_refGlade->get_widget_derived("sprite-bank-1", this->_sprite1, true, this);

	this->refresh();
	this->show_all();
}

EmuPanel::~EmuPanel()
{
/*	delete this->_rom_name;
	delete this->_rom_title;
	delete this->_rom_code;
	delete this->_rom_cassette;
	delete this->_rom_dest;
	delete this->_rom_version;

	delete this->_cpu_a;
	delete this->_cpu_f;
	delete this->_cpu_bc;
	delete this->_cpu_de;
	delete this->_cpu_hl;
	delete this->_cpu_pc;
	delete this->_cpu_sp;

	delete this->_cpu_z;
	delete this->_cpu_n;
	delete this->_cpu_h;
	delete this->_cpu_cy;

	delete this->_sprite0;
*/}

bool		EmuPanel::eventClickImage(const GdkEventButton* btn, EmuPanel::t_evarg arg)
{
	//Glib::RefPtr<Gtk::Builder>	copy = Gtk::Builder::create();

	//copy->add_from_file("./xml/sprite.glade");
	//copy->get_widget_derived("sprite", this->_sprite_view);

	return (false);
	(void)arg;
	(void)btn;
}

void		EmuPanel::refresh(void)
{
	this->_rom_name->set_text("");
	this->_rom_title->refresh();
	this->_rom_code->refresh();
	this->_rom_maker->refresh();
	this->_rom_cassette->refresh();
	this->_rom_dest->refresh();
	this->_rom_version->refresh();

	this->_cpu_a->refresh();
	this->_cpu_f->refresh();
	this->_cpu_bc->refresh();
	this->_cpu_de->refresh();
	this->_cpu_hl->refresh();
	this->_cpu_pc->refresh();
	this->_cpu_sp->refresh();

	this->_cpu_z->set_mode(this->_refCpu->_af & (1 << 7));
	this->_cpu_n->set_mode(this->_refCpu->_af & (1 << 6));
	this->_cpu_h->set_mode(this->_refCpu->_af & (1 << 5));
	this->_cpu_cy->set_mode(this->_refCpu->_af & (1 << 4));
}

void		EmuPanel::eventClickZ(void)
{
	this->_refCpu->_af ^= (1 << 7);
}

void		EmuPanel::eventClickN(void)
{
	this->_refCpu->_af ^= (1 << 6);
}

void		EmuPanel::eventClickH(void)
{
	this->_refCpu->_af ^= (1 << 5);
}

void		EmuPanel::eventClickCY(void)
{
	this->_refCpu->_af ^= (1 << 4);
}
