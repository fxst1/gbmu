class	Gameboy;
class	GameboyServer;

extern "C" unsigned short	opcodes[MAX_OPCODES];
extern "C" intptr_t			instructions[MAX_OPCODES];
extern "C" unsigned char	*gbram;
extern "C" Gameboy			*gbuser;
extern "C" Gameboy			*gbuser2;
extern "C" GameboyServer	server;
extern "C" GameboyClient	client;
