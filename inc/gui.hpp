#ifndef GUI_HPP
# define GUI_HPP
# include <gameboy.hpp>
# include <gtkmm/builder.h>
# include <gtkmm/main.h>
# include <guiderivated.hpp>
# include <algorithm>

#ifdef WIN32
# define	DEFAULT_CONFIG_FILE 		".\\config"
# define	DEFAULT_UI					".\\xml\\main.glade"
# define	DEFAULT_CSS					".\\xml\\ui.css"
# define	DEFAULT_LANG				".\\lang\\fr"
# define	DEFAULT_SERVERPANEL_UI		".\\xml\\server_panel.glade"
# define	DEFAULT_SERVERCLIENT_UI		".\\xml\\server_client.glade"
# define	DEFAULT_SERVERPANEL_CSS		".\\xml\\server_panel.css"
#else
# define	DEFAULT_CONFIG_FILE			"./config"
# define	DEFAULT_UI					"./xml/main.glade"
# define	DEFAULT_CSS					"./xml/ui.css"
# define	DEFAULT_LANG				"./lang/fr"
# define	DEFAULT_SERVERPANEL_UI		"./xml/server_panel.glade"
# define	DEFAULT_SERVERCLIENT_UI		"./xml/server_client.glade"
# define	DEFAULT_SERVERPANEL_CSS		"./xml/server_panel.css"
# define	DEFAULT_CLIENTPANEL_UI		"./xml/client_panel.glade"
# define	DEFAULT_CLIENTSERVER_UI		"./xml/client_server.glade"
# define	DEFAULT_CLIENTPANEL_CSS		"./xml/client_panel.css"
#endif

# define	DEFAULT_SERVER_SLOTS		"5"
# define	DEFAULT_SERVER_SLOTS_INT	5

# define	DEFAULT_SERVER_PORT			"11000"
# define	DEFAULT_SERVER_PORT_INT		11000

# define	DEFAULT_CLIENT_PORT			"11001"
# define	DEFAULT_CLIENT_PORT_INT		11001

# define	DEFAULT_NOTIFY_AUTO			"true"
# define	DEFAULT_NOTIFY_KICK			"true"
# define	DEFAULT_NOTIFY_DISCONNECT	"true"

# define	GBMU_VERSION				"AlphaDev"
# define	GBMU_VERSIONNO				0.01

class	Gui: public Gtk::Application
{
	public :

		using ConfigMap = std::map<std::string, std::string>;

		Gui(int argc, char** argv);
		~Gui(void);

		MainWindow		*_main;

		void			configure(void);
		void			init(void);

		bool			loadRom(std::string s);
		void			start(void);
		void			stop(void);
		void			pause(void);
		void			softReset(void);
		void			hardReset(void);

		ConfigMap&		getConfigMap(void);

		Gameboy*		getGameboy(void);

		template<typename T = bool>
		bool			getConfigValue(const std::string& key)
		{
			std::string	value = this->_configmap[key];

			std::transform(value.begin(), value.end(), value.begin(), ::tolower);
			if (value == "true" || value == "1")
				return (true);
			else if (value == "false" || value == "0")
				return (false);

			throw std::exception("[" + key + ":" + value + "] not boolean");
			return (false);
		}

		template<typename T = bool>
		int				getConfigValue(const std::string& key)
		{
			std::string	value = this->_configmap[key];

			if (value.length() == 0)
				throw std::exception("[" + key + ":" + value + "] not integer");
			return (atoi(value.c_str()));
		}

		template<typename T = bool>
		std::string		getConfigValue(const std::string& key, bool allow_empty = false)
		{
			std::string	value = this->_configmap[key];

			if (value.length() == 0 && !allow_empty)
				throw std::exception("[" + key + ":" + value + "] is empty");
			return (value);
		}

		void			setConfigValue(const std::string& key, bool value)
		{
			this->_configmap[key] = (value ? "true" : "false");
		}

		void			setConfigValue(const std::string& key, int value)
		{
			this->_configmap[key] = std::to_string(value);
		}

		void			setConfigValue(const std::string& key, std::string& value)
		{
			this->_configmap[key] = value;
		}

		void			reloadConfigs(void);
		void			defaultConfig(void);

		static void		fatalError(const std::string& err);
		static void		fatalError(const char* err);
		static void		readconfig(const char* file, std::map<std::string, std::string>& cfg);
		static void		readserverlist(const char* file, std::vector<t_servercom*>& cfg);
		static std::string	readfile(const char* filename);

	private :

		Glib::RefPtr<Gtk::Builder>			_builder;
		Gameboy*							_gb;
		char*								_config;
		std::map<std::string, std::string>	_configmap;

	protected :

		void			on_activate() override;

};

#endif