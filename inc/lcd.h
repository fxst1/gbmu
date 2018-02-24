#ifndef	LCD_H
# define	LCD_H
# include	<string.h>
# include	<gameboy_data.h>

void			load_lcd(unsigned char *ram, t_gb_lcd *disp);
void			load_oam(unsigned char *ram, t_gb_obj *objects);
void			load_obj(unsigned char *ram, unsigned int index, t_gb_obj *objects);

typedef struct		s_lcddrv
{
	unsigned int	*framebuffer[2];
	void			(*drawLine)(unsigned char *, unsigned int *, unsigned int);
}					t_lcddrv;


void				lcddrv_DMG_draw(unsigned char *, unsigned int *, unsigned int);
void				lcddrv_CGB_draw(unsigned char *, unsigned int *, unsigned int);
void				lcddrv_SGB_draw(unsigned char *, unsigned int *, unsigned int);

#endif