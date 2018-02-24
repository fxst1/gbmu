#include <gameboy.hpp>
#include <thread>

unsigned short	opcodes[MAX_OPCODES] = {0x0};
intptr_t		instructions[MAX_OPCODES] = {0x0};
unsigned char*	gbram = NULL;
Gameboy*		gbuser = NULL;
Gameboy*		gbuser2 = NULL;
GameboyServer	server = GameboyServer(5);
GameboyClient	client = GameboyClient();

Gameboy::Gameboy(void):
	_checksum(false),
	_skipboot(false),
	_running(false),
	_model(0),
	_transfer_counter(0),
	_me(client),
	_ram(NULL),
	_rom(new GameboyRom()),
	_cpu(new GameboyCpu()),
	_output(new GameboyLcdDMG()),
	_input(new GameboyIO())
{}

Gameboy::~Gameboy()
{}

bool			Gameboy::isRunning(void)
{
	return (this->_running);
}

bool			Gameboy::isReady(void)
{
	return (this->_rom->status == true);
}

void			Gameboy::open(const char* filename)
{
	this->_rom->load(filename);
}

unsigned char	Gameboy::getModel(void)
{
	return (this->_model);
}

void			Gameboy::setModel(unsigned char model)
{
	if (this->_model <= Gameboy::SGB)
		this->_model = model;
	else
		throw Gameboy::Error("Undefined model");
}

void			Gameboy::loadState(const char* filename)
{
	(void)filename;
	throw Gameboy::Error("Unsupported load state");
	return ;
}

void			Gameboy::saveState(const char* filename)
{
	(void)filename;
	throw Gameboy::Error("Unsupported save state");
	return ;
}

void			Gameboy::transfer(void)
{
	unsigned char	code;
	bool			bit;

	if (this->_transfer_counter == 0)
		return ;

	if (*(gbram + 0xFF42) & 1)
	{
		try
		{
			code = *(gbram + 0xFF41);
			bit = (code >> 7) & 1;
			*(gbram + 0xFF41) = (code << 1) & 0xFF; 
			this->serialSend(bit);
			this->_transfer_counter--;
		}
		catch (Gameboy::Error& e)
		{
			std::cerr << "Transfer Failed (master): " << e.what() << std::endl;			
		}
	}
	else
	{
		try
		{
			bit = this->serialRecv();
			code = *(gbram + 0xFF41);
			code = code << 1;
			*(gbram + 0xFF41) = code & 0xFF;
			this->_transfer_counter--;
		}
		catch (Gameboy::Error& e)
		{
			std::cerr << "Transfer Failed (slave): " << e.what() << std::endl;			
		}
	}
}

void			Gameboy::serialSend(bool bit)
{
	unsigned char	buf = (unsigned char)bit;

	if (!this->_me.send(&buf, 1))
	{
		throw Gameboy::Error("Cannot send " + std::string(bit ? "0x1" : "0x0"));
	}
}

unsigned char	Gameboy::serialRecv(void)
{
	unsigned char	buf = 0;
	size_t			len = 0;

	if (!this->_me.recv(&buf, &len))
		return (buf & 0x1);
	throw Gameboy::Error("Cannot receive");
	return (0x2);
}

bool			Gameboy::isConnected(void)
{
	return (this->_me._socket != INVALID_SOCKET);
}

void			Gameboy::etablishLink(const char* hostname, unsigned short port)
{
	this->_me.connect(hostname, port);
}

void			Gameboy::pause(void)
{
	this->_running = false;
	if (!this->isReady())
		return ;
}

void			Gameboy::powerOn(void)
{
	if (!this->isReady())
		return ;

	if (!this->_running)
	{
		this->_cpu->init();
		this->_output->init();
		this->_input->init();
		this->_running = true;
		this->_ram = gbram;
		gbram = this->_rom->get_buffer();
		std::thread	th(&Gameboy::start, this);

		th.detach();
	}
}

void			Gameboy::powerOff(void)
{
	if (this->_running)
	{
		//this->_rom.exit();
		this->_cpu->exit();
		this->_output->exit();
		this->_input->exit();
	}
	this->_running = false;
}

void			Gameboy::softReset(void)
{
	if (this->_running)
	{
		this->_cpu->init();
		this->_output->init();
		this->start();
	}
}

void			Gameboy::hardReset(void)
{
	if (this->_running)
	{
		//this->_rom.init();
		this->_cpu->init();
		this->_output->init();
		this->start();
	}
}

void			Gameboy::start(void)
{
	gbuser = this;
	if (!this->_skipboot)
		this->boot();

	while (this->_cpu->_clockfreq && this->isRunning())
	{
		this->_cpu->frameskip();
		this->_cpu->execute();
		this->_cpu->dma();
		if (*(gbram + 0xFF40) & (1 << 7))
			this->_output->_callback(this->_output->getOutput());
	}
}

void			Gameboy::boot(void)
{
	while (this->_cpu->_clockfreq && this->isRunning())
	{
		if ((this->_cpu->_pc == this->_rom->start_addr) &&
			(this->_cpu->_pc > DMGbiosSize))
		{
			std::cerr << "START ADDR" << std::endl;
			return ;
		}

		this->_cpu->frameskip();
		this->_cpu->execute((unsigned char*)DMGbios);
		this->_cpu->dma();
		if (*(gbram + 0xFF40) & (1 << 7))
			this->_output->_callback(this->_output->getOutput());
	}
}

GameboyRom*		Gameboy::getRom(void)
{
	return (this->_rom);
}

GameboyCpu*		Gameboy::getCpu(void)
{
	return (this->_cpu);
}

GameboyLcd*		Gameboy::getOutput(void)
{
	return (this->_output);
}

GameboyIO*		Gameboy::getInput(void)
{
	return (this->_input);
}

GameboyClient&	Gameboy::getClient(void)
{
	return (this->_me);
}
