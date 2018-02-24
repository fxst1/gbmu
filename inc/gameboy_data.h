#ifndef GAMEBOY_CHARACTER_HPP
# define GAMEBOY_CHARACTER_HPP

typedef struct		s_gb_char
{
	char			c;
}					t_gb_char;

//typedef unsigned char buffer[8][8] t_gb_tile;

typedef struct		s_gb_pal
{
	
}					t_gb_pal;

typedef struct		s_gb_pal_attr
{
	bool			flag;
	unsigned char	pal_data_no;
	unsigned char	pal_no;
	bool			pal_next_flag;
}					t_gb_pal_attr;

typedef struct		s_gb_obj
{
	unsigned char	lcd_x;
	unsigned char	lcd_y;
	unsigned char	chr_code;
	unsigned int	pal_color : 3;
	bool			pal_cbank;
	bool			pal_spec;
	bool			h_flip;
	bool			v_flip;
	bool			priority;
}					t_gb_obj;

typedef struct		s_gb_chr
{
	unsigned char	code;
	bool			priority;
	bool			up;
	bool			lr;
	bool			bank;
	unsigned int	pal : 3;
}					t_gb_chr;

typedef struct		s_gb_lcd
{
	unsigned short	scx;
	unsigned short	scy;
	unsigned short	wx;
	unsigned short	wy;
	unsigned short	ly;
	unsigned short	lyc;

	bool			bg_flag;
	bool			obj_flag;
	unsigned short	obj_block_x;
	unsigned short	obj_block_y;

	unsigned char	*select_area;
	unsigned char	*select_cdata;

	bool			windowing_flag;
	unsigned char	*select_warea;

	bool			stop_flag;
	bool			match_flag;
	bool			interupt_flag;
	unsigned short	mode_flag;

	unsigned char	bg_data;
	unsigned char	obj_data;

	t_gb_pal_attr	bg;
	t_gb_pal_attr	obj;

	t_gb_obj		objects[40];
}					t_gb_lcd;

#endif