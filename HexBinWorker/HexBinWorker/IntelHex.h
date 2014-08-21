#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include "GlobalDefine.h"
using namespace std;


class IntelHex {
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
	bool matchLine(const char *src, HexRecord& hexRecord);
	bool verifyLine(const HexRecord& hexRecord);
	bool appendDatas(const HexRecord& hexRecord);
	unsigned int hexToDec(const string& str);
	void splitHexData(const string& inData, vector<BYTE>& outData);
	void hexBlocksToOutDatas();


	CString _fileName;
	FILE* _pHexFileHandler;

	// -in
	string _inStr;

	// -out
	BYTE* _outDatas;
	long  _dataSize;
	long  _startAddr;

	list<HexBlock> _hexBlocks;

public:
	IntelHex(void) { 
		_pHexFileHandler = NULL; 
		_outDatas = NULL;
        _dataSize = 0;
	}
	IntelHex(const CString& hexFileName) {
		
		_fileName = hexFileName;
		_pHexFileHandler = NULL;

		_inStr = "";

		_outDatas = NULL;
		_dataSize = 0;
		_startAddr = 0x00;
	}
	~IntelHex(void) { 
		if (_pHexFileHandler != NULL) {
			fclose(_pHexFileHandler);
			_pHexFileHandler = NULL;
		}

		if (_outDatas != NULL) {
			delete [] _outDatas;
			_outDatas = NULL;
		}
	}

	bool read();
	bool parse(string& inStr);
	bool parse();

	// getter 
	string getHex();
	void getBin(BYTE* &outDatas, int &dataSize);
	string getFilePath();
	
	// setter
	void setHex(string &inStr) { _inStr = inStr; }

	// write
	FILE* getFileWriteHandler();
	bool write();
	void writeToBinFile(FILE* fileHandler);
};

