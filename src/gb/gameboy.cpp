#include <gameboy.hpp>

Gameboy::Gameboy(void)
{
	this->color = false;
	this->model = Gameboy::DMG;
	this->game = NULL;
}

Gameboy::Gameboy(bool color)
{
	this->color = color;
	this->model = Gameboy::CGB;
	this->game = NULL;
}

bool	Gameboy::load(const char *filename)
{
	if (this->game == NULL)
		this->game = new Rom(filename);
	else
		this->game->load(filename);

	return (this->status);
}

bool	Gameboy::force(std::string model)
{
	if (model == "DMG")
		this->model = Gameboy::DMG;
	else if (model == "MGB")
		this->model = Gameboy::MGB;
	else if (model == "MGB")
		this->model = Gameboy::MGL;
	else if (model == "MGB")
		this->model = Gameboy::CGB;
	else
	{
		std::cerr << "Undefined model " << this->model << std::endl;	
		return (false);
	}

	this->get_model();
	return (true);
}

bool	Gameboy::get_model(void)
{
	switch (this->model)
	{
		case Gameboy::DMG:
			break;

		case Gameboy::MGB:
			break;

		case Gameboy::MGL:
			break;

		case Gameboy::CGB:
			break;

		case Gameboy::SGB:
			break;

		default:
			std::cerr << "Undefined model code " << std::to_string(this->model);
			return (false);
	};

	return (true);
}

bool	Gameboy::start()
{
	unsigned char	support_code;

	if (this->game == NULL || this->game->status == false || this->status == false)
		return (false);

	support_code = this->game->get_cgb_support_code();
	if (support_code == 0 && this->model == Gameboy::CGB)
	{
		std::cerr << "Unsupported CGB model" << std::endl;
		std::cerr << "> Rollback to MGB model" << std::endl;

		this->model = Gameboy::MGB;
	}
	else if (support_code == 0xC0 && this->model != Gameboy::CGB)
	{
		std::cerr << "Only supported CGB model" << std::endl;
		std::cerr << "> force CGB model" << std::endl;

		this->model = Gameboy::CGB;
	}

	support_code = this->game->get_sgb_support_code();
	if (support_code == 0x3 && this->model != Gameboy::SGB)
	{
		std::cerr << "Only supported CGB model" << std::endl;
		std::cerr << "> force CGB model" << std::endl;		

		this->model = Gameboy::SGB;
	}

	std::cout << "Starting CPU ..." << std::endl;
	return (true);
}
