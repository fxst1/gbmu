#include <gameboy.hpp>

Gameboy::Error::Error(const char* err, int code)
{
	this->_err = std::string(err);
	this->_code = code;
}

Gameboy::Error::Error(const std::string& err, int code)
{
	this->_err = std::string(err);
	this->_code = code;
}

Gameboy::Error::~Error(void)
{}

const char*		Gameboy::Error::what(void)
{
	return (this->_err.c_str());
}
