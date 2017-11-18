#include <rom.hpp>

int	main(int argc, char **argv)
{
	Rom			*rom = NULL;

	if (argc == 2)
	{
		rom = new Rom(argv[1]);

		if(rom->status == false)
		{
			std::cerr << "An error occured" << std::endl;
		}
		else
			rom->header();

		delete rom;
	}

	return (0);
}