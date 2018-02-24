#ifndef GUIDERIVATED_HPP
# define GUIDERIVATED_HPP
# include <gameboy.hpp>
# include <gdkmm/general.h>
# include <glibmm/regex.h>
# include <gdkmm/pixbuf.h>
# include <gtkmm/label.h>
# include <gtkmm/box.h>
# include <gtkmm/grid.h>
# include <gtkmm/button.h>
# include <gtkmm/togglebutton.h>
# include <gtkmm/colorbutton.h>
# include <gtkmm/eventbox.h>
# include <gtkmm/entry.h>
# include <gtkmm/imagemenuitem.h>
# include <gtkmm/texttag.h>
# include <gtkmm/textview.h>
# include <gtkmm/scrolledwindow.h>
# include <gtkmm/main.h>
# include <gtkmm/menu.h>
# include <gtkmm/menubar.h>
# include <gtkmm/menuitem.h>
# include <gtkmm/separatormenuitem.h>
# include <gtkmm/filechooserdialog.h>
# include <gtkmm/messagedialog.h>
# include <gtkmm/stock.h>
# include <gtkmm/window.h>
# include <gtkmm/builder.h>
# include <gtkmm/listbox.h>
# include <gtkmm/listboxrow.h>
# include <gtkmm/spinner.h>
# include <gtkmm/switch.h>
# include <gtkmm/drawingarea.h>
# include <sigc++/sigc++.h>
# include <guidialog.hpp>

class			Gui;

	class		GuiAddressModifier
	{
		size_t			_len;
		size_t			_maskbits;
		intptr_t		_addr;
		unsigned char*	_backup;

		public :

			GuiAddressModifier(intptr_t addr, size_t len, size_t mask = 0);
			~GuiAddressModifier(void);

			template<typename T = char*>
			char*			getValue(void)
			{
				return (*((char**)this->_addr));
			}

			template<typename T = unsigned char*>
			unsigned char*	getValue(void)
			{
				return (*((unsigned char**)this->_addr));
			}

			template<typename T = char>
			char			getValue(void)
			{
				return (*((char*)this->_addr));
			}

			template<typename T = unsigned char>
			unsigned char	getValue(void)
			{
				return (*((unsigned char*)this->_addr));
			}

			template<typename T = int>
			int				getValue(void)
			{
				return (*((int*)this->_addr));
			}

			template<typename T = unsigned int>
			unsigned int	getValue(void)
			{
				return (*((unsigned int*)this->_addr));
			}

			template<typename T = bool>
			bool			getValue(void)
			{
				return (*((int*)this->_addr) & 0x1);
			}

			void		reload(intptr_t addr, size_t len, size_t mask = 0);

			void		validate(void);
			void		reset(void);
	};

/*******************************************************************/

	class		ItemEntry: public Gtk::Entry
	{
		intptr_t						_addr;
		size_t							_len;
		Glib::RefPtr<Glib::Regex>		_regex;
		unsigned char*					_backup;

		public :

			ItemEntry(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder
				, intptr_t addr, size_t len, const Glib::ustring& regex);
			virtual ~ItemEntry(void);

			void				refresh(void);

			void				registerBackup(const void *buf = NULL, size_t len = 0);
			void				loadBackup(void *buf, size_t len);

			std::string			getValueString(void);
			unsigned int		getValueU32(void);
			unsigned short		getValueU16(void);
			unsigned char		getValueU8(void);

	};

/*******************************************************************/

	class		ServerPanel;
	class		ClientPanel;
	class		EmuPanel;
	class		AsmPanel;

	class		DisplayArea: public Gtk::DrawingArea
	{
		Gameboy*					_g;
		Glib::RefPtr<Gdk::Pixbuf>	_image;

		public :

			DisplayArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
						Gameboy* g);
			virtual ~DisplayArea(void);

			void	gpuOutput(unsigned char*);

		protected :

			bool	on_draw(const Cairo::RefPtr<Cairo::Context>&) override;
	};

	class	AsmPanel: public Gtk::Window
	{
		Glib::RefPtr<Gtk::Builder>		_refGlade;
		Gameboy*						_gbPtr;

		std::vector<t_compiler_method*>	_syntax;

		std::vector<Gtk::Label>			_nums;
		std::vector<Gtk::Label>			_addrs;

		Gtk::TextBuffer*				_textBuf;
		Gtk::TextTagTable*				_tagTable;
		Gtk::TextTag*					_tag_error;
		Gtk::TextTag*					_tag_normal;
		Gtk::TextTag*					_tag_header;
		Gtk::TextTag*					_tag_register;
		Gtk::TextTag*					_tag_value;
		Gtk::TextTag*					_tag_current;
		Gtk::TextTag*					_tag_comment;
		Gtk::Box*						_lines_addr;
		Gtk::Box*						_lines_num;
		Gtk::TextView*					_text_asm;
		Gtk::TextView*					_text_hex;

		public :

			AsmPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
					Gameboy *gb);
			virtual ~AsmPanel(void);

			void						build(void);
			void						resetLines(void);
			void						eventTextChanged(void);
	};

	class		MainWindow: public Gtk::Window
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		Gui&						_refGui;

		EmuPanel*					_emupanel;
		ServerPanel*				_serverpanel;
		ClientPanel*				_clientpanel;
		AsmPanel*					_asmpanel;

		DisplayArea*				_screen;

		Gtk::ImageMenuItem*			_menu_serverpanel;
		Gtk::ImageMenuItem*			_menu_clientpanel;
		Gtk::ImageMenuItem*			_menu_asmpanel;
		Gtk::ImageMenuItem*			_menu_fileopen;
		Gtk::ImageMenuItem*			_menu_quit;
		Gtk::ImageMenuItem*			_menu_emustart;
		Gtk::ImageMenuItem*			_menu_emustop;
		Gtk::ImageMenuItem*			_menu_emupause;
		Gtk::ImageMenuItem*			_menu_emusreset;
		Gtk::ImageMenuItem*			_menu_emuhreset;
		Gtk::ImageMenuItem*			_menu_emuinfo;

		public :

			MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder
						, Gui& g);
			virtual ~MainWindow(void);

			void					openFile(void);
			void					info(void);
			void					quit(void);
			void					start(void);
			void					stop(void);
			void					pause(void);
			void					softReset(void);
			void					hardReset(void);
			void					openServerPanel(void);
			void					openClientPanel(void);
			void					openAsmPanel(void);
			void					closeServerPanel(void);
	};

/*******************************************************************/

	class		Sprite: public Gtk::Box
	{
		public :

			Glib::RefPtr<Gtk::Builder>		_refGlade;
			std::vector<Gtk::ColorButton*>	_btns;
			intptr_t						_refAddr;
			unsigned int					_refCode;
			bool							_refBank;
			Gtk::Label*						_addr;
			Gtk::Label*						_code;
			Gtk::Grid*						_dots;

			Sprite(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, 
				unsigned int code, intptr_t addr, bool bank);
			virtual ~Sprite(void);

			void			eventChangeColor(Gtk::ColorButton *btn, off_t i);

	};

	class		SpriteBank: public Gtk::Box
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		bool						_loading;
		bool						_bank;
		std::vector<Gtk::EventBox*>	_sprites;

		public :

			SpriteBank(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
					bool bank, EmuPanel* panel);
			virtual ~SpriteBank(void);

			void		load(Gtk::Image* img, unsigned int v);

	};

	class		EmuPanel: public Gtk::Window
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		GameboyCpu*					_refCpu;
		GameboyRom*					_refRom;

		Gtk::Entry*					_rom_name;
		ItemEntry*					_rom_title;
		ItemEntry*					_rom_code;
		ItemEntry*					_rom_maker;
		ItemEntry*					_rom_cassette;
		ItemEntry*					_rom_dest;
		ItemEntry*					_rom_version;

		ItemEntry*					_cpu_a;
		ItemEntry*					_cpu_f;
		ItemEntry*					_cpu_bc;
		ItemEntry*					_cpu_de;
		ItemEntry*					_cpu_hl;
		ItemEntry*					_cpu_pc;
		ItemEntry*					_cpu_sp;

		Gtk::ToggleButton*			_cpu_z;
		Gtk::ToggleButton*			_cpu_n;
		Gtk::ToggleButton*			_cpu_h;
		Gtk::ToggleButton*			_cpu_cy;

		Gtk::Box*					_sprite_view;
		SpriteBank*					_sprite0;
		SpriteBank*					_sprite1;

		public :

			typedef
			struct
			{
				Gtk::Image*			_img;
				unsigned int		_v;
			}						t_evarg;

			EmuPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
					GameboyRom* rom, GameboyCpu* cpu);
			virtual ~EmuPanel(void);

			bool		eventClickImage(const GdkEventButton* btn, EmuPanel::t_evarg arg);

			void		refresh(void);

			void		eventClickZ(void);
			void		eventClickN(void);
			void		eventClickH(void);
			void		eventClickCY(void);
	};

/*******************************************************************/

	class		ClientPanel;

	class		ClientServer: public Gtk::ListBoxRow
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		ClientPanel*				_panelparent;
		int							_index;
		Gtk::Label*					_id;
		Gtk::Image*					_status;
		Gtk::Button*				_connect;
		Gtk::Button*				_disconnect;
		Gtk::Button*				_forget;

		public :

			t_servercom					*_com;
			bool						_connected;

			ClientServer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
						t_servercom *com, int index, ClientPanel *panel);
			virtual ~ClientServer(void);
			void	ping(void);
			void	setStatus(int status);
			void	eventConnect(void);
			void	eventDisconnect(void);
			void	eventForget(void);
			ClientPanel	*accessPanel(void);
	};

	class		ClientServers: public Gtk::ListBox
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		std::vector<ClientServer*>	_list;

		public :

			ClientServers(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
						ClientPanel *s);
			virtual ~ClientServers(void);
			void	addConnection(t_servercom *com, int index = -1, ClientPanel *s = NULL);
			void	pingAll(void);
			void	refresh(void);
			void	resetButtons(void);

	};

	class		ClientPanel: public Gtk::Window
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		std::map<std::string, std::string>&					_refConfig;
		ClientServers*				_servers;
		Gtk::Entry*					_username;
		Gtk::Entry*					_port;
		Gtk::Label*					_status;
		Gtk::Entry*					_newhost;
		Gtk::Entry*					_newport;
		Gtk::Button*				_add;
		Gtk::Switch*				_auto;
		Gtk::Switch*				_kick;
		Gtk::Switch*				_disconnect;
		Gtk::Spinner*				_spinner1;
		Gtk::Spinner*				_spinner2;

		public :

			ClientPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
						const std::map<std::string, std::string>& configmap);
			virtual ~ClientPanel(void);

			void	setStatusLabel(void);
			void	refresh(bool do_ping = false);

			void	eventKick(void);
			void	eventAccept(void);
			void	eventConnectError(void);
			void	eventDeny(void);

			void	eventChangeAuto(void);
			void	eventChangeKick(void);
			void	eventChangeDisconnect(void);
			void	eventAdd(void);
			std::string	getClientXML(void);
			void	activeMode(bool set);
	};

/*******************************************************************/

	class		ServerClient: public Gtk::ListBoxRow
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		SOCKET&						_sock;
		SOCKADDR_IN&				_info;
		Gtk::Label*					_id;
		Gtk::Image*					_status;
		Gtk::Button*				_accept;
		Gtk::Button*				_deny;
		Gtk::Button*				_kick;

		public :

			ServerClient(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
			const SOCKET& id, const SOCKADDR_IN info);
			virtual ~ServerClient	(void);
	};

	class		ServerClients: public Gtk::ListBox
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		std::vector<ServerClient*>	_socks;

		public :

			ServerClients(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
			virtual ~ServerClients(void);
	};

	class		ServerPanel: public Gtk::Window
	{
		Glib::RefPtr<Gtk::Builder>	_refGlade;
		ServerClients*				_clients;
		Gtk::Entry*					_hostname;
		Gtk::Entry*					_port;
		Gtk::Entry*					_slot;
		Gtk::ButtonBox*				_statusbar;
		Gtk::Button*				_stop;
		Gtk::Button*				_start;
		Gtk::Button*				_reload;
		Gtk::Spinner*				_spinner;

		public :

			ServerPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
			virtual ~ServerPanel(void);

			void	eventStartClient(void);
			void	eventStopClient(void);

			void	eventStart(void);
			void	eventStop(void);
			void	restart(void);
			void	save(void);
			void	refreshBox(void);
			void	proceed(void);
	};

#endif