#ifndef DRIVER_HPP
# define DRIVER_HPP

	class		Driver
	{
		intptr_t	_addr;

		public :

			Driver(intptr_t addr);
			~Driver(void);

			void	reload(intptr_t addr);
			void	proceed(void);
	};

#endif