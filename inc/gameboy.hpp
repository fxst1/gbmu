#ifndef GAMEBOY_HPP
# define GAMEBOY_HPP
# include <functional>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <iostream>
# include <string>
# include <socket.h>
# include <vector>
# include <map>
# include <bios.h>

# define	REGISTER_8(reg, byte) (((unsigned char*)reg) + byte)
# define	REGISTER_16(reg, byte) (unsigned short*)(((unsigned char*)reg) + byte)
# define	MAX_OPCODES	1024

class	Gameboy;
typedef
void					(*instruction_t)(void);

/******************************************************************/

class	GameboyRom
{
	public:

		bool			status;

		GameboyRom(const char *filename = NULL);
		~GameboyRom(void);

		bool			load(const char *filename);
		bool			load(const int fd);

		bool			header(void);

		unsigned char	get_sgb_support_code(void);
		unsigned char	get_cgb_support_code(void);
		size_t			get_rom_size();
		size_t			get_ram_size();

		std::string		get_cartridge_string();
		std::string		get_rom_size_string();
		std::string		get_ram_size_string();
		std::string		get_dest_code_string();

		size_t			calc_checksum();

		unsigned char*	get_code();
		unsigned short	get_start_address();
		unsigned char*	get_buffer();

		char			*filename;
		unsigned char	*buffer;
		size_t			len;

		unsigned short	start_addr;
		unsigned char*	game_title;
		unsigned char*	game_code;
		unsigned char	cgb_support_code;
		unsigned char*	maker_code;
		unsigned char	sgb_support_code;
		unsigned char	cassette_type;
		unsigned char	rom_size;
		unsigned char	ram_size;
		unsigned char	dest_code;
		unsigned char	mask_rom_version;
		unsigned char	complement_check;
		unsigned short	checksum;

		bool			info(void);
};

/******************************************************************/

typedef	struct							s_compiler_method
{
	//Recognize code -> bin
	char*								_pattern;
	std::initializer_list<unsigned int>	_list;

	//Recognize bin -> dis
	unsigned char						_opcode[3];
	size_t								_len;
}										t_compiler_method;

class	GameboyCpu
{
	public :

		bool				_doDma;
		size_t				_dma_size;
		unsigned short		_dma_src;
		unsigned short		_dma_dst;

		unsigned int		_frameskip;
		unsigned int		_speed;

		unsigned char		_stack[127];
		unsigned int		_clockfreq;

		/*
		**	CORE
		*/

		unsigned short		_af;
		unsigned short		_bc;
		unsigned short		_de;
		unsigned short		_hl;
		unsigned short		_pc;
		unsigned short		_sp;

		/*
		**	TIMER
		*/

		unsigned char		_div;
		unsigned char		_tima;
		unsigned char		_tma;
		unsigned char		_tac;

		/*
		**	INTERRUPT
		*/

		bool				_ime;
		unsigned char		_if;
		unsigned char		_ie;

		const size_t		max_opcodes = MAX_OPCODES;

		const unsigned char	FLAG_Z = 1;
		const unsigned char	FLAG_C = 2;
		const unsigned char	FLAG_NZ = 3;
		const unsigned char	FLAG_NC = 4;

		const unsigned char	VBLANK_INTERRUPT = 0;
		const unsigned char	LCDC_INTERRUPT = 1;
		const unsigned char	TIMER_INTERRUPT = 2;
		const unsigned char	SERIAL_INTERRUPT = 3;
		const unsigned char	INPUT_INTERRUPT = 4;

		GameboyCpu(void);
		~GameboyCpu(void);
		std::string		disassembly_line(unsigned char *code,
									std::vector<t_compiler_method*> list,
									size_t *len);
		std::string		disassembly(unsigned char *code,
									std::vector<t_compiler_method*> list);

		void			setSpeed(unsigned int speed);
		void			setFrameSkip(unsigned int frames);
		void			frameskip();
		void			init(void);
		void			pause(void);
		void			exit(void);
		void			reload(void);

		void			setClock(unsigned int freq);
		unsigned int	getClock(void);

		void			execute(unsigned char* bios);
		virtual void	execute(void);

		virtual void	interrupt(void);
		virtual void	interrupt(unsigned char bit);

		bool			accessRam(unsigned short addr, bool read);
		void			waitCycles(unsigned char cycles);
		void			instruction_parser(unsigned char *code);

		void			setFlagZ(bool f);
		void			setFlagN(bool f);
		void			setFlagH(bool f);
		void			setFlagC(bool f);

		bool			getFlagZ(void);
		bool			getFlagN(void);
		bool			getFlagH(void);
		bool			getFlagC(void);

		void			calcFlagZ(unsigned short value);
		void			calcFlagN(unsigned short value);
		void			calcFlagH(unsigned short value);
		void			calcFlagC(unsigned short value);

		bool			matchFlag(unsigned char ftype);

		void			dma(void);

	private :

		void			swap(unsigned char* code);
		void			rrc(unsigned char* code);
		void			rlc(unsigned char* code);
		void			rr(unsigned char* code);
		void			rl(unsigned char* code);
		void			sla(unsigned char* code);
		void			sra(unsigned char* code);
		void			srl(unsigned char* code);
		void			set(unsigned char* code);
		void			res(unsigned char* code);
		void			bit(unsigned char* code);

		void			inc16(unsigned short *reg);
		void			inc8(unsigned short *reg, bool access);
		void			dec16(unsigned short *reg);
		void			dec8(unsigned short *reg, bool access);

		void			add16_data(unsigned char* code);
		void			add16(unsigned short b);
		void			add8(unsigned char *code);
		void			add8_data(unsigned char *code);
		void			adc8(unsigned char *code);
		void			adc8_data(unsigned char *code);
		void			sub8(unsigned char *code);
		void			sub8_data(unsigned char *code);
		void			sbc8(unsigned char *code);
		void			sbc8_data(unsigned char *code);
		void			and8(unsigned char *code);
		void			and8_data(unsigned char *code);
		void			or8(unsigned char *code);
		void			or8_data(unsigned char *code);
		void			xor8(unsigned char *code);
		void			xor8_data(unsigned char *code);
		void			cp8(unsigned char *code);
		void			cp8_data(unsigned char *code);
		void			cpl8(void);

		void			rlca(void);
		void			rla(void);
		void			rrca(void);
		void			rra(void);

		void			ld16_sp_hl(void);
		void			ld16_hl(unsigned char* code);
		void			ld16_address(unsigned char *code);
		void			ld16_read(unsigned char *code);
		void			ld16_write(unsigned char *code);
		void			ld16_in(unsigned char *code);
		void			ld16_out(unsigned char *code);
		void			ld16(unsigned short *reg, unsigned char *code);

		void			ld8_in(void);
		void			ld8_out(void);
		void			ld8_from_address(unsigned short *reg, int delta);
		void			ld8_address(unsigned short *reg, int delta);
		void			ld8_data(unsigned short *reg, unsigned char* code, bool access);
		void			ld8(unsigned char *code);

		void			jp(unsigned char ftype, unsigned char *code);
		void			jp(unsigned char *code);
		void			jp(void);
		void			jr(unsigned char ftype, unsigned char *code);
		void			jr(unsigned char *code);

		void			rst(unsigned char code);
		void			ret(void);
		void			reti(void);
		void			ret(unsigned char ftype);
		void			call(unsigned char *code);
		void			call(unsigned char ftype, unsigned char *code);

		void			push(unsigned short value);
		void			pop(unsigned short* value);

		void			stop(void);
		void			nop(void);
		void			halt(void);

		void			daa(void);
		void			ccf(void);
		void			scf(void);
		void			ei(void);
		void			di(void);

};

/******************************************************************/

class	GameboyLcd
{
	protected:

		unsigned int		_color;
		bool				_drawing;
		bool				_bgcolor;
		int					_current_line;
		unsigned int		_greypal[4];
		unsigned int		_buffer[2][256 * 224];
		unsigned int		_buffer_bgcolor[256];
		unsigned int*		_selectedBuffer;

		unsigned char		_output[360];

		unsigned char*		_tiledata;
		unsigned char*		_bgtilemap;
		unsigned char*		_wintilemap;

		unsigned char		_ly;
		unsigned char		_lyc;
		unsigned char		_lcdc;
		unsigned char		_stat;

		unsigned char*		_oam;
		unsigned char		_scx;
		unsigned char		_scy;
		unsigned char		_wx;
		unsigned char		_wy;

		void					initMainRegisters(void);

	public :

		std::function<void(unsigned char*)>		_callback;

		void					setCallback(std::function<void(unsigned char*)> fct);

		virtual void			init(void);
		virtual void			exit(void);
		virtual void			reload(void);
		virtual unsigned char*	getOutput(void) = 0;
		virtual void			drawLine(unsigned int y) = 0;

		static unsigned int		RGB(unsigned char r, unsigned char g, unsigned char b);

		unsigned char*			getBufferOffset(unsigned int x, unsigned int y);
};

class	GameboyLcdDMG: public GameboyLcd
{
	protected :

		unsigned int		_pal[4];
		unsigned int		_pal0[4];
		unsigned int		_pal1[4];

		unsigned int		_bgp;
		unsigned int		_obp0;
		unsigned int		_obp1;

	public :

		GameboyLcdDMG(void);
		virtual ~GameboyLcdDMG(void);

		void			init(void);
		void			reload(void);
		unsigned char*	getOutput(void);

		void			applyTileRow(unsigned char upper, unsigned char lower, unsigned char* out);
		void			drawLine(unsigned int y);
};

/******************************************************************/

class	GameboyIO
{
	protected :

		/*
		**	USER
		*/

		unsigned char		_p1;

		/*
		**	COM
		*/

		unsigned char		_sb;
		unsigned char		_sc;


	public :

		const unsigned char	KEY_RIGHT = 0x11;
		const unsigned char	KEY_LEFT = 0x12;
		const unsigned char	KEY_UP = 0x14;
		const unsigned char	KEY_DOWN = 0x18;
		const unsigned char	KEY_A = 0x21;
		const unsigned char	KEY_B = 0x22;
		const unsigned char	KEY_SELECT = 0x24;
		const unsigned char	KEY_START = 0x28;
		const unsigned char	RESET_KEYS = 0x30;

		GameboyIO(void);
		~GameboyIO(void);

		void			init(void);
		void			exit(void);
		void			reload(void);

		bool			isConnected();

		unsigned char	getUserInput(void);
		virtual void	serialTransfer();
};

/******************************************************************/

class	GameboyServer
{
	public :

		bool							_used;
		bool							_started;
		bool							_listening;
		SOCKET							_socket;
		size_t							_ncom;
		char*							_host;
		unsigned short					_port;
		std::map<SOCKET, SOCKADDR_IN>	_clients;
	
		GameboyServer(const size_t ncom = 5, unsigned short port = 11000);
		~GameboyServer(void);

		void			proceed(void);
		void			stop(void);
		bool			bind(unsigned short port);
		bool			accept(void);
		void			relay(void);
		void			send(unsigned char* bytes, SOCKET sender = 0);
		bool			allow(SOCKET id);
		bool			deny(SOCKET id);
		void			kick(SOCKET id);

};

typedef struct		s_servercom
{
	char*			host;
	unsigned short	port;
	bool			ready;
}					t_servercom;

class	GameboyClient
{
	public :

		std::vector<t_servercom*>	_servers;
		SOCKET			_socket;
		bool			_connected;
		unsigned short	_port;
		char*			_username;
		char*			_host;
	
		GameboyClient(void);
		~GameboyClient(void);

		static bool		ping(const char* hostname, unsigned short port);
		bool			disconnect(void);
		bool			connect(const char* hostname, unsigned short port);
		bool			send(const unsigned char* data, size_t len);
		bool			sync(const unsigned int freq);
		bool			recv(unsigned char* data, size_t *len);
};

/******************************************************************/

class	Gameboy
{
	protected :

		bool				_checksum;
		bool				_skipboot;
		bool				_running;
		unsigned char		_model;
		unsigned char		_transfer_counter;
		GameboyClient&		_me;

	public :

		class		Error: public std::exception
		{
			std::string			_err;
			int					_code;

			public :

				Error(const char* err, int code = 0);
				Error(const std::string& err, int code = 0);
				~Error(void);

				const char*	what(void);
		};

		static const unsigned char		DMG = 0;	//Monochrome
		static const unsigned char		MGB = 1;	//Monochrome Pocket
		static const unsigned char		MGL = 2;	//Monochrome Light
		static const unsigned char		CGB = 3;	//Color
		static const unsigned char		SGB = 4;	//Super

		unsigned char*	_ram;
		GameboyRom*		_rom;
		GameboyCpu*		_cpu;
		GameboyLcd*		_output;
		GameboyIO*		_input;

		Gameboy(void);
		~Gameboy(void);

		std::vector<t_compiler_method*>		dissasembler(void);

		bool			isReady(void);
		bool			isRunning(void);
		void			open(const char *filename);

		unsigned char	getModel(void);
		void			setModel(unsigned char model);

		void			loadState(const char* filename);
		void			saveState(const char* filename);

		void			transfer(void);
		void			serialSend(bool bit);
		unsigned char	serialRecv(void);
		void			etablishLink(const char* hostname, unsigned short port);
		bool			isConnected(void);

		void			pause(void);
		void			powerOn(void);
		void			powerOff(void);
		void			softReset(void);
		void			hardReset(void);

		void			boot(void);

		GameboyRom*		getRom(void);
		GameboyCpu*		getCpu(void);
		GameboyLcd*		getOutput(void);
		GameboyIO*		getInput(void);
		GameboyClient&	getClient(void);


	private :

		void			start(void);
};

#endif