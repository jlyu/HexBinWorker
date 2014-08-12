#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include "defineV.h"

using namespace std;



class IntelHex
{
private:
	
	struct HexRecord {
		string dataLength;
		string startAddress;
		string recordType;
		string data;
		vector<string> datas;
		string sumCheck;
		string origRecord;
	};

	struct HexBlock {
		BYTE datas[FLASH_VOLUME * 1024]; // alloc 64K, init with 00
		int validLength;

		HexBlock() {
			memset(datas, 0x00, FLASH_VOLUME * 1024);
			validLength = 0;
		}
	};

	
	bool openHexFile(CString& hexFileName);
	bool checkLine(const char *src);
	bool matchLine(const char *src);
	bool verifyLine(const HexRecord& hexRecord);

	void hexStringToDec();
	unsigned int hexToDec(const string& str);

	bool formatParse(const char *src, const int lineNo);
	bool hexFormatParse(const char *src, char *dst);

	void hexStringToByte(const char* src, const int srcLen, BYTE* dst);
	void byteToHexString(BYTE* source, char* dest, int sourceLen);
	void splitHexData(const string& inData, vector<BYTE>& outData);

	void byteToBin(BYTE *pByte, char* pBin);


	CString _fileName;
	FILE* _pHexFileHandler;


	string _inStr;
	string _outStr;

	list<HexBlock> _hexBlocks;


public:

	IntelHex(void) { _pHexFileHandler = NULL; }
	IntelHex(const CString& hexFileName) {
		
		_fileName = hexFileName;
		_pHexFileHandler = NULL;

		_inStr = "";
		_outStr = "";
	}

	~IntelHex(void) { 
		if (_pHexFileHandler != NULL) {
			fclose(_pHexFileHandler);
			_pHexFileHandler = NULL;
		}
	}

	bool read();

	bool parse(string& inStr);
	bool parse();

	// text
	string getHex();
	string getBin();


	// write
	void writeToBinFile(FILE* fileHandler);


	string getFilePath();
	FILE* getFileWriteHandler();
	string _hexEditField;
	

};

