#ifndef MENUBAR_HPP
# define MENUBAR_HPP
# include <gtkmm/menubar.h>
# include <gtkmm/menuitem.h>
# include <gtkmm/menu.h>

class	Menubar: public Gtk::MenuBar
{
	public :

		Menubar();
		~Menubar();

	private:

		Gtk::Menu		*file;
		Gtk::MenuItem	*filemenu;
		Gtk::MenuItem	*open;
		Gtk::MenuItem	*close;

		Gtk::Menu		*debug;
		Gtk::MenuItem	*debugmenu;
		Gtk::MenuItem	*regs;
		Gtk::MenuItem	*_asm;
};

#endif