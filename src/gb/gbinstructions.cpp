#include <gameboy.hpp>

void	GameboyCpu::instruction_parser(unsigned char *code)
{
	if (*code == 0x0)
		this->nop();
	else if (*code == 0x10)
		this->stop();
	else if (*code == 0x20)
		this->jr(GameboyCpu::FLAG_NZ, code);
	else if (*code == 0x30)
		this->jr(GameboyCpu::FLAG_NC, code);
	else if (*code == 0x76)
		this->halt();
	else if ((*code >= 0x40) && (*code <= 0x7F))
		this->ld8(code);
	else if ((*code & 0xF0) == 0x80)
	{
		if ((*code & 0x0F) >= 0x08)
			this->adc8(code);
		else
			this->add8(code);
	}
	else if ((*code & 0xF0) == 0x90)
	{
		if ((*code & 0x0F) >= 0x08)
			this->sbc8(code);
		else
			this->sub8(code);
	}
	else if ((*code & 0xF0) == 0xA0)
	{
		if ((*code & 0x0F) >= 0x08)
			this->and8(code);
		else
			this->xor8(code);
	}
	else if ((*code & 0xF0) == 0xB0)
	{
		if ((*code & 0x0F) >= 0x08)
			this->or8(code);
		else
			this->cp8(code);
	}
	else if (*code == 0xC0)
		this->ret(GameboyCpu::FLAG_NZ);
	else if (*code == 0xD0)
		this->ret(GameboyCpu::FLAG_NC);
	else if (*code == 0xE0)
		this->ld16_in(code);
	else if (*code == 0xF0)
		this->ld16_out(code);

	else if (*code == 0x01)
		this->ld16(&this->_bc, code);
	else if (*code == 0x11)
		this->ld16(&this->_de, code);
	else if (*code == 0x21)
		this->ld16(&this->_hl, code);
	else if (*code == 0x31)
		this->ld16(&this->_sp, code);
	else if (*code == 0xC1)
		this->pop(&this->_bc);
	else if (*code == 0xD1)
		this->pop(&this->_de);
	else if (*code == 0xE1)
		this->pop(&this->_hl);
	else if (*code == 0xF1)
		this->pop(&this->_af);

	else if (*code == 0x02)
		this->ld8_address(&this->_bc, 0);
	else if (*code == 0x12)
		this->ld8_address(&this->_de, 0);
	else if (*code == 0x22)
		this->ld8_address(&this->_hl, 1);
	else if (*code == 0x32)
		this->ld8_address(&this->_hl, -1);
	else if (*code == 0xC2)
		this->jp(GameboyCpu::FLAG_NZ, code);
	else if (*code == 0xD2)
		this->jp(GameboyCpu::FLAG_NC, code);
	else if (*code == 0xE2)
		this->ld8_in();
	else if (*code == 0xF2)
		this->ld8_out();

	else if (*code == 0x03)
		this->inc16(&this->_bc);
	else if (*code == 0x13)
		this->inc16(&this->_de);
	else if (*code == 0x23)
		this->inc16(&this->_hl);
	else if (*code == 0x33)
		this->inc16(&this->_sp);
	else if (*code == 0xC3)
		this->jp(code);
	else if (*code == 0xF3)
		this->di();

	else if (*code == 0x04)
		this->inc8(&this->_bc, false);
	else if (*code == 0x14)
		this->inc8(&this->_de, false);
	else if (*code == 0x24)
		this->inc8(&this->_hl, false);
	else if (*code == 0x34)
		this->inc8(&this->_hl, true);
	else if (*code == 0xC4)
		this->call(GameboyCpu::FLAG_NZ, code);
	else if (*code == 0xD4)
		this->call(GameboyCpu::FLAG_NC, code);

	else if (*code == 0x05)
		this->dec8(&this->_bc, false);
	else if (*code == 0x15)
		this->dec8(&this->_de, false);
	else if (*code == 0x25)
		this->dec8(&this->_hl, false);
	else if (*code == 0x35)
		this->dec8(&this->_hl, true);
	else if (*code == 0xC5)
		this->push(this->_bc);
	else if (*code == 0xD5)
		this->push(this->_de);
	else if (*code == 0xE5)
		this->push(this->_hl);
	else if (*code == 0xF5)
		this->push(this->_af);

	else if (*code == 0x06)
		this->ld8_data(&this->_bc, code, false);
	else if (*code == 0x16)
		this->ld8_data(&this->_de, code, false);
	else if (*code == 0x26)
		this->ld8_data(&this->_hl, code, false);
	else if (*code == 0x36)
		this->ld8_data(&this->_hl, code, true);
	else if (*code == 0xC6)
		this->add8_data(code);
	else if (*code == 0xD6)
		this->sub8_data(code);
	else if (*code == 0xE6)
		this->and8_data(code);
	else if (*code == 0xF6)
		this->or8_data(code);

	else if (*code == 0x07)
		this->rlca();
	else if (*code == 0x17)
		this->rla();
	else if (*code == 0x27)
		this->daa();
	else if (*code == 0x37)
		this->scf();
	else if (*code == 0xC7)
		this->rst(0x00);
	else if (*code == 0xD7)
		this->rst(0x10);
	else if (*code == 0xE7)
		this->rst(0x20);
	else if (*code == 0xF7)
		this->rst(0x30);

	else if (*code == 0x08)
		this->ld16_address(code);
	else if (*code == 0x18)
		this->jr(code);
	else if (*code == 0x28)
		this->jr(GameboyCpu::FLAG_Z, code);
	else if (*code == 0x38)
		this->jr(GameboyCpu::FLAG_C, code);
	else if (*code == 0xC8)
		this->ret(GameboyCpu::FLAG_Z);
	else if (*code == 0xD8)
		this->ret(GameboyCpu::FLAG_NZ);
	else if (*code == 0xE8)
		this->add16_data(code);
	else if (*code == 0xF8)
		this->ld16_hl(code);

	else if (*code == 0x09)
		this->add16(this->_bc);
	else if (*code == 0x19)
		this->add16(this->_de);
	else if (*code == 0x29)
		this->add16(this->_hl);
	else if (*code == 0x39)
		this->add16(this->_sp);
	else if (*code == 0xC9)
		this->ret();
	else if (*code == 0xD9)
		this->reti();
	else if (*code == 0xE9)
		this->jp();
	else if (*code == 0xF9)
		this->ld16_sp_hl();

	else if (*code == 0x0A)
		this->ld8_from_address(&this->_bc, 0);
	else if (*code == 0x1A)
		this->ld8_from_address(&this->_de, 0);
	else if (*code == 0x2A)
		this->ld8_from_address(&this->_hl, 1);
	else if (*code == 0x3A)
		this->ld8_from_address(&this->_hl, -1);
	else if (*code == 0xCA)
		this->jp(GameboyCpu::FLAG_Z, code);
	else if (*code == 0xDA)
		this->jp(GameboyCpu::FLAG_C, code);
	else if (*code == 0xEA)
		this->ld16_write(code);
	else if (*code == 0xFA)
		this->ld16_read(code);

	else if (*code == 0x0B)
		this->dec16(&this->_bc);
	else if (*code == 0x1B)
		this->dec16(&this->_de);
	else if (*code == 0x2B)
		this->dec16(&this->_hl);
	else if (*code == 0x3B)
		this->dec16(&this->_sp);
	else if (*code == 0xCB)
	{
		code++;
		if ((*code & 0xF0) == 0x00)
		{
			if (*code < 0x08)
				this->rlc(code);
			else
				this->rrc(code);
		}
		else if ((*code & 0xF0) == 0x10)
		{
			if (*code < 0x08)
				this->rl(code);
			else
				this->rr(code);
		}
		else if ((*code & 0xF0) == 0x20)
		{
			if (*code < 0x08)
				this->sla(code);
			else
				this->sra(code);
		}
		else if ((*code & 0xF0) == 0x30)
		{
			if (*code < 0x08)
				this->swap(code);
			else
				this->srl(code);
		}
		else if (*code >= 0x40 && *code <= 0x7F)
			this->bit(code);
		else if (*code >= 0x80 && *code <= 0xBF)
			this->res(code);
		else if (*code >= 0xC0)
			this->set(code);
		else
			std::cerr << "CB error" << std::endl;
	}
	else if (*code == 0xFB)
		this->ei();

	else if (*code == 0x0C)
		this->inc8((unsigned short*)((unsigned char*)&this->_bc) + 1, false);
	else if (*code == 0x1C)
		this->inc8((unsigned short*)((unsigned char*)&this->_de) + 1, false);
	else if (*code == 0x2C)
		this->inc8((unsigned short*)((unsigned char*)&this->_hl) + 1, false);
	else if (*code == 0x3C)
		this->inc8((unsigned short*)((unsigned char*)&this->_af), false);
	else if (*code == 0xCC)
		this->call(GameboyCpu::FLAG_Z, code);
	else if (*code == 0xDC)
		this->call(GameboyCpu::FLAG_C, code);

	else if (*code == 0x0D)
		this->dec8((unsigned short*)((unsigned char*)&this->_bc) + 1, false);
	else if (*code == 0x1D)
		this->dec8((unsigned short*)((unsigned char*)&this->_de) + 1, false);
	else if (*code == 0x2D)
		this->dec8((unsigned short*)((unsigned char*)&this->_hl) + 1, false);
	else if (*code == 0x3D)
		this->dec8((unsigned short*)((unsigned char*)&this->_af), false);
	else if (*code == 0xCD)
		this->call(code);

	else if (*code == 0x0E)
		this->ld8_data((unsigned short*)((unsigned char*)&this->_bc) + 1, code, false);
	else if (*code == 0x1E)
		this->ld8_data((unsigned short*)((unsigned char*)&this->_de) + 1, code, false);
	else if (*code == 0x2E)
		this->ld8_data((unsigned short*)((unsigned char*)&this->_hl) + 1, code, false);
	else if (*code == 0x3E)
		this->ld8_data((unsigned short*)((unsigned char*)&this->_af), code, true);
	else if (*code == 0xCE)
		this->adc8_data(code);
	else if (*code == 0xDE)
		this->sbc8_data(code);
	else if (*code == 0xEE)
		this->xor8_data(code);
	else if (*code == 0xFE)
		this->cp8_data(code);

	else if (*code == 0x0F)
		this->rrca();
	else if (*code == 0x1F)
		this->rra();
	else if (*code == 0x2F)
		this->cpl8();
	else if (*code == 0x3F)
		this->ccf();
	else if (*code == 0xCF)
		this->rst(0x08);
	else if (*code == 0xDF)
		this->rst(0x18);
	else if (*code == 0xEF)
		this->rst(0x28);
	else if (*code == 0xFF)
		this->rst(0x38);

	else
	{
		this->_clockfreq = 0;
		std::cout << "Bad opcode " << std::to_string(*code) << std::endl;
	}
}

static std::string		getdisformat(char *str, unsigned int v, size_t *i)
{
	char				buf[32] = {0};
	std::string			s = "%";

	str++;
	*i = 0;
	while (*str && *str != '%' && *str != 'x' && *str != 'd')
	{
		s += *str;
		str++;
		(*i)++;
	}
	sprintf(buf, s.c_str(), v);

	return (std::string(s));
}

static std::string		getdiscode(t_compiler_method *m, char *s, unsigned char *code)
{
	std::string			str = "";
	auto				it = m->_list.begin();

	size_t				n = 0;
	size_t				i = 0;

	while (*s)
	{
		if (*s == '%' && it != m->_list.end())
		{
			unsigned int	v = (*(unsigned int*)code);

			n = (*it);
			v &= n;
			while (!n)
			{
				v = v >> 1;
				n = n >> 1;
			}
			str += getdisformat(s, v, &i);
			s += i;
			it++;
		}
		else
		{
			str += *s;
			s++;
		}
	}
	return (str);
}

std::string				GameboyCpu::disassembly(unsigned char *code,
												std::vector<t_compiler_method*> list)
{
	size_t				size = 0x150;
	t_compiler_method*	m = NULL;
	std::string			s = "";
	bool				ok = false;

	while (size < 0x8000)
	{
		for (auto it = list.begin(); it != list.end(); it++)
		{
			m = (t_compiler_method*)(*it);
			if (!memcmp(code, m->_opcode, m->_len))
			{
				s += getdiscode(m, (char*)m->_pattern.c_str(), code) + "\n";
				code += m->_len;
				size += m->_len;
				ok = true;
				break ;
			}
		}

		if (!ok)
			break ;
	}
	return (s);
}

std::string				GameboyCpu::disassembly_line(unsigned char *code,
												std::vector<t_compiler_method*> list,
												size_t *len)
{
	t_compiler_method*	m = NULL;

	for (auto it = list.begin(); it != list.end(); it++)
	{
		m = (t_compiler_method*)(*it);
		if (!memcmp(code, m->_opcode, m->_len))
		{
			*len = m->_len;
			return (getdiscode(m, (char*)m->_pattern.c_str(), code) + "\n");
		}
	}
	*len = 1;
	return ("Error\n");
}
