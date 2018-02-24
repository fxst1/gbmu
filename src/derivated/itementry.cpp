#include <guiderivated.hpp>

ItemEntry::ItemEntry(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder
						, intptr_t addr, size_t len, const Glib::ustring& regex):
	Gtk::Entry(cobject),
	_addr(addr),
	_len(len),
	_regex(Glib::Regex::create(regex)),
	_backup(NULL)
{
	this->registerBackup();
	(void)builder;
}

ItemEntry::~ItemEntry(void)
{
	free(this->_backup);
}

void			ItemEntry::refresh(void)
{
	unsigned int	v = 0;
	char			buf[32] = {0};

	memcpy(&v, (void*)this->_addr, this->_len < 4 ? this->_len : 4);
	if (this->_len == 4)
		sprintf(buf, "%x", v);
	else if (this->_len == 2)
		sprintf(buf, "%hx", v);
	else
		sprintf(buf, "%hhx", v);
	this->set_text(std::string(buf));
}

void			ItemEntry::registerBackup(const void *buf, size_t len)
{
	if (buf && len)
	{
		free(this->_backup);
		this->_len = len;
		this->_backup = (unsigned char*)malloc(this->_len);
		memcpy(this->_backup, buf, this->_len);
	}
	else if (this->_len)
	{
		free(this->_backup);
		this->_backup = (unsigned char*)malloc(this->_len);
		memcpy(this->_backup, (void*)this->_addr, this->_len);
	}
	else
		this->_backup = NULL;
}

void			ItemEntry::loadBackup(void *buf, size_t len)
{
	len = this->_len < len ? this->_len : len;

	if (this->_backup && buf && len)
		memcpy(buf, this->_backup, len);
	else
		bzero(buf, len);
}

std::string		ItemEntry::getValueString(void)
{
	Glib::ustring	s = this->get_text();
	Glib::MatchInfo	info;

	if (this->_regex->match(s, info))
	{
		this->registerBackup(s.c_str(), s.length());
		return (std::string(s.c_str()));
	}
	return (std::string((char*)this->_backup));
}

unsigned int	ItemEntry::getValueU32(void)
{
	Glib::ustring	s = this->get_text();
	Glib::MatchInfo	info;
	unsigned int	ret;

	if (this->_regex->match(s, info))
	{
		ret = (unsigned int)atoi(s.c_str());
		this->registerBackup(&ret, 4);
		return (ret);
	}
	else
		this->loadBackup(&ret, 4);
	return (ret);
}

unsigned short	ItemEntry::getValueU16(void)
{
	Glib::ustring	s = this->get_text();
	Glib::MatchInfo	info;
	unsigned short	ret;

	if (this->_regex->match(s, info))
	{
		ret = (unsigned short)atoi(s.c_str());
		this->registerBackup(&ret, 2);
		return (ret);
	}
	else
		this->loadBackup(&ret,	 2);
	return (ret);
}

unsigned char	ItemEntry::getValueU8(void)
{
	Glib::ustring	s = this->get_text();
	Glib::MatchInfo	info;
	unsigned char	ret;

	if (this->_regex->match(s, info))
	{
		ret = (unsigned short)atoi(s.c_str());
		this->registerBackup(&ret, 1);
		return (ret);
	}
	else
		this->loadBackup(&ret, 1);
	return (ret);
}
