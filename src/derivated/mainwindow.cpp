#include <guiderivated.hpp>
#include <gui.hpp>

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, Gui& g):
	Gtk::Window(cobject),
	_refGlade(builder),
	_refGui(g)
{
	this->set_icon_from_file("./images/gb.png");
	this->_serverpanel = NULL;
	this->_clientpanel = NULL;
	this->_emupanel = NULL;

	this->_refGlade->get_widget_derived("screen", this->_screen, g.getGameboy());
	this->_refGlade->get_widget("server-open-panel", this->_menu_serverpanel);
	this->_refGlade->get_widget("client-open-panel", this->_menu_clientpanel);
	this->_refGlade->get_widget("asm-open-panel", this->_menu_asmpanel);

	this->_menu_serverpanel->signal_activate().connect(sigc::mem_fun(this, &MainWindow::openServerPanel));
	this->_menu_clientpanel->signal_activate().connect(sigc::mem_fun(this, &MainWindow::openClientPanel));
	this->_menu_asmpanel->signal_activate().connect(sigc::mem_fun(this, &MainWindow::openAsmPanel));

	this->_refGlade->get_widget("file-open", this->_menu_fileopen);
	this->_refGlade->get_widget("file-quit", this->_menu_quit);

	this->_menu_fileopen->signal_activate().connect(sigc::mem_fun(this, &MainWindow::openFile));
	this->_menu_quit->signal_activate().connect(sigc::mem_fun(this, &MainWindow::quit));

	this->_refGlade->get_widget("emu-start", this->_menu_emustart);
	this->_refGlade->get_widget("emu-pause", this->_menu_emupause);
	this->_refGlade->get_widget("emu-stop", this->_menu_emustop);
	this->_refGlade->get_widget("emu-sreset", this->_menu_emusreset);
	this->_refGlade->get_widget("emu-hreset", this->_menu_emuhreset);
	this->_refGlade->get_widget("emu-open-info", this->_menu_emuinfo);

	this->_menu_emustart->signal_activate().connect(sigc::mem_fun(this, &MainWindow::start));
	this->_menu_emupause->signal_activate().connect(sigc::mem_fun(this, &MainWindow::pause));
	this->_menu_emustop->signal_activate().connect(sigc::mem_fun(this, &MainWindow::stop));
	this->_menu_emusreset->signal_activate().connect(sigc::mem_fun(this, &MainWindow::softReset));
	this->_menu_emuhreset->signal_activate().connect(sigc::mem_fun(this, &MainWindow::hardReset));
	this->_menu_emuinfo->signal_activate().connect(sigc::mem_fun(this, &MainWindow::info));

	this->show_all();
}

MainWindow::~MainWindow(void)
{
	delete this->_menu_clientpanel;
	delete this->_menu_serverpanel;
	delete this->_menu_fileopen;
	delete this->_menu_quit;

	delete this->_menu_emustart;
	delete this->_menu_emustop;
	delete this->_menu_emupause;
	delete this->_menu_emusreset;
	delete this->_menu_emuhreset;
	delete this->_menu_emuinfo;

	if (this->_clientpanel)
		delete this->_clientpanel;

	if (this->_serverpanel)
		delete this->_serverpanel;
}

void		MainWindow::openFile(void)
{
	std::string		s = OpenRomDialog::open((Gtk::Window&)*this);

	if (s != "")
		this->_refGui.loadRom(s);
	this->_screen->queue_draw();
}

void		MainWindow::quit(void)
{
	((Gtk::Application&)this->_refGui).quit();
}

void		MainWindow::start(void)
{
	this->_refGui.start();
	this->_screen->queue_draw();
}

void		MainWindow::stop(void)
{
	this->_refGui.stop();
	this->_screen->queue_draw();
}

void		MainWindow::pause(void)
{
	this->_refGui.pause();
	this->_screen->queue_draw();
}

void		MainWindow::softReset(void)
{
	this->_refGui.softReset();
	this->_screen->queue_draw();
}

void		MainWindow::hardReset(void)
{
	this->_refGui.hardReset();
	this->_screen->queue_draw();
}

void		MainWindow::info(void)
{
	if (this->_emupanel)
		this->_emupanel->show();
	else
		this->_refGlade->get_widget_derived(
				"emu-panel",
				this->_emupanel,
				this->_refGui.getGameboy()->getRom(),
				this->_refGui.getGameboy()->getCpu()
		);
}

void		MainWindow::openServerPanel(void)
{
	if (this->_serverpanel)
		this->_serverpanel->show();
	else
		this->_refGlade->get_widget_derived("server-panel", this->_serverpanel);
}

void		MainWindow::openClientPanel(void)
{
	if (this->_clientpanel)
		this->_clientpanel->show();
	else
		this->_refGlade->get_widget_derived("client-panel", this->_clientpanel, this->_refGui.getConfigMap());
}

void		MainWindow::openAsmPanel(void)
{
	if (this->_clientpanel)
		this->_clientpanel->show();
	else
		this->_refGlade->get_widget_derived("asm-panel", this->_asmpanel, this->_refGui.getGameboy());
}
