#ifndef GUIDIALOG_HPP
# define GUIDIALOG_HPP
# include <gtkmm/filechooserdialog.h>
# include <gtkmm/filefilter.h>
# include <gtkmm/messagedialog.h>

	class			ConfirmDialog: Gtk::MessageDialog
	{
		public :

			static int			create(Gtk::Window& parent,
									const std::string& primary,
									const std::string& secondary,
									Gtk::MessageType type, Gtk::ButtonsType btns)
			{
				Gtk::MessageDialog		dialog(parent, primary,
											true, type, btns);
				dialog.set_secondary_text(secondary);
				return (dialog.run());
			}

			static int			create(Gtk::Window& parent,
									const std::string& primary,
									Gtk::MessageType type, Gtk::ButtonsType btns)
			{
				Gtk::MessageDialog		dialog(parent, primary,
											false, type, btns);
				return (dialog.run());
			}
	};

	class		OpenRomDialog: public Gtk::FileChooserDialog
	{
		public :

			static std::string	open(Gtk::Window& parent);
	};

#endif