#include <menubar.hpp>

Menubar::Menubar()
{
	this->file = new Gtk::Menu();
	this->filemenu = new Gtk::MenuItem("_File", true);
	this->open = new Gtk::MenuItem("_Open", true);
	this->close = new Gtk::MenuItem("_Close", true);
	this->file->append(*this->open);
	this->file->append(*this->close);

//	this->edit = new Gtk::Menu("_Edit", true);
//	this->config = new Gtk::Menu("_Config", true);

	this->debug = new Gtk::Menu();
	this->debugmenu = new Gtk::MenuItem("_Debug", true);
	this->regs = new Gtk::MenuItem("_Registers", true);
	this->_asm = new Gtk::MenuItem("_Asm", true);
	this->debug->append(*this->regs);
	this->debug->append(*this->_asm);

	this->append(*this->filemenu);
//	this->append(this->edit);
//	this->append(this->config);
	this->append(*this->debugmenu);
}

Menubar::~Menubar()
{

}

