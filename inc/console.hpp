#ifndef CONSOLEMODE_HPP
# define CONSOLEMODE_HPP
# include <gameboy.hpp>

	class	GameboyConsole
	{
		Gameboy		*_gb;

		public :

			GameboyConsole(int argc, char **argv)
			{
				this->_gb = new Gameboy();
				argv++;
				while (*argv)
				{
					if (strcmp(*argv, "--file") == 0)
					{
						if (!*(argv + 1))
						{
							std::cerr << "File expected" << std::endl;
							::exit(EXIT_FAILURE);
						}
						this->_gb->getRom()->load(*(argv + 1));
						argv++;
					}
					else if (strcmp(*argv, "--connect") == 0)
					{
						if (!*(argv + 1) || !*(argv + 2))
						{
							std::cerr << "hostname and port are expected" << std::endl;
							::exit(EXIT_FAILURE);
						}
						this->_gb->getClient().connect(*(argv + 1), atoi(*(argv + 2)));	
						argv += 2;
					}
					else
						argv++;
				}
				(void)argc;
			}

			~GameboyConsole(void)
			{
				delete this->_gb;
			}
	
			void		run(void)
			{
				this->_gb->powerOn();
				while (this->_gb->isRunning())
				{
					;
				}
			}
	};

#endif