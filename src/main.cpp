#include <gameboy.hpp>
#include <gui.hpp>
#include <console.hpp>

static void		loadGUI(int argc, char **argv)
{
	try
	{
		Gui			*gui = new Gui(argc, argv);
		int			ret;

		ret = gui->run(argc, argv);
		delete gui;
		exit(ret);
	}
	catch (Gtk::BuilderError& e)
	{
		Gui::fatalError(e.what());
	}
}

static void		loadConsoleMode(int argc, char **argv)
{
	GameboyConsole	gui = GameboyConsole(argc, argv);

	gui.run();
}

static void		cpuOnly(const char *filename)
{
	Gameboy			g = Gameboy();

	g.open(filename);
	g.powerOn();
	while (g.isRunning())
	{
		;
	}
}

int				main(int argc, char **argv)
{
	char	**tmp = argv + 1;
	bool	ok = false;

	while (*tmp)
	{
		std::cout << *tmp << std::endl;
		if (!strcmp(*tmp, "--console"))
		{
			ok = true;
			loadConsoleMode(argc, argv);
			break ;
		}
		else if (!strcmp(*tmp, "--cpuonly"))
		{
			if (!(*tmp + 1))
			{
				std::cerr << "File expected" << std::endl;
				exit(EXIT_FAILURE);
			}
			ok = true;
			cpuOnly(*(tmp + 1));
			break ;
		}
		tmp++;
	}

	if (!ok)
		loadGUI(argc, argv);

	return (0);
}