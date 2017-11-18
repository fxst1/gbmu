#ifndef ROM_HPP
# define ROM_HPP
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <iostream>

	class	Rom
	{
		public:

			bool			status;

			Rom(const char *filename);
			~Rom();

			bool			load(const int fd);
			bool			load(const char *filename);
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


		private :

			char			*filename;
			unsigned char	*buffer;
			size_t			len;

			unsigned short	start_addr;
			unsigned char	*game_title;
			unsigned char	*game_code;
			unsigned char	cgb_support_code;
			unsigned char	*maker_code;
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

#endif