#pragma once

// HexEdit
const char hexTable[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#define TOHEX(a, b)	{*b++ = hexTable[a>>4]; *b++ = hexTable[a&0xf];}

// ComController
#define ACK  0x79
#define NACK 0x1F
#define GET_COMMAND  "\x00\xFF"
#define ERASE_MEMORY "\x43\xBC"
#define GLOBAL_ERASE "\xFF\x00"
#define WRITE_MEMORY "\x31\xCE"


const int FLASH_VOLUME = 64; // KB
const int RECORD_LENGTH = 16;