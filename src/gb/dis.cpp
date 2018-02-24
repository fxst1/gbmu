#include <gameboy.hpp>

static t_compiler_method*			create_cm(std::string format, std::initializer_list<unsigned int> list,
												unsigned int opcode, size_t len)
{
	t_compiler_method*				m = (t_compiler_method*)malloc( sizeof(t_compiler_method) );

	m->_pattern = (char*)malloc(format.length() + 1);
	strcat(m->_pattern, format.c_str());
	memcpy(m->_opcode, &opcode, len);
	m->_list = list;
	m->_len = len;
	return (m);
}

std::vector<t_compiler_method*>		Gameboy::dissasembler(void)
{
	std::vector<t_compiler_method*>	v = std::vector<t_compiler_method*>();

	v.push_back(create_cm("NOP", {0x0}, 0x00, 1));
	v.push_back(create_cm("LD\tBC, $%hx", {0xFFFF00}, 0x01, 3));
	v.push_back(create_cm("LD\t(BC), A", {0x0}, 0x02, 1));
	v.push_back(create_cm("INC\tBC", {0x0}, 0x03, 1));
	v.push_back(create_cm("INC\tB", {0x0}, 0x04, 1));
	v.push_back(create_cm("DEC\tB", {0x0}, 0x05, 1));
	v.push_back(create_cm("LD\tB, $%hhx", {0xFF00}, 0x06, 2));
	v.push_back(create_cm("RLCA", {0xFF00}, 0x07, 1));
	v.push_back(create_cm("LD\t($%hx), SP", {0xFFFF00}, 0x08, 3));
	v.push_back(create_cm("ADD\tHL, BC", {0x0}, 0x09, 1));
	v.push_back(create_cm("LD\tA, (BC)", {0x0}, 0x0A, 1));
	v.push_back(create_cm("DEC\tBC", {0x0}, 0x0B, 1));
	v.push_back(create_cm("INC\tC", {0x0}, 0x0C, 1));
	v.push_back(create_cm("DEC\tC", {0x0}, 0x0D, 1));
	v.push_back(create_cm("LD\tC, $%hhx", {0xFF00}, 0x0E, 2));
	v.push_back(create_cm("RRCA", {0x0}, 0x0F, 1));

	v.push_back(create_cm("STOP", {0x0}, 0x00, 2));
	v.push_back(create_cm("LD\tDE, $%hx", {0xFFFF00}, 0x11, 3));
	v.push_back(create_cm("LD\t(DE), A", {0x0}, 0x12, 1));
	v.push_back(create_cm("INC\tDE", {0x0}, 0x13, 1));
	v.push_back(create_cm("INC\tD", {0x0}, 0x14, 1));
	v.push_back(create_cm("DEC\tD", {0x0}, 0x15, 1));
	v.push_back(create_cm("LD\tD, $%hhx", {0xFF00}, 0x16, 2));
	v.push_back(create_cm("RLA", {0xFF00}, 0x17, 1));
	v.push_back(create_cm("JR\t($%hx)", {0xFF00}, 0x18, 2));
	v.push_back(create_cm("ADD\tHL, DE", {0x0}, 0x19, 1));
	v.push_back(create_cm("LD\tA, (DE)", {0x0}, 0x1A, 1));
	v.push_back(create_cm("DEC\tDE", {0x0}, 0x1B, 1));
	v.push_back(create_cm("INC\tE", {0x0}, 0x1C, 1));
	v.push_back(create_cm("DEC\tE", {0x0}, 0x1D, 1));
	v.push_back(create_cm("LD\tE, $%hhx", {0xFF00}, 0x1E, 2));
	v.push_back(create_cm("RRA", {0x0}, 0x1F, 1));

	v.push_back(create_cm("JR\tNZ, $%hhx", {0xFF00}, 0x20, 2));
	v.push_back(create_cm("LD\tHL, $%hx", {0xFFFF00}, 0x21, 3));
	v.push_back(create_cm("LD\t(HL+), A", {0x0}, 0x22, 1));
	v.push_back(create_cm("INC\tHL", {0x0}, 0x23, 1));
	v.push_back(create_cm("INC\tH", {0x0}, 0x24, 1));
	v.push_back(create_cm("DEC\tH", {0x0}, 0x25, 1));
	v.push_back(create_cm("LD\tH, $%hhx", {0xFF00}, 0x26, 2));
	v.push_back(create_cm("DAA", {0xFF00}, 0x27, 1));
	v.push_back(create_cm("JR\tZ, ($%hx)", {0xFF00}, 0x28, 2));
	v.push_back(create_cm("ADD\tHL, HL", {0x0}, 0x29, 1));
	v.push_back(create_cm("LD\tA, (HL+)", {0x0}, 0x2A, 1));
	v.push_back(create_cm("DEC\tHL", {0x0}, 0x2B, 1));
	v.push_back(create_cm("INC\tL", {0x0}, 0x2C, 1));
	v.push_back(create_cm("DEC\tL", {0x0}, 0x2D, 1));
	v.push_back(create_cm("LD\tL, $%hhx", {0xFF00}, 0x2E, 2));
	v.push_back(create_cm("CPL", {0x0}, 0x2F, 1));

	v.push_back(create_cm("JR\tNC, $%hhx", {0xFF00}, 0x30, 2));
	v.push_back(create_cm("LD\tSP, $%hx", {0xFFFF00}, 0x31, 3));
	v.push_back(create_cm("LD\t(HL-), A", {0x0}, 0x32, 1));
	v.push_back(create_cm("INC\tSP", {0x0}, 0x33, 1));
	v.push_back(create_cm("INC\t(HL)", {0x0}, 0x34, 1));
	v.push_back(create_cm("DEC\t(HL)", {0x0}, 0x35, 1));
	v.push_back(create_cm("LD\t(HL), $%hhx", {0xFF00}, 0x36, 2));
	v.push_back(create_cm("SCF", {0x0}, 0x37, 1));
	v.push_back(create_cm("JR\tC, ($%hx)", {0xFF00}, 0x38, 2));
	v.push_back(create_cm("ADD\tHL, SP", {0x0}, 0x39, 1));
	v.push_back(create_cm("LD\tA, (HL-)", {0x0}, 0x3A, 1));
	v.push_back(create_cm("DEC\tSP", {0x0}, 0x3B, 1));
	v.push_back(create_cm("INC\tA", {0x0}, 0x3C, 1));
	v.push_back(create_cm("DEC\tA", {0x0}, 0x3D, 1));
	v.push_back(create_cm("LD\tA, $%hhx", {0xFF00}, 0x3E, 2));
	v.push_back(create_cm("CCF", {0x0}, 0x3F, 1));

	v.push_back(create_cm("LB\tB, B", {0x0}, 0x40, 1));
	v.push_back(create_cm("LB\tB, C", {0x0}, 0x41, 1));
	v.push_back(create_cm("LB\tB, D", {0x0}, 0x42, 1));
	v.push_back(create_cm("LB\tB, E", {0x0}, 0x43, 1));
	v.push_back(create_cm("LB\tB, H", {0x0}, 0x44, 1));
	v.push_back(create_cm("LB\tB, L", {0x0}, 0x45, 1));
	v.push_back(create_cm("LB\tB, (HL)", {0x0}, 0x46, 1));
	v.push_back(create_cm("LB\tB, A", {0x0}, 0x47, 1));
	v.push_back(create_cm("LB\tC, B", {0x0}, 0x48, 1));
	v.push_back(create_cm("LB\tC, C", {0x0}, 0x49, 1));
	v.push_back(create_cm("LB\tC, D", {0x0}, 0x4A, 1));
	v.push_back(create_cm("LB\tC, E", {0x0}, 0x4B, 1));
	v.push_back(create_cm("LB\tC, H", {0x0}, 0x4C, 1));
	v.push_back(create_cm("LB\tC, L", {0x0}, 0x4D, 1));
	v.push_back(create_cm("LB\tC, (HL)", {0x0}, 0x4E, 1));
	v.push_back(create_cm("LB\tC, A", {0x0}, 0x4F, 1));

	v.push_back(create_cm("LB\tD, B", {0x0}, 0x50, 1));
	v.push_back(create_cm("LB\tD, C", {0x0}, 0x51, 1));
	v.push_back(create_cm("LB\tD, D", {0x0}, 0x52, 1));
	v.push_back(create_cm("LB\tD, E", {0x0}, 0x53, 1));
	v.push_back(create_cm("LB\tD, H", {0x0}, 0x54, 1));
	v.push_back(create_cm("LB\tD, L", {0x0}, 0x55, 1));
	v.push_back(create_cm("LB\tD, (HL)", {0x0}, 0x56, 1));
	v.push_back(create_cm("LB\tD, A", {0x0}, 0x57, 1));
	v.push_back(create_cm("LB\tE, B", {0x0}, 0x58, 1));
	v.push_back(create_cm("LB\tE, C", {0x0}, 0x59, 1));
	v.push_back(create_cm("LB\tE, D", {0x0}, 0x5A, 1));
	v.push_back(create_cm("LB\tE, E", {0x0}, 0x5B, 1));
	v.push_back(create_cm("LB\tE, H", {0x0}, 0x5C, 1));
	v.push_back(create_cm("LB\tE, L", {0x0}, 0x5D, 1));
	v.push_back(create_cm("LB\tE, (HL)", {0x0}, 0x5E, 1));
	v.push_back(create_cm("LB\tE, A", {0x0}, 0x5F, 1));

	v.push_back(create_cm("LB\tH, B", {0x0}, 0x60, 1));
	v.push_back(create_cm("LB\tH, C", {0x0}, 0x61, 1));
	v.push_back(create_cm("LB\tH, D", {0x0}, 0x62, 1));
	v.push_back(create_cm("LB\tH, E", {0x0}, 0x63, 1));
	v.push_back(create_cm("LB\tH, H", {0x0}, 0x64, 1));
	v.push_back(create_cm("LB\tH, L", {0x0}, 0x65, 1));
	v.push_back(create_cm("LB\tH, (HL)", {0x0}, 0x66, 1));
	v.push_back(create_cm("LB\tH, A", {0x0}, 0x67, 1));
	v.push_back(create_cm("LB\tL, B", {0x0}, 0x68, 1));
	v.push_back(create_cm("LB\tL, C", {0x0}, 0x69, 1));
	v.push_back(create_cm("LB\tL, D", {0x0}, 0x6A, 1));
	v.push_back(create_cm("LB\tL, E", {0x0}, 0x6B, 1));
	v.push_back(create_cm("LB\tL, H", {0x0}, 0x6C, 1));
	v.push_back(create_cm("LB\tL, L", {0x0}, 0x6D, 1));
	v.push_back(create_cm("LB\tL, (HL)", {0x0}, 0x6E, 1));
	v.push_back(create_cm("LB\tL, A", {0x0}, 0x6F, 1));

	v.push_back(create_cm("LB\t(HL), B", {0x0}, 0x70, 1));
	v.push_back(create_cm("LB\t(HL), C", {0x0}, 0x71, 1));
	v.push_back(create_cm("LB\t(HL), D", {0x0}, 0x72, 1));
	v.push_back(create_cm("LB\t(HL), E", {0x0}, 0x73, 1));
	v.push_back(create_cm("LB\t(HL), H", {0x0}, 0x74, 1));
	v.push_back(create_cm("LB\t(HL), L", {0x0}, 0x75, 1));
	v.push_back(create_cm("HALT", {0x0}, 0x76, 1));
	v.push_back(create_cm("LB\t(HL), A", {0x0}, 0x77, 1));
	v.push_back(create_cm("LB\tA, B", {0x0}, 0x78, 1));
	v.push_back(create_cm("LB\tA, C", {0x0}, 0x79, 1));
	v.push_back(create_cm("LB\tA, D", {0x0}, 0x7A, 1));
	v.push_back(create_cm("LB\tA, E", {0x0}, 0x7B, 1));
	v.push_back(create_cm("LB\tA, H", {0x0}, 0x7C, 1));
	v.push_back(create_cm("LB\tA, L", {0x0}, 0x7D, 1));
	v.push_back(create_cm("LB\tA, (HL)", {0x0}, 0x7E, 1));
	v.push_back(create_cm("LB\tA, A", {0x0}, 0x7F, 1));

	v.push_back(create_cm("ADD\tA, B", {0x0}, 0x80, 1));
	v.push_back(create_cm("ADD\tA, C", {0x0}, 0x81, 1));
	v.push_back(create_cm("ADD\tA, D", {0x0}, 0x82, 1));
	v.push_back(create_cm("ADD\tA, E", {0x0}, 0x83, 1));
	v.push_back(create_cm("ADD\tA, H", {0x0}, 0x84, 1));
	v.push_back(create_cm("ADD\tA, L", {0x0}, 0x85, 1));
	v.push_back(create_cm("ADD\tA, (HL)", {0x0}, 0x86, 1));
	v.push_back(create_cm("ADD\tA, A", {0x0}, 0x87, 1));
	v.push_back(create_cm("ADC\tA, B", {0x0}, 0x88, 1));
	v.push_back(create_cm("ADC\tA, C", {0x0}, 0x89, 1));
	v.push_back(create_cm("ADC\tA, D", {0x0}, 0x8A, 1));
	v.push_back(create_cm("ADC\tA, E", {0x0}, 0x8B, 1));
	v.push_back(create_cm("ADC\tA, H", {0x0}, 0x8C, 1));
	v.push_back(create_cm("ADC\tA, L", {0x0}, 0x8D, 1));
	v.push_back(create_cm("ADC\tA, (HL)", {0x0}, 0x8E, 1));
	v.push_back(create_cm("ADC\tA, A", {0x0}, 0x8F, 1));

	v.push_back(create_cm("SUB\tA, B", {0x0}, 0x90, 1));
	v.push_back(create_cm("SUB\tA, C", {0x0}, 0x91, 1));
	v.push_back(create_cm("SUB\tA, D", {0x0}, 0x92, 1));
	v.push_back(create_cm("SUB\tA, E", {0x0}, 0x93, 1));
	v.push_back(create_cm("SUB\tA, H", {0x0}, 0x94, 1));
	v.push_back(create_cm("SUB\tA, L", {0x0}, 0x95, 1));
	v.push_back(create_cm("SUB\tA, (HL)", {0x0}, 0x96, 1));
	v.push_back(create_cm("SUB\tA, A", {0x0}, 0x97, 1));
	v.push_back(create_cm("SBC\tA, B", {0x0}, 0x98, 1));
	v.push_back(create_cm("SBC\tA, C", {0x0}, 0x99, 1));
	v.push_back(create_cm("SBC\tA, D", {0x0}, 0x9A, 1));
	v.push_back(create_cm("SBC\tA, E", {0x0}, 0x9B, 1));
	v.push_back(create_cm("SBC\tA, H", {0x0}, 0x9C, 1));
	v.push_back(create_cm("SBC\tA, L", {0x0}, 0x9D, 1));
	v.push_back(create_cm("SBC\tA, (HL)", {0x0}, 0x9E, 1));
	v.push_back(create_cm("SBC\tA, A", {0x0}, 0x9F, 1));

	v.push_back(create_cm("AND\tA, B", {0x0}, 0xA0, 1));
	v.push_back(create_cm("AND\tA, C", {0x0}, 0xA1, 1));
	v.push_back(create_cm("AND\tA, D", {0x0}, 0xA2, 1));
	v.push_back(create_cm("AND\tA, E", {0x0}, 0xA3, 1));
	v.push_back(create_cm("AND\tA, H", {0x0}, 0xA4, 1));
	v.push_back(create_cm("AND\tA, L", {0x0}, 0xA5, 1));
	v.push_back(create_cm("AND\tA, (HL)", {0x0}, 0xA6, 1));
	v.push_back(create_cm("AND\tA, A", {0x0}, 0xA7, 1));
	v.push_back(create_cm("XOR\tA, B", {0x0}, 0xA8, 1));
	v.push_back(create_cm("XOR\tA, C", {0x0}, 0xA9, 1));
	v.push_back(create_cm("XOR\tA, D", {0x0}, 0xAA, 1));
	v.push_back(create_cm("XOR\tA, E", {0x0}, 0xAB, 1));
	v.push_back(create_cm("XOR\tA, H", {0x0}, 0xAC, 1));
	v.push_back(create_cm("XOR\tA, L", {0x0}, 0xAD, 1));
	v.push_back(create_cm("XOR\tA, (HL)", {0x0}, 0xAE, 1));
	v.push_back(create_cm("XOR\tA, A", {0x0}, 0xAF, 1));

	v.push_back(create_cm("OR\tA, B", {0x0}, 0xB0, 1));
	v.push_back(create_cm("OR\tA, C", {0x0}, 0xB1, 1));
	v.push_back(create_cm("OR\tA, D", {0x0}, 0xB2, 1));
	v.push_back(create_cm("OR\tA, E", {0x0}, 0xB3, 1));
	v.push_back(create_cm("OR\tA, H", {0x0}, 0xB4, 1));
	v.push_back(create_cm("OR\tA, L", {0x0}, 0xB5, 1));
	v.push_back(create_cm("OR\tA, (HL)", {0x0}, 0xB6, 1));
	v.push_back(create_cm("OR\tA, A", {0x0}, 0xB7, 1));
	v.push_back(create_cm("CP\tA, B", {0x0}, 0xB8, 1));
	v.push_back(create_cm("CP\tA, C", {0x0}, 0xB9, 1));
	v.push_back(create_cm("CP\tA, D", {0x0}, 0xBA, 1));
	v.push_back(create_cm("CP\tA, E", {0x0}, 0xBB, 1));
	v.push_back(create_cm("CP\tA, H", {0x0}, 0xBC, 1));
	v.push_back(create_cm("CP\tA, L", {0x0}, 0xBD, 1));
	v.push_back(create_cm("CP\tA, (HL)", {0x0}, 0xBE, 1));
	v.push_back(create_cm("CP\tA, A", {0x0}, 0xBF, 1));

	v.push_back(create_cm("RET\tNZ", {0x0}, 0xC0, 1));
	v.push_back(create_cm("POP\tBC", {0x0}, 0xC1, 1));
	v.push_back(create_cm("JP\tNZ, $%hx", {0xFFFF00}, 0xC2, 3));
	v.push_back(create_cm("JP\t$%hx", {0xFFFF00}, 0xC3, 3));
	v.push_back(create_cm("CALL\tNZ, $%hx", {0xFFFF00}, 0xC4, 3));
	v.push_back(create_cm("PUSH\tBC", {0x0}, 0xC5, 1));
	v.push_back(create_cm("AND\tA, $%hhx", {0xFF00}, 0xC6, 2));
	v.push_back(create_cm("RST\t$00", {0x0}, 0xC7, 1));
	v.push_back(create_cm("RET\tZ", {0x0}, 0xC8, 1));
	v.push_back(create_cm("RET", {0x0}, 0xC9, 1));
	v.push_back(create_cm("JP\tA, $%hhx", {0xFF00}, 0xCA, 3));
	v.push_back(create_cm("CB\t$%hhx", {0xFF00}, 0xCB, 2));
	v.push_back(create_cm("CALL\tZ, $%hx", {0xFFFF00}, 0xCC, 3));
	v.push_back(create_cm("CALL\t$%hx", {0xFFFF00}, 0xCD, 3));
	v.push_back(create_cm("ADC\tA, $%hhx", {0xFF00}, 0xCE, 2));
	v.push_back(create_cm("RST\t$08", {0x0}, 0xCF, 1));

	v.push_back(create_cm("RET\tNC", {0x0}, 0xD0, 1));
	v.push_back(create_cm("POP\tDE", {0x0}, 0xD1, 1));
	v.push_back(create_cm("JP\tNC, $%hx", {0xFFFF00}, 0xD2, 3));
	v.push_back(create_cm("CALL\tNC, $%hx", {0xFFFF00}, 0xD4, 3));
	v.push_back(create_cm("PUSH\tDE", {0x0}, 0xD5, 1));
	v.push_back(create_cm("SUB\tA, $%hhx", {0xFF00}, 0xD6, 2));
	v.push_back(create_cm("RST\t$10", {0x0}, 0xD7, 1));
	v.push_back(create_cm("RET\tC", {0x0}, 0xD8, 1));
	v.push_back(create_cm("RETI", {0x0}, 0xD9, 1));
	v.push_back(create_cm("JP\tC, $%hx", {0xFFFF00}, 0xDA, 3));
	v.push_back(create_cm("CALL\tC, $%hx", {0xFFFF00}, 0xDC, 3));
	v.push_back(create_cm("SBC\tA, $%hhx", {0xFF00}, 0xDE, 2));
	v.push_back(create_cm("RST\t$28", {0x0}, 0xDF, 1));

	v.push_back(create_cm("LDH\t($%hhx), A", {0xFF00}, 0xE0, 2));
	v.push_back(create_cm("POP\tHL", {0x0}, 0xE1, 1));
	v.push_back(create_cm("LD\t(C), A", {0x0}, 0xE2, 2));
	v.push_back(create_cm("PUSH\tHL", {0x0}, 0xE5, 1));
	v.push_back(create_cm("AND\tA, $%hhx", {0xFF00}, 0xE6, 2));
	v.push_back(create_cm("RST\t$20", {0x0}, 0xE7, 1));
	v.push_back(create_cm("ADD\tSP, ($%hhx)", {0xFF00}, 0xE8, 2));
	v.push_back(create_cm("JP (HL)", {0x0}, 0xE9, 1));
	v.push_back(create_cm("LD\tA, ($%hx)", {0xFFFF00}, 0xEA, 3));
	v.push_back(create_cm("XOR\tA, $%hhx", {0xFF00}, 0xEE, 2));
	v.push_back(create_cm("RST\t$28", {0x0}, 0xEF, 1));

	v.push_back(create_cm("LDH\tA, ($%hhx)", {0xFF00}, 0xF0, 2));
	v.push_back(create_cm("POP\tAF", {0x0}, 0xF1, 1));
	v.push_back(create_cm("LD\tA, (C)", {0x0}, 0xF2, 2));
	v.push_back(create_cm("DI", {0x0}, 0xF2, 1));
	v.push_back(create_cm("PUSH\tAF", {0x0}, 0xF5, 1));
	v.push_back(create_cm("OR\tA, $%hhx", {0xFF00}, 0xF6, 2));
	v.push_back(create_cm("RST\t$30", {0x0}, 0xF7, 1));
	v.push_back(create_cm("LD\tHL, SP + ($%hhx)", {0xFF00}, 0xF8, 2));
	v.push_back(create_cm("LD SP, HL", {0x0}, 0xF9, 1));
	v.push_back(create_cm("LD\t($%hhx), A", {0xFFFF00}, 0xFA, 3));
	v.push_back(create_cm("CP\tA, $%hhx", {0xFF00}, 0xFE, 2));
	v.push_back(create_cm("RST\t$38", {0x0}, 0xFF, 1));

	return (v);
}
