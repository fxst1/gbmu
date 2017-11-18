#include <rom.hpp>

Rom::Rom(const char *filename)
{
	this->start_addr = 0;
	this->game_title = NULL;
	this->game_code = NULL;
	this->sgb_support_code = 0;
	this->cgb_support_code = 0;
	this->maker_code = NULL;
	this->sgb_support_code = 0;
	this->cassette_type = 0;
	this->rom_size = 0;
	this->ram_size = 0;
	this->dest_code = 0;
	this->mask_rom_version = 0;
	this->complement_check = 0;
	this->checksum = 0;

	this->status = false;
	this->len = 0;
	this->buffer = NULL;
	this->filename = (char*)filename;
	this->load(filename);
}

Rom::~Rom()
{
	if (this->status)
	{
		free(this->game_title);
		free(this->game_code);
		free(this->maker_code);
		free(this->buffer);
	}
}

bool		Rom::load(const char *filename)
{
	int		fd = -1;

	this->filename = (char*)filename;

	fd = open(this->filename, O_RDONLY);
	if (fd > 0)
	{
		this->start_addr = 0x150;
		this->status = this->load(fd);
		close(fd);
	}
	else
		this->status = false;

	return (this->status);
}

bool		Rom::load(const int fd)
{
	struct stat		stat_buf;
	long			size = 0;
	int				rc = fstat(fd, &stat_buf);

	size = (rc == 0 ? stat_buf.st_size : -1);
	if (size <= 0)
		return (false);

	this->status = true;
	this->buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
	bzero(this->buffer, sizeof(unsigned char) * size);

	this->len = (size_t)size;
	size = read(fd, this->buffer, size);
	if (this->buffer[0x100] != 0x0 || this->buffer[0x101] != 0xC3)
	{
		std::cerr << "Bad gameboy code : expected 00C3" << std::endl;
		this->status = false;
	}
	else
		this->info();

	return (true);
}

static unsigned char	*copystr(const unsigned char *buffer, size_t n)
{
	unsigned char		*buf = (unsigned char*)malloc(sizeof(unsigned char) * (n + 1));

	bzero(buf, sizeof(unsigned char) * (n + 1));

	while (n--)
		buf[n] = buffer[n];

	return (buf);
}

bool		Rom::info(void)
{
	if (this->status == true)
	{
		memcpy(&this->start_addr, this->buffer + 0x102, 2);
		this->game_title = copystr(this->buffer + 0x134, 11);
		this->game_code = copystr(this->buffer + 0x13f, 4);
		this->maker_code = copystr(this->buffer + 0x144, 2);
		this->sgb_support_code = *(this->buffer + 0x146);
		this->cgb_support_code = *(this->buffer + 0x143);
		this->cassette_type = *(this->buffer + 0x147);
		this->rom_size = *(this->buffer + 0x148);
		this->ram_size = *(this->buffer + 0x149);
		this->dest_code = *(this->buffer + 0x14A);
		this->mask_rom_version = *(this->buffer + 0x14C);
		this->complement_check = *(this->buffer + 0x14D);
		memcpy(&this->checksum, this->buffer + 0x14E, 2);

		if (this->cgb_support_code != 0 &&
			this->cgb_support_code != 0xC0 &&
			this->cgb_support_code != 0x80)
		{
			std::cerr << "Wrong CGB support code (0x0, 0xC0, 0x80)" << std::endl;
			std::cerr << " > Force CGB Compatible (0xC0)" << std::endl;			
		}

		if (this->sgb_support_code == 0x03 &&
			*(this->buffer + 0x14B) != 0x33)
		{
			std::cerr << "Missing 0x33 @0x14B when SGB is required" << std::endl;
			std::cerr << " > Force set 0x33 @0x14B" << std::endl;

			*(this->buffer + 0x14B) = 0x33;
		}
		else if (this->sgb_support_code != 0)
		{
			std::cerr << "Wrong SGB support code (0x0, 0x03)" << std::endl;
			std::cerr << " > Force SGB disabled (0x0)" << std::endl;
		}

		if (this->cassette_type == 0x4 || this->cassette_type == 0x7)
		{
			std::cerr << "Wrong Cartridge type" << std::endl;
			std::cerr << "> Force 0x0" << std::endl;
		}
	}
	else
	{
		std::cerr << "Cannot read header" << std::endl;
		return (false);
	}

	return (true);
}

bool		Rom::header()
{
	std::string		s = "";

	if (this->status == false)
		return (false);

	std::cout << "=== " << this->filename << " ===" << std::endl;
	std::cout << "= " << std::endl;
	std::cout << "= Title : " << this->game_title << std::endl;
	std::cout << "= Version : " << std::to_string(this->mask_rom_version) << std::endl;

	if (this->cgb_support_code == 0xC0)
		std::cout << "= CGB support : " << std::to_string(this->cgb_support_code) << " (CGB Exclusive)" << std::endl;
	else if (this->cgb_support_code == 0x80)
		std::cout << "= CGB support : " << std::to_string(this->cgb_support_code) << " (CGB Compatible)" << std::endl;
	else if (this->cgb_support_code == 0x0)
		std::cout << "= CGB support : " << std::to_string(this->cgb_support_code) << " (CGB Incompatible)" << std::endl;
	else
		std::cout << "= CGB support : " << std::to_string(this->cgb_support_code) << " (Unknown : force CGB Compatible)" << std::endl;

	if (this->sgb_support_code == 0x03)
		std::cout << "= SGB support : " << std::to_string(this->sgb_support_code) << " (Compatible SGB)" << std::endl;
	else if (this->sgb_support_code == 0x0)
		std::cout << "= SGB support : " << std::to_string(this->sgb_support_code) << " (SGB Only)" << std::endl;
	else
		std::cout << "= SGB support : " << std::to_string(this->sgb_support_code) << " (Unknown : force SGB Compatible)" << std::endl;

	std::cout << "= Game code : " << this->game_code << std::endl;
	std::cout << "= Maker code : " << this->maker_code << std::endl;

	if (this->cassette_type == 0x4 || this->cassette_type == 0x7)
		std::cout << "= Cassette type : " << std::to_string(this->cassette_type) << " (Unknown : Force ROM)" <<std::endl;
	else
		std::cout << "= Cassette type : " << std::to_string(this->cassette_type) << " (" << this->get_cartridge_string() << ")" <<std::endl;

	std::cout << "= " << std::endl;
	std::cout << "== " << std::endl;
	std::cout << "= " << std::endl;

	if (this->rom_size > 0x8)
		std::cout << "= Rom size : " << std::to_string(this->rom_size) << " (Unknown : Force 64Mbit)" << std::endl;
	else
		std::cout << "= Rom size : " << std::to_string(this->rom_size) << " (" << this->get_rom_size_string() << ")" << std::endl;

	if (this->ram_size == 0x1 || this->ram_size > 8)
		std::cout << "= Ram size : " << std::to_string(this->ram_size) << " (Unknown : Force 1 Mbit)" << std::endl;
	else
		std::cout << "= Ram size : " << std::to_string(this->ram_size) << " (" << this->get_ram_size_string() << ")" << std::endl;

	std::cout << "= Start @ : 0x" << std::to_string(this->start_addr) << std::endl;

	if (this->dest_code > 1)
		std::cout << "= Dest code : " << std::to_string(this->dest_code) << " (Unknown : Force All Others)" << std::endl;
	else
		std::cout << "= Dest code : " << std::to_string(this->dest_code) << " (" << this->get_dest_code_string() << ")" << std::endl;

	std::cout << "= " << std::endl;
	std::cout << "== " << std::endl;
	std::cout << "= " << std::endl;
	std::cout << "= Checksum : " << std::to_string(this->checksum) << " (" << this->calc_checksum() << ")" << std::endl;
	std::cout << "= > complement : " << std::to_string(this->complement_check) << std::endl;
	std::cout << "= " << std::endl;
	std::cout << "=== " << std::endl;

	return (true);
}

unsigned char	Rom::get_sgb_support_code()
{
	if (this->sgb_support_code == 0 ||
		this->sgb_support_code == 0x03)
		return (this->sgb_support_code);
	else
		return (0x0);
}

unsigned char	Rom::get_cgb_support_code()
{
	if (this->cgb_support_code == 0 ||
		this->cgb_support_code == 0xC0 ||
		this->cgb_support_code == 0x80)
		return (this->cgb_support_code);
	else
		return (0xC0);
}

size_t			Rom::get_ram_size()
{
	if (this->ram_size > 4 || this->ram_size == 1)
		return (1000000);
	else if (this->ram_size == 0)
		return (0);
	else if (this->ram_size == 2)
		return (64000);
	else if (this->ram_size == 3)
		return (256000);
	else if (this->ram_size == 4)
		return (1000000);
	return (0);
}

size_t			Rom::get_rom_size()
{
	size_t		size = (this->rom_size + 1) * 256;

	if (this->rom_size > 8)
		return (64000000);
	else
		return (size);
}

std::string		Rom::get_cartridge_string()
{
	std::string	s = "";

	s += "ROM";
	if (this->cassette_type >= 0x1 && this->cassette_type <= 0x3)
	{
		s += ", MBC-1";
		if (this->cassette_type == 0x3)
			s += ", SRAM, Backup Battery";
		else
			s += ", SRAM";
	}
	else if (this->cassette_type == 0x5)
		s += ", MBC-2";
	else if (this->cassette_type == 0x6)
		s += ", MBC-2, Backup Battery";
	else if (this->cassette_type == 0x8)
		s += ", SRAM";
	else if (this->cassette_type == 0x9)
		s += ", SRAM, Backup Battery";
	else if (this->cassette_type == 0xF)
		s += ", MBC-3 RTC, Backup Battery";
	else if (this->cassette_type == 0x10)
		s += ", MBC-3 RTC, SRAM, Backup Battery";
	else if (this->cassette_type == 0x11)
		s += ", MBC-3 NO RTC";
	else if (this->cassette_type == 0x12)
		s += ", MBC-3 NO RTC, SRAM";
	else if (this->cassette_type == 0x13)
		s += ", MBC-3 NO RTC, SRAM, Backup Battery";
	else if (this->cassette_type == 0x19)
		s += ", MBC-5 NO RUMBLE";
	else if (this->cassette_type == 0x1A)
		s += ", MBC-5 NO RUMBLE, SRAM";
	else if (this->cassette_type == 0x1B)
		s += ", MBC-5 NO RUMBLE, SRAM, Backup Battery";
	else if (this->cassette_type == 0x1C)
		s += ", MBC-5 RUMBLE";
	else if (this->cassette_type == 0x1D)
		s += ", MBC-5 RUMBLE, SRAM";
	else if (this->cassette_type == 0x1E)
		s += ", MBC-5 RUMBLE, SRAM, Backup Battery";

	return (s);
}

std::string		Rom::get_rom_size_string()
{
	std::string	s = "";
	size_t		size = (this->rom_size + 1) * 256;

	if (size >= 1000)
	{
		s += std::to_string(size / 1000);
		s += " MBit";
	}
	else
	{
		s += std::to_string(size);
		s += " KBit";
	}

	return (s);
}

std::string		Rom::get_ram_size_string()
{
	std::string	s = "";

	if (this->ram_size == 0)
		s += "No RAM or MBC-2";
	else if (this->ram_size == 2)
		s += "64 KBit";
	else if (this->ram_size == 3)
		s += "256 KBit";
	else if (this->ram_size == 4)
		s += "1 MBit";

	return (s);
}

std::string		Rom::get_dest_code_string()
{
	std::string	s = "";

	if (this->dest_code == 0)
		s += "Japan";
	else if (this->dest_code == 1)
		s += "All Others";

	return (s);
}

size_t			Rom::calc_checksum()
{
	size_t		i = 0x134;
	size_t		sum = 0;

	while (i <= 0x14C)
	{
		sum += *(this->buffer + i);
		i++;
	}

	sum += 0x19;
	sum += *(this->buffer + 0x14D);

	return (sum);
}