#ifndef	GAMEBOY_CPU_HPP
# define GAMEBOY_CPU_HPP
# include <iostream>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <string>
# include <string.h>
# include <thread>
# include <functional>
# include <mutex>
# include <condition_variable>
# include <chrono>
# include <vector>

typedef unsigned long call_t[4];

extern "C" void	instruction_halt(void);
extern "C" void	instruction_stop(void);
extern "C" void	instruction_bit(void);
extern "C" void	instruction_set(void);
extern "C" void	instruction_res(void);
extern "C" void	instruction_inc16(void);
extern "C" void	instruction_dec16(void);
extern "C" void	instruction_swap(void);

extern "C" void	instruction_add_8_1(void);
extern "C" void	instruction_add_8_2(void);
extern "C" void	instruction_adc_1(void);
extern "C" void	instruction_adc_2(void);
extern "C" void	instruction_sub_8_1(void);
extern "C" void	instruction_sub_8_2(void);
extern "C" void	instruction_sbc_1(void);
extern "C" void	instruction_sbc_2(void);
extern "C" void	instruction_and_1(void);
extern "C" void	instruction_and_2(void);
extern "C" void	instruction_or_1(void);
extern "C" void	instruction_or_2(void);
extern "C" void	instruction_xor_1(void);
extern "C" void	instruction_xor_2(void);
extern "C" void	instruction_cp_1(void);
extern "C" void	instruction_cp_2(void);
extern "C" void	instruction_cpl(void);
extern "C" void	instruction_inc8(void);
extern "C" void	instruction_dec8(void);
extern "C" void	instruction_ld_8_1(void);
extern "C" void	instruction_ld_8_1_5(void);
extern "C" void	instruction_ld_8_2(void);
extern "C" void	instruction_ld_8_3(void);
extern "C" void	instruction_ld_8_4(void);
extern "C" void	instruction_ld_8_5(void);
extern "C" void	instruction_ld_8_6(void);
extern "C" void	instruction_ld_8_7(void);
extern "C" void	instruction_ld_8_8(void);
extern "C" void	instruction_ld_8_9(void);
extern "C" void	instruction_ld_8_10(void);
extern "C" void	instruction_ld_8_11(void);
extern "C" void	instruction_ld_8_12(void);
extern "C" void	instruction_ld_8_13(void);
extern "C" void	instruction_ld_8_14(void);
extern "C" void	instruction_ld_8_15(void);
extern "C" void	instruction_ld_16_1(void);
extern "C" void	instruction_ld_16_2(void);
extern "C" void	instruction_ld_16_3(void);
extern "C" void	instruction_push(void);
extern "C" void	instruction_pop(void);
extern "C" void	instruction_rlca(void);
extern "C" void	instruction_rla(void);
extern "C" void	instruction_rrca(void);
extern "C" void	instruction_rra(void);
extern "C" void	instruction_rlc(void);
extern "C" void	instruction_rl(void);
extern "C" void	instruction_rrc(void);
extern "C" void	instruction_rr(void);
extern "C" void	instruction_sla(void);
extern "C" void	instruction_sra(void);
extern "C" void	instruction_srl(void);
extern "C" void	instruction_jp_1(void);
extern "C" void	instruction_jp_2(void);
extern "C" void	instruction_jp_3(void);
extern "C" void	instruction_jr_1(void);
extern "C" void	instruction_jr_2(void);
extern "C" void	instruction_ret_1(void);
extern "C" void	instruction_ret_2(void);
extern "C" void	instruction_call_1(void);
extern "C" void	instruction_call_2(void);
extern "C" void	instruction_reti(void);
extern "C" void	instruction_rst(void);

extern "C" void	instruction_add_16_1(void);
extern "C" void	instruction_add_16_2(void);
extern "C" void	instruction_daa(void);
extern "C" void	instruction_ldhl(void);
extern "C" void	instruction_nop(void);

extern "C" void	instruction_di(void);
extern "C" void	instruction_ei(void);
extern "C" void	instruction_ccf(void);
extern "C" void	instruction_scf(void);


extern "C" void	instruction_pal01(void);
extern "C" void	instruction_pal23(void);
extern "C" void	instruction_pal03(void);
extern "C" void	instruction_pal12(void);
extern "C" void	instruction_attr_chr(void);
extern "C" void	instruction_sound(void);
extern "C" void	instruction_snd_trn(void);
extern "C" void	instruction_pal_set(void);
extern "C" void	instruction_pal_trn(void);
extern "C" void	instruction_atrc_en(void);
extern "C" void	instruction_icon_en(void);
extern "C" void	instruction_data_trn(void);
extern "C" void	instruction_mlt_req(void);
extern "C" void	instruction_jump(void);
extern "C" void	instruction_chr_trn(void);
extern "C" void	instruction_pct_trn(void);
extern "C" void	instruction_attr_trn(void);
extern "C" void	instruction_attr_set(void);
extern "C" void	instruction_mask_en(void);
extern "C" void	instruction_pal_pri(void);

extern "C" void	instruction_data_snd(void);
extern "C" void	instruction_attr_lin(void);
extern "C" void	instruction_attr_div(void);
extern "C" void	instruction_attr_blk(void);


typedef	struct		s_expr
{
	unsigned int	fct;
	unsigned int	issigned;

	unsigned int	type;
	unsigned int	high_bit;
	unsigned int	nbit;
	unsigned int	offset;
	std::string		*strings;

	int				index;
}					t_expr;

typedef struct				s_transfer
{
	std::mutex				mtx;
	std::condition_variable	cv;
	unsigned int			flag;
	unsigned char			*addr;
	bool					isset;
	bool					ready;
	void					*obj;
}							t_transfer;

extern "C" unsigned long codecpu(unsigned int nbytes, unsigned int opcode);
extern "C" int	requestcpu(unsigned char *rssp, t_transfer *rqst);
extern "C" void	asmcpu(unsigned short start, unsigned char *code, t_transfer *request);
std::vector<unsigned char*>			dump_by_line(unsigned char *buffer);

class	Cpu
{
	public :

		// Stop CPU
		static const unsigned int	stop = 0;
		// Pause
		static const unsigned int	wait = 1;
		// Get Values
		static const unsigned int	rssp = 2;
		// Get PC register
		static const unsigned int	where = 4;
		// Continue (Default)
		static const unsigned int	run = 8;
		// Next (next instruction)
		static const unsigned int	next = 16;

		static std::string			*conditions_names() {
			static std::string		tab[] = { "NZ", "Z", "NC", "C" };
			return ((std::string*) tab);
		}
		static std::string			*registers_i_names() {
			static std::string		tab[] = { "BC", "DE", "HL", "SP" };
			return ((std::string*) tab);
		}
		static std::string			*registers_ii_names() {
			static std::string		tab[] = { "BC", "DE", "HL", "AF" };
			return ((std::string*) tab);
		}
		static std::string			*registers_iii_names() {
			static std::string		tab[] = { "B", "C", "D", "E", "H", "L", "(HL)", "A" };
			return ((std::string*) tab);
		}

		bool			pause;
		unsigned short	start;
		unsigned char	*code;

		Cpu();
		Cpu(unsigned char *code, unsigned short start);
		~Cpu();

		static bool				codecpu_parser(call_t call, unsigned short opcode)
		{
			return (call[2] == (opcode & call[3]));
		}

		static call_t		*get_calls()
		{
			static call_t	calls[] = {

				/* 2 bytes const opcode */

				{(unsigned long)&instruction_stop, 2, 0x0100, 0xFFFF},

				// 2 bytes opcode with 1 variable

				{(unsigned long)&instruction_rlc, 2, 0xCB00, 0xFFF8},
				{(unsigned long)&instruction_rl, 2, 0xCB10, 0xFFF8},
				{(unsigned long)&instruction_rrc, 2, 0xCB08, 0xFFF8},
				{(unsigned long)&instruction_rr, 2, 0xCB18, 0xFFF8},
				{(unsigned long)&instruction_sla, 2, 0xCB20, 0xFFF8},
				{(unsigned long)&instruction_sra, 2, 0xCB28, 0xFFF8},
				{(unsigned long)&instruction_srl, 2, 0xCB38, 0xFFF8},
				{(unsigned long)&instruction_swap, 2, 0xCB30, 0xFFF8},

				// 2 bytes opcode with multiple variable

				{(unsigned long)&instruction_bit, 2, 0xCB40, 0xFFC0},
				{(unsigned long)&instruction_set, 2, 0xCBC0, 0xFFC0},
				{(unsigned long)&instruction_res, 2, 0xCB80, 0xFFC0},

				/* 1 byte const opcode */

/*				{(unsigned long)&instruction_pal01, 1, 0x00, 0xFF},
				{(unsigned long)&instruction_pal23, 1, 0x01, 0xFF},
				{(unsigned long)&instruction_pal03, 1, 0x02, 0xFF},
				{(unsigned long)&instruction_pal12, 1, 0x03, 0xFF},
				{(unsigned long)&instruction_attr_chr, 1, 0x07, 0xFF},
				{(unsigned long)&instruction_sound, 1, 0x08, 0xFF},
				{(unsigned long)&instruction_snd_trn, 1, 0x09, 0xFF},
				{(unsigned long)&instruction_pal_set, 1, 0x0A, 0xFF},
				{(unsigned long)&instruction_pal_trn, 1, 0x0B, 0xFF},
				{(unsigned long)&instruction_atrc_en, 1, 0x0C, 0xFF},
				{(unsigned long)&instruction_icon_en, 1, 0x0E, 0xFF},
				{(unsigned long)&instruction_data_snd, 1, 0x0F, 0xFF},
				{(unsigned long)&instruction_data_trn, 1, 0x10, 0xFF},
				{(unsigned long)&instruction_mlt_req, 1, 0x11, 0xFF},
				{(unsigned long)&instruction_jump, 1, 0x12, 0xFF},
				{(unsigned long)&instruction_chr_trn, 1, 0x13, 0xFF},
				{(unsigned long)&instruction_pct_trn, 1, 0x14, 0xFF},
				{(unsigned long)&instruction_attr_trn, 1, 0x15, 0xFF},
				{(unsigned long)&instruction_attr_set, 1, 0x16, 0xFF},
				{(unsigned long)&instruction_mask_en, 1, 0x17, 0xFF},
				{(unsigned long)&instruction_pal_pri, 1, 0x18, 0xFF},



				{(unsigned long)&instruction_pal01, 1, 0x01, 0xFF},
				{(unsigned long)&instruction_pal23, 1, 0x09, 0xFF},
				{(unsigned long)&instruction_pal03, 1, 0x17, 0xFF},
				{(unsigned long)&instruction_pal12, 1, 0x19, 0xFF},
				{(unsigned long)&instruction_attr_chr, 1, 0x39, 0xFF},
				{(unsigned long)&instruction_sound, 1, 0x41, 0xFF},
				{(unsigned long)&instruction_snd_trn, 1, 0x49, 0xFF},
				{(unsigned long)&instruction_pal_set, 1, 0x51, 0xFF},
				{(unsigned long)&instruction_pal_trn, 1, 0x59, 0xFF},
				{(unsigned long)&instruction_atrc_en, 1, 0x61, 0xFF},
				{(unsigned long)&instruction_icon_en, 1, 0x71, 0xFF},
				{(unsigned long)&instruction_data_snd, 1, 0x79, 0xFF},
				{(unsigned long)&instruction_data_trn, 1, 0x81, 0xFF},
				{(unsigned long)&instruction_mlt_req, 1, 0x89, 0xFF},
				{(unsigned long)&instruction_jump, 1, 0x91, 0xFF},
				{(unsigned long)&instruction_chr_trn, 1, 0x99, 0xFF},
				{(unsigned long)&instruction_pct_trn, 1, 0xA1, 0xFF},
				{(unsigned long)&instruction_attr_trn, 1, 0xA9, 0xFF},
				{(unsigned long)&instruction_attr_set, 1, 0xB1, 0xFF},
				{(unsigned long)&instruction_mask_en, 1, 0xB9, 0xFF},
				{(unsigned long)&instruction_pal_pri, 1, 0xC9, 0xFF},
*/
				{(unsigned long)&instruction_nop, 1, 0x00, 0xFF},
				{(unsigned long)&instruction_halt, 1, 0x76, 0xFF},
				{(unsigned long)&instruction_ccf, 1, 0x3F, 0xFF},
				{(unsigned long)&instruction_scf, 1, 0x37, 0xFF},
				{(unsigned long)&instruction_di, 1, 0xF3, 0xFF},
				{(unsigned long)&instruction_ei, 1, 0xF7, 0xFF},
				{(unsigned long)&instruction_call_1, 1, 0xCD, 0xFF},
				{(unsigned long)&instruction_ret_1, 1, 0xC9, 0xFF},
				{(unsigned long)&instruction_reti, 1, 0xD9, 0xFF},

				{(unsigned long)&instruction_rlca, 1, 0x07, 0xFF},
				{(unsigned long)&instruction_rla, 1, 0x17, 0xFF},
				{(unsigned long)&instruction_rrca, 1, 0x0F, 0xFF},
				{(unsigned long)&instruction_rra, 1, 0x1F, 0xFF},

				{(unsigned long)&instruction_jp_1, 1, 0xC3, 0xFF},
				{(unsigned long)&instruction_jp_3, 1, 0xE9, 0xFF},
				{(unsigned long)&instruction_jr_1, 1, 0x18, 0xFF},

				{(unsigned long)&instruction_daa, 1, 0x27, 0xFF},
				{(unsigned long)&instruction_add_8_2, 1, 0xC6, 0xFF},
				{(unsigned long)&instruction_add_16_2, 1, 0xE8, 0xFF},
				{(unsigned long)&instruction_adc_2, 1, 0xCE, 0xFF},
				{(unsigned long)&instruction_sub_8_2, 1, 0xD6, 0xFF},
				{(unsigned long)&instruction_sbc_2, 1, 0xDE, 0xFF},
				{(unsigned long)&instruction_and_2, 1, 0xE6, 0xFF},
				{(unsigned long)&instruction_or_2, 1, 0xF6, 0xFF},
				{(unsigned long)&instruction_xor_2, 1, 0xEE, 0xFF},
				{(unsigned long)&instruction_cp_2, 1, 0xFE, 0xFF},
				{(unsigned long)&instruction_cpl, 1, 0x2F, 0xFF},

				{(unsigned long)&instruction_ld_8_2, 1, 0x0A, 0xFF},
				{(unsigned long)&instruction_ld_8_3, 1, 0x1A, 0xFF},
				{(unsigned long)&instruction_ld_8_4, 1, 0xF2, 0xFF},
				{(unsigned long)&instruction_ld_8_5, 1, 0xE2, 0xFF},
				{(unsigned long)&instruction_ld_8_6, 1, 0xF0, 0xFF},
				{(unsigned long)&instruction_ld_8_7, 1, 0xE0, 0xFF},
				{(unsigned long)&instruction_ld_8_8, 1, 0xFA, 0xFF},
				{(unsigned long)&instruction_ld_8_9, 1, 0xEA, 0xFF},
				{(unsigned long)&instruction_ld_8_10, 1, 0x2A, 0xFF},
				{(unsigned long)&instruction_ld_8_11, 1, 0x3A, 0xFF},
				{(unsigned long)&instruction_ld_8_12, 1, 0x02, 0xFF},
				{(unsigned long)&instruction_ld_8_13, 1, 0x12, 0xFF},
				{(unsigned long)&instruction_ld_8_14, 1, 0x22, 0xFF},
				{(unsigned long)&instruction_ld_8_15, 1, 0x32, 0xFF},
				{(unsigned long)&instruction_ld_16_2, 1, 0xF9, 0xFF},
				{(unsigned long)&instruction_ld_16_3, 1, 0x08, 0xFF},
				{(unsigned long)&instruction_ldhl, 1, 0xF8, 0xFF},
/*
				{(unsigned long)&instruction_attr_blk, 1, 0x04, 0xFF},
				{(unsigned long)&instruction_attr_lin, 1, 0x05, 0xFF},
				{(unsigned long)&instruction_attr_div, 1, 0x06, 0xFF},

				// 1 byte variable opcode

				{(unsigned long)&instruction_attr_blk, 1, 0x20, 0xF8},
				{(unsigned long)&instruction_attr_lin, 1, 0x28, 0xF8},
				{(unsigned long)&instruction_attr_div, 1, 0x30, 0xF8},
*/
				{(unsigned long)&instruction_call_2, 1, 0xC4, 0xE7},
				{(unsigned long)&instruction_ret_2, 1, 0xC0, 0xE7},
				{(unsigned long)&instruction_rst, 1, 0xC7, 0xC7},

				{(unsigned long)&instruction_ld_8_1_5, 1, 0x06, 0xC7},
				{(unsigned long)&instruction_ld_16_1, 1, 0x01, 0xCF},

				{(unsigned long)&instruction_push, 1, 0xC5, 0xCF},
				{(unsigned long)&instruction_pop, 1, 0xC1, 0xCF},

				{(unsigned long)&instruction_add_8_1, 1, 0x80, 0xF8},
				{(unsigned long)&instruction_adc_1, 1, 0x88, 0xF8},
				{(unsigned long)&instruction_sub_8_1, 1, 0x90, 0xF8},
				{(unsigned long)&instruction_sbc_1, 1, 0x98, 0xF8},
				{(unsigned long)&instruction_and_1, 1, 0xA0, 0xF8},
				{(unsigned long)&instruction_or_1, 1, 0xB0, 0xF8},
				{(unsigned long)&instruction_xor_1, 1, 0xA8, 0xF8},
				{(unsigned long)&instruction_cp_1, 1, 0xB8, 0xF8},
				{(unsigned long)&instruction_inc8, 1, 0x04, 0xC7},
				{(unsigned long)&instruction_dec8, 1, 0x05, 0xC7},
				{(unsigned long)&instruction_add_16_1, 1, 0x09, 0xCF},
				{(unsigned long)&instruction_inc16, 1, 0x03, 0xCB},
				{(unsigned long)&instruction_dec16, 1, 0x0B, 0xCB},

				{(unsigned long)&instruction_jp_2, 1, 0xC2, 0xE7},
				{(unsigned long)&instruction_jr_2, 1, 0x20, 0xE7},

				// 1 byte opcode with multiple variable

				{(unsigned long)&instruction_ld_8_1, 1, 0x40, 0xC0},

				{0, 0, 0x0, 0x0}
			};

			return (calls);
		}

		static char				**get_calls_names()
		{
			static const char	*calls[] = {

				/* 2 bytes const opcode */

				"Stop",

				// 2 bytes opcode with 1 variable

				"Rlc r",
				"Rl r",
				"Rrc r",
				"Rr r",
				"Sla r",
				"Sra r",
				"Srl r",
				"Swap r",

				// 2 bytes opcode with multiple variable

				"Bit n r",
				"Set n r",
				"Res n r",

				/* 1 byte const opcode */

/*				"PAL01",
				"PAL23",
				"PAL03",
				"PAL12",
				"ATTR_DIV",
				"SOUND",
				"SOU_TRN",
				"PAL_SET",
				"PAL_TRN",
				"ATRC_EN",
				"ICON_EN",
				"DATA_SND",
				"DATA_TRN",
				"MLT_REQ",
				"JUMP",
				"CHR_TRN",
				"PCT_TRN",
				"ATTR_TRN",
				"ATTR_SET",
				"MASK_EN",
				"PAL_PRI",
*/
				"Nop",
				"Halt",
				"Ccf",
				"Scf",
				"Di",
				"Ei",
				"Call nn",
				"Ret",
				"Reti",

				"Rlca",
				"Rla",
				"Rrca",
				"Rra",

				"Jp nn",
				"Jp (HL)",
				"Jr n",

				"Daa",
				"Add n",
				"Add SP e",
				"Adc n",
				"Sub n",
				"Sbc n",
				"And n",
				"Or n",
				"Xor n",
				"Cp n",
				"Cpl",

				"Ld A (BC)",
				"Ld A (DE)",
				"Ldr A (C)",
				"Ldr (C), A",
				"Ldr A, (n)",
				"Ldr (n), A",
				"Ldr A, (nn)",
				"Ldr (nn), A",
				"Ld A (HLI)",
				"Ld A (HLD)",
				"Ld (BC) A",
				"Ld (DE) A",
				"Ld (HLI) A",
				"Ld (HLD) A",
				"Ld SP HL",
				"Ld (nn) SP",
				"Ldhl e",

				// 1 byte opcode with 1 variable


//				"ATTR_BLK",	//+2
//				"ATTR_LIN",
//				"ATTRITUTE_CHR",

				"Call cc, nn",
				"Ret cc",
				"Rst t",

				"Ld r n",
				"Ld R1 nn",

				"Push R1",
				"Pop R1",

				"Add A r",
				"Adc r",
				"Sub r",
				"Sbc r",
				"And r",
				"Or r",
				"Xor r",
				"Cp r",
				"Inc r",
				"Dec r",
				"Add HL R1",
				"Inc R1",
				"Dec R1",

				"Jp cc nn",
				"Jr cc n",

				// 1 byte opcode with multiple variable

				"Ld r r'"

			};

			return ((char**)calls);
		}

		static char				**get_calls_expr()
		{
			static const char	*calls[] = {

				/* 2 bytes const opcode */

				"Stop",

				// 2 bytes opcode with 1 variable

				"Rlc %r.10",
				"Rl %r.10",
				"Rrc %r.10",
				"Rr %r.10",
				"Sla %r.10",
				"Sra %r.10",
				"Srl %r.10",
				"Swap %r.10",

				// 2 bytes opcode with multiple variable

				"Bit 0x%v.13 %r.10",
				"Set 0x%v.13 %r.10",
				"Res 0x%v.13 %r.10",

				/* 1 byte const opcode */

/*				"PAL01\t0x%V.15 0x%V.23 0x%V.31 0x%V.39\n     \t0x%V.47 0x%V.55 0x%V.63 0x%V.71\n     \t0x%V.79 0x%V.87 0x%V.95\n     \t0x%V.103 0x%V.111 0x%V.119\n     \t0x%V.127",
				"PAL23\t0x%V.15 0x%V.23 0x%V.31 0x%V.39\n     \t0x%V.47 0x%V.55 0x%V.63 0x%V.71\n     \t0x%V.79 0x%V.87 0x%V.95\n     \t0x%V.103 0x%V.111 0x%V.119\n     \t0x%V.127",
				"PAL03\t0x%V.15 0x%V.23 0x%V.31 0x%V.39\n     \t0x%V.47 0x%V.55 0x%V.63 0x%V.71\n     \t0x%V.79 0x%V.87 0x%V.95\n     \t0x%V.103 0x%V.111 0x%V.119\n     \t0x%V.127",
				"PAL12\t0x%V.15 0x%V.23 0x%V.31 0x%V.39\n     \t0x%V.47 0x%V.55 0x%V.63 0x%V.71\n     \t0x%V.79 0x%V.87 0x%V.95\n     \t0x%V.103 0x%V.111 0x%V.119\n     \t0x%V.127",
				"ATTR_DIV 0x%h7.15 0x%h5.20",
				"SOUND 0x%V.15 0x%V.23 0x%V.31 0x%V.39",
				"SOU_TRN",
				"PAL_SET 0x%V.15 0x%h1.16 0x%V.31 0x%V.47 0x%V.63 0x%V.79",
				"PAL_TRN",
				"ATRC_EN 0x%h1.8",
				"ICON_EN 0x%h3.10",
				"DATA_SND %f4",
				"DATA_TRN 0x%V.23%V.15 0x%V.31",
				"MLT_REQ 0x%h2.9",
				"JUMP 0x%V.23%V.15 0x%V.31 0x%V.47%V.39 0x%V.55",
				"CHR_TRN 0x%h1.8",
				"PCT_TRN",
				"ATTR_TRN",
				"ATTR_SET 0x%h7.14",
				"MASK_EN 0x%h2.9",
				"PAL_PRI",
*/
				"Nop",
				"Halt",
				"Ccf",
				"Scf",
				"Di",
				"Ei",
				"Call 0x%V.23%V.15",
				"Ret",
				"Reti",

				"Rlca",
				"Rla",
				"Rrca",
				"Rra",

				"Jp 0x%V.23%V.15",
				"Jp (HL)",
				"Jr 0x%s3.15",

				"Daa",
				"Add 0x%V.15",
				"Add SP 0x%V.15",
				"Adc 0x%V.15",
				"Sub 0x%V.15",
				"Sbc 0x%V.15",
				"And 0x%V.15",
				"Or 0x%V.15",
				"Xor 0x%V.15",
				"Cp 0x%V.15",
				"Cpl",

				"Ld A (BC)",
				"Ld A (DE)",
				"Ldr A (C)",
				"Ldr (C), A",
				"Ldr A, (0x%V.15)",
				"Ldr (0x%V.15), A",
				"Ldr A, (0x%V.23%V.15)",
				"Ldr (0x%V.23%V.15) A",
				"Ld A (HLI)",
				"Ld A (HLD)",
				"Ld (BC) A",
				"Ld (DE) A",
				"Ld (HLI) A",
				"Ld (HLD) A",
				"Ld SP HL",
				"Ld (0x%V.23%V.15) SP",
				"Ldhl 0x%V.15",

				// 1 byte opcode with 1 variable

/*				"ATTR_BLK %f1",
				"ATTR_LIN %f2",
				"ATTRITUTE_CHR %f3",
*/
				"Call %c.4, 0x%V.23%V.15",
				"Ret %c.4",
				"Rst %t.5",

				"Ld %r.5 0x%V.15",
				"Ld %R1.5 0x%V.23%V.15",

				"Push %R2.5",
				"Pop %R2.5",

				"Add %r.2",
				"Adc %r.2",
				"Sub %r.2",
				"Sbc %r.2",
				"And %r.2",
				"Or %r.2",
				"Xor %r.2",
				"Cp %r.2",
				"Inc %r.2",
				"Dec %r.2",
				"Add HL %R1.5",
				"Inc %R1.5",
				"Dec %R1.5",

				"Jp %c.4 0x%V.23%V.15",
				"Jr %c.4 0x%s3.15",

				// 1 byte opcode with multiple variable

				"Ld %r.5 %r.2"
			};

			return ((char**)calls);
		}


};


#endif