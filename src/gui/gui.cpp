#include <gui.hpp>
#include <globals.hpp>
#include <thread>

Gui::Gui(int argc, char **argv):
	Gtk::Application("fxst1.emu.gbmu"),
	_builder(Gtk::Builder::create()),
	_gb(new Gameboy()),
	_config((char*)DEFAULT_CONFIG_FILE),
	_configmap(std::map<std::string, std::string>())
{
	this->_main = nullptr;

	argv++;
	while (*argv)
	{
		if (strcmp(*argv, "--config") == 0)
		{
			if (!*(argv + 1))
				std::cerr << "Config file expected" << std::endl;
			else
				this->_config = *(argv + 1);
			argv += 2;
		}
		else
			argv++;
	}
	this->defaultConfig();
	this->configure();
	this->reloadConfigs();
	(void)argc;
}

Gui::~Gui(void)
{
	delete	this->_main;
	delete	this->_gb;
}

Gameboy*	Gui::getGameboy(void)
{
	return (this->_gb);
}

void		Gui::on_activate(void)
{
	this->init();
}

bool		Gui::loadRom(std::string s)
{
	this->_gb->open(s.c_str());
	if (!this->_gb->isReady())
	{
		ConfirmDialog::create(*this->_main, s + " is not a valid gameboy rom", Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		return (false);
	}
	return (true);
}

void		Gui::start(void)
{
	std::string		s = "";

	if (!this->_gb->isReady())
	{
		s = OpenRomDialog::open(*this->_main);
		if (s == "")
			return ;
		this->loadRom(s);
	}
	if (!this->_gb->isRunning())
		this->_gb->powerOn();
}

void		Gui::stop(void)
{
	if (this->_gb->isRunning())
		this->_gb->powerOff();
}

void		Gui::pause(void)
{
	if (this->_gb->isRunning())
		this->_gb->pause();
}

void		Gui::softReset(void)
{
	if (this->_gb->isRunning())
		this->_gb->softReset();
}

void		Gui::hardReset(void)
{
	if (this->_gb->isRunning())
		this->_gb->hardReset();
}

void		Gui::fatalError(const std::string& err)
{
	std::cerr << err << std::endl;
	std::exit(EXIT_FAILURE);
}

void		Gui::fatalError(const char* err)
{
	std::cerr << err << std::endl;
	std::exit(EXIT_FAILURE);
}

void		Gui::init(void)
{
	if (this->_main)
		delete this->_main;
	try
	{
		this->_builder->get_widget_derived("main-window", this->_main, *this);	
	}
	catch (Gtk::BuilderError& e)
	{
		Gui::fatalError(e.what());
	}

	if (!this->_main)
		this->fatalError("Cannot create main-window");
	this->_main->present();
	this->add_window(*this->_main);
}

/************************************************************/

void				Gui::defaultConfig(void)
{
	this->_configmap["default_ui"] = DEFAULT_UI;
	this->_configmap["default_css"] = DEFAULT_CSS;
	this->_configmap["default_lang"] = DEFAULT_LANG;
	this->_configmap["server_panel_ui"] = DEFAULT_SERVERPANEL_UI;
	this->_configmap["server_panel_css"] = DEFAULT_SERVERPANEL_CSS;
	this->_configmap["server_client_ui"] = DEFAULT_SERVERCLIENT_UI;
	this->_configmap["client_panel_ui"] = DEFAULT_SERVERPANEL_UI;
	this->_configmap["client_panel_css"] = DEFAULT_SERVERPANEL_CSS;
	this->_configmap["client_server_ui"] = DEFAULT_SERVERCLIENT_UI;

	this->_configmap["server_slots"] = DEFAULT_SERVER_SLOTS;
	this->_configmap["default_server_port"] = DEFAULT_SERVER_PORT;

	this->_configmap["default_client_port"] = DEFAULT_CLIENT_PORT;
	this->_configmap["auto_connect"] = DEFAULT_NOTIFY_AUTO;
	this->_configmap["notify_kick"] = DEFAULT_NOTIFY_KICK;
	this->_configmap["notify_disconnect"] = DEFAULT_NOTIFY_DISCONNECT;
}

Gui::ConfigMap&		Gui::getConfigMap(void)
{
	return (this->_configmap);
}

void				Gui::configure(void)
{
	std::string								s = "";

	if (FILE *fp = fopen(this->_config, "r"))
	{
		char buf[1024] = {0};
		while (fread(buf, 1, sizeof(buf), fp) > 0)
			s += buf;
		fclose(fp);
		Gui::readconfig(s.c_str(), this->_configmap);
	}
	else
	{
		std::cerr << "Cannot open config file: " << this->_config << std::endl;
		return ;
	}
}

void				Gui::reloadConfigs(void)
{
	std::string		def_ui = this->_configmap["default_ui"];
	std::string		def_css = this->_configmap["default_css"];
	std::string		def_lang = this->_configmap["default_lang"];
	size_t			sslots = (size_t)atoi(this->_configmap["server_slots"].c_str());

	if (def_ui.length() == 0)
	{
		def_ui = DEFAULT_UI;
		this->_configmap["default_ui"] = DEFAULT_UI;
	}
	if (def_css.length() == 0)
	{
		def_css = DEFAULT_CSS;
		this->_configmap["default_css"] = DEFAULT_CSS;
	}
	if (def_lang.length() == 0)
	{
		def_lang = DEFAULT_LANG;
		this->_configmap["default_lang"] = DEFAULT_LANG;
	}

	if (sslots == 0)
	{
		sslots = (size_t)atoi((char*)DEFAULT_SERVER_SLOTS);
		this->_configmap["server_slots"] = DEFAULT_SERVER_SLOTS;		
	}

	server._ncom = sslots;
	this->_builder->add_from_file(def_ui);
	this->_builder->add_from_file(this->_configmap["server_panel_ui"]);
	this->_builder->add_from_file(this->_configmap["server_client_ui"]);
	this->_builder->add_from_file(this->_configmap["client_panel_ui"]);
	this->_builder->add_from_file(this->_configmap["client_server_ui"]);

	if (this->_configmap["server_list"] != "")
	{
		Gui::readserverlist(Gui::readfile(this->_configmap["server_list"].c_str()).c_str(), client._servers);
	}

	(void)def_css;
	(void)def_lang;
}

std::string			Gui::readfile(const char* filename)
{
	std::string	s = "";
	if (FILE *fp = fopen(filename, "r"))
	{
		char buf[1024] = {0};
		while (fread(buf, 1, sizeof(buf), fp))
			s += buf;
		fclose(fp);
	}
	return (s);
}

void				Gui::readconfig(const char* file, std::map<std::string, std::string>& cfg)
{
	std::string		key = "";
	std::string		value = "";
	size_t			line = 1;

	while (*file)
	{
		key = "";
		value = "";
		while (*file && *file != ':')
		{
			if (!isspace(*file))
				key += *file;
			file++;
		}

		if (key.length() > 0 && *file != ':')
		{
			std::cerr << "Bad config file : Missing separator at line " << std::to_string(line) << std::endl;
			std::cerr << key << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (*file == ':')
			file++;

		while (*file && *file != '\n')
		{
			if (!isspace(*file))
				value += *file;
			file++;
		}

		if (key.length() > 0 && value.length() == 0)
		{
			std::cerr << "Bad config file : Missing value at line " << std::to_string(line) << std::endl;
			std::cerr << key << std::endl;
			exit(EXIT_FAILURE);
		}

		if (*file == '\n')
		{
			file++;
			line++;
		}

		if (value.length() > 0 && key.length() > 0)
		{
			std::cout << key << " : " << value << std::endl;
			cfg[key] = value;
		}
	}
}

static void		__doPing(t_servercom *s)
{
	s->ready = GameboyClient::ping(s->host, s->port);
}

void	Gui::readserverlist(const char* file, std::vector<t_servercom*>& cfg)
{
	std::string		key = "";
	std::string		value = "";
	size_t			line = 1;

	while (*file)
	{
		key = "";
		value = "";
		while (*file && *file != ':')
		{
			if (!isspace(*file))
				key += *file;
			file++;
		}

		if (key.length() > 0 && *file != ':')
		{
			std::cerr << "Bad config file : Missing separator at line " << std::to_string(line) << std::endl;
			std::cerr << key << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (*file == ':')
			file++;

		while (*file && *file != '\n')
		{
			if (!isspace(*file))
				value += *file;
			file++;
		}

		if (key.length() > 0 && value.length() == 0)
		{
			std::cerr << "Bad config file : Missing value at line " << std::to_string(line) << std::endl;
			std::cerr << key << std::endl;
			exit(EXIT_FAILURE);
		}

		if (*file == '\n')
		{
			file++;
			line++;
		}

		if (value.length() > 0 && key.length() > 0)
		{
			t_servercom	*com = (t_servercom*)malloc(sizeof(t_servercom));

			if (com)
			{
				com->host = (char*)strdup(key.c_str());
				com->port = (unsigned short)atoi(value.c_str());
				com->ready = false;
				cfg.push_back(com);
				std::cout << "adding" << com->host << " : " << std::to_string(com->port) << std::endl;
				std::thread		th(__doPing, com);

				th.detach();
			}
		}
	}
}
