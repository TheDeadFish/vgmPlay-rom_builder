// entrypoints for the vgmPlayer

struct ImageHeadr {
	WORD pal[16];
	WORD scrollX, scrollY;
	WORD width, height, nTile;
};

inline void mdcode_setInt(byte* romData, int value, int offset) {
	*(uint*)(romData+offset) = bswap32(value); }
inline void mdcode_setRepLoop(byte* romData, int value) {
	mdcode_setInt(romData, value, 8); }
inline void mdcode_setPause(byte* romData, int value) {
	mdcode_setInt(romData, value, 12); }
inline void mdcode_setUiFlags(byte* romData, int value) {
	romData[16] = value; }
