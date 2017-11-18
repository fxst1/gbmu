#ifndef GAMEBOY_HPP
# define GAMEBOY_HPP
# include <rom.hpp>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <iostream>
# include <string>

class	Gameboy
{
	public :

		static const unsigned char		DMG = 0;	//Monochrome
		static const unsigned char		MGB = 1;	//Monochrome Pocket
		static const unsigned char		MGL = 2;	//Monochrome Light
		static const unsigned char		CGB = 3;	//Color
		static const unsigned char		SGB = 4;	//Super

		Gameboy();				//FORCE MGB
		Gameboy(bool color);	//FORCE CGB

		bool	force(std::string model);
		bool	load(const char *filename);
		bool	start(void);

	private :

		bool			color;
		bool			status;
		unsigned char	model;
		Rom				*game;

		bool			get_model(void);
};

#endif