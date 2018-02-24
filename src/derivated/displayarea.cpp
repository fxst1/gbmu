#include <guiderivated.hpp>
#include <gui.hpp>
#include <functional>

using namespace		std::placeholders;

DisplayArea::DisplayArea(BaseObjectType* cobject,
			const Glib::RefPtr<Gtk::Builder>& builder,
			Gameboy* g):
	Gtk::DrawingArea(cobject),
	_g(g)
{
	this->set_size_request(160, 144);
	this->show();
	this->_g->getOutput()->setCallback(
		std::bind(&DisplayArea::gpuOutput, this, _1)
	);
	this->_image = Gdk::Pixbuf::create(
		Gdk::Colorspace::COLORSPACE_RGB,
		true,
		8,
		160,
		140
	);
	(void)builder;
}

DisplayArea::~DisplayArea(void)
{}

void	DisplayArea::gpuOutput(unsigned char* vram)
{
	printf("%p\n", vram);
	if (!vram)
		return ;

	this->_image = Gdk::Pixbuf::create_from_data(
		vram,
		Gdk::Colorspace::COLORSPACE_RGB,
		true,
		8,
		160,
		140,
		0
	);

	this->queue_draw();
}

bool	DisplayArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cx)
{
	Gtk::Allocation		allocation = get_allocation();
	const int			width = allocation.get_width();
	const int			height = allocation.get_height();

	if (!this->_g->isRunning())
	{
		cx->set_source_rgb(255, 0, 0);
		cx->set_line_width(1.0);

		for (int i = 0; i < width; i += 2)
		{
			cx->move_to(i + 10, 0);
			cx->line_to(i + 10, height);
		}

		cx->stroke();
	}
	else
	{
		Gdk::Cairo::set_source_pixbuf(cx, this->_image, width, height);
	}

	return (true);
}
