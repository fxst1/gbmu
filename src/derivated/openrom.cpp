#include <guiderivated.hpp>
#include <guidialog.hpp>

std::string			OpenRomDialog::open(Gtk::Window& parent)
{
	Glib::RefPtr<Gtk::FileFilter>	gb = Gtk::FileFilter::create();
	Glib::RefPtr<Gtk::FileFilter>	gbc = Gtk::FileFilter::create();
	Glib::RefPtr<Gtk::FileFilter>	all = Gtk::FileFilter::create();
	Glib::RefPtr<Gtk::FileFilter>	supported = Gtk::FileFilter::create();
	Glib::RefPtr<Gtk::FileFilter>	bin = Gtk::FileFilter::create();
	Gtk::FileChooserDialog			dialog(parent, "Open rom", Gtk::FILE_CHOOSER_ACTION_OPEN);

	/**/
	dialog.set_transient_for(parent);
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
//	dialog.add_modal_grab();

	/**/
	gb->set_name("DMG - Monochrome");
	gb->add_pattern("*.gb");

	gbc->set_name("CGB - Color");
	gbc->add_pattern("*.gbc");

	all->set_name("All files");
	all->add_pattern("*");

	supported->set_name("Supported formats");
	supported->add_pattern("*.gb");
	supported->add_pattern("*.gbc");
	supported->add_pattern("*.bin");
	supported->add_pattern("*.raw");

	bin->set_name("Binary");
	bin->add_mime_type("application/octet-stream");
	bin->add_mime_type("application/x-binary");

	/**/
	dialog.add_filter(supported);
	dialog.add_filter(all);
	dialog.add_filter(bin);
	dialog.add_filter(gbc);
	dialog.add_filter(gb);

	/**/

	int result = dialog.run();

	if (result == Gtk::RESPONSE_OK)
		return (dialog.get_filename());
	return ("");
}