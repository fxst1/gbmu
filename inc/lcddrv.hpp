#ifndef LCDDRV_HPP
# define LCDDRV_HPP
# include <stdlib.h>
/*
	class		LcdDriver
	{
		protected :

			unsigned int	greypal[4];
			int				current_line;
			unsigned int	buffer[2][256 * 224];
			unsigned int	buffer_bgcolor[256];
			unsigned int	*selectedBuffer;

			unsigned char	*tiledata;
			unsigned char	*bgtilemap;
			unsigned char	*wintilemap;

			unsigned char	*oam;
			unsigned int	lcdc;
			unsigned int	scx;
			unsigned int	scy;
			unsigned int	wx;
			unsigned int	wy;

			unsigned int	color;
			bool			drawing;
			bool			bgcolor;

		public :

			unsigned char	*addr;
			bool			status;

			LcdDriver(unsigned char *rssp = NULL);
			~LcdDriver();

			virtual void	loadPal(void)
			{}
			virtual void	draw(unsigned int y)
			{(void)y;}

			unsigned char	*getVram(void);
			void			load(void);
			void			reset(unsigned char *rssp);
			void			halt(unsigned int cpuhalt, unsigned int y);	
			static unsigned int	RGB(unsigned char red, unsigned char green, unsigned char blue);
	};

	class		LcdDriverDMG: public LcdDriver
	{
		unsigned int	pal[4];
		unsigned int	pal0[4];
		unsigned int	pal1[4];

		unsigned int	bgp;
		unsigned int	obp0;
		unsigned int	obp1;

		public :

			LcdDriverDMG(unsigned char *rssp = NULL);
			virtual ~LcdDriverDMG();

			void	loadPal(void);
			void	draw(unsigned int y);
			void	drawTile(unsigned int x, unsigned int y, unsigned int tile);
			void	drawSprite(int x, unsigned int baseindex, unsigned char *sprite, unsigned char *data);
			void	loadSprites(unsigned int y, unsigned int baseindex);
	};
*/
	/*class		LcdDriverCGB: public LcdDriver
	{
		public :

			LcdDriverCGB(unsigned char *rssp = NULL);
			virtual ~LcdDriverCGB();

			void	loadPal(void);
			void	draw(unsigned int y);
	};*/

#endif