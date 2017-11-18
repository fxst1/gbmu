#ifndef MEMORY_HPP
# define MEMORY_HPP

	class	Memory
	{
		public:

			Memory();

			const		unsigned int SizeMax = 0x7FFF;

			void		address(unsigned int addr);
			void		rst(unsigned int addr);	//0x0 - 0x0FF
			void		data(unsigned int addr);	//0x100 - 0x14F
			void		start();					//0x150

		private:

			void	*ram;
	}

#endif