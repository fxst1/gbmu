#ifndef DISPLAY_HPP
# define DISPLAY_HPP
/*
# include <cairomm/context.h>
# include <gdkmm/general.h>
# include <gameboy_data.h>
# include <rom.hpp>
# include <gtkmm/drawingarea.h>
# include <lcddrv.hpp>

class	Display : public Gtk::DrawingArea
{
	public :

		LcdDriver		*drv;

		Display(bool allowbank1);
		virtual ~Display();

		void			setResolution(std::string res);
		std::string		getResolution(void);
		std::string		*getResolutions(void);

		void			refresh(void);
		void			reset(unsigned char *addr);
		void			allowBank(bool allow);

	protected :

		bool			on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

	private :

		Glib::RefPtr<Gdk::Pixbuf> buf;
		Cairo::RefPtr<Cairo::Context> ctx;

		unsigned int	ratioX(void);
		unsigned int	ratioY(void);

		void			drawString();
		void			drawPixel();

		unsigned int	width;
		unsigned int	height;

};
*/
#endif