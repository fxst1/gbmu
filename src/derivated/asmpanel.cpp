#include <guiderivated.hpp>

AsmPanel::AsmPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
					Gameboy *gb):
	Gtk::Window(cobject),
	_refGlade(builder),
	_gbPtr(gb)
{
	this->_syntax = gb->dissasembler();

	this->_nums = std::vector<Gtk::Label>();
	this->_addrs = std::vector<Gtk::Label>();

	this->_refGlade->get_widget("asm-buffer", this->_textBuf);
	this->_refGlade->get_widget("asm-tags", this->_tagTable);

	this->_refGlade->get_widget("asm-tag-error", this->_tag_error);
	this->_refGlade->get_widget("asm-tag-normal", this->_tag_normal);
	this->_refGlade->get_widget("asm-tag-header", this->_tag_header);
	this->_refGlade->get_widget("asm-tag-register", this->_tag_register);
	this->_refGlade->get_widget("asm-tag-value", this->_tag_value);
	this->_refGlade->get_widget("asm-tag-current", this->_tag_current);
	this->_refGlade->get_widget("asm-tag-comment", this->_tag_comment);

	this->_refGlade->get_widget("line-addr", this->_lines_addr);
	this->_refGlade->get_widget("line-num", this->_lines_num);

	this->_refGlade->get_widget("text-asm", this->_text_asm);
	this->_refGlade->get_widget("text-hex", this->_text_hex);

	this->_text_asm->signal_composited_changed().connect(
		sigc::mem_fun(
			this, &AsmPanel::eventTextChanged
		)
	);

	this->build();
}

AsmPanel::~AsmPanel(void)
{
	delete this->_textBuf;
	delete this->_tagTable;

	delete this->_tag_error;
	delete this->_tag_normal;
	delete this->_tag_header;
	delete this->_tag_register;
	delete this->_tag_value;
	delete this->_tag_current;
	delete this->_tag_comment;

	delete this->_lines_addr;
	delete this->_lines_num;

	delete this->_text_asm;
	delete this->_text_hex;
}

void		AsmPanel::build(void)
{
	std::string		str = "";
	unsigned int	x = 0x150;
	unsigned int	line = 1;
	unsigned char	*code = this->_gbPtr->getRom()->get_code();
	size_t			len = 0;

	for (auto it = this->_lines_addr->get_children().begin();
			  it != this->_lines_addr->get_children().end();
			  it++)
		this->_lines_addr->remove( **it );

	for (auto it = this->_lines_num->get_children().begin();
			  it != this->_lines_num->get_children().end();
			  it++)
		this->_lines_num->remove( **it );

	while (x < 0x8000)
	{
		char	buf[16] = {0};

		this->_nums.push_back(Gtk::Label(buf));
		this->_addrs.push_back(Gtk::Label(std::to_string(line)));

		this->_lines_addr->add( this->_addrs[line] );
		this->_lines_num->add( this->_nums[line] );

		str += this->_gbPtr->getCpu()->disassembly_line(code + x, this->_syntax, &len);	
		x += len;
		line++;
	}

	this->_textBuf->set_text(str);
}

void		AsmPanel::eventTextChanged(void)
{
	this->build();
}
