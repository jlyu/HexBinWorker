#pragma once
#include <iostream>

#include "defineV.h"

using namespace std;

class Bin
{
private:
	CString _fileName;
	FILE* _pBinFileHandler;

	// -in
	string _inStr;  // without formatting
	BYTE *_inDatas;
	long _dataSize;
	long _startAddr; 
	

	// -out
	string _outStr;
	
	bool openBinFile(const CString& binFileName);

public:
	Bin(void) { 
		_pBinFileHandler = NULL;
		_inDatas = NULL;
	}

	Bin(CString& binFileName) {

		_fileName = binFileName;
		_pBinFileHandler = NULL;
		
		_inStr = "";
		_inDatas = NULL;

		_outStr = "";

		_startAddr = 0x00;
		_dataSize = 0;
	}

	~Bin(void) {
		if (_pBinFileHandler != NULL) {
			fclose(_pBinFileHandler);
			_pBinFileHandler = NULL;
		}

		if (_inDatas != NULL) {
			delete [] _inDatas;
			_inDatas = NULL;
		}
	}

	FILE* getFileWriteHandler();

	bool read();
	bool parse(BYTE *pDatas, int dataSize);
	bool parse();

	// getter / setter
	void   getBin(BYTE* &datas, int &dataSize) { 
		if (_inDatas == NULL) {
			_inDatas = new BYTE[FLASH_VOLUME * 64];
		}
		datas = _inDatas;
		dataSize =  _dataSize; 
	}

	// text
	string getBin();
	string getHex();
	

	string _binEditField;
	string getEditFieldText();
	string getFilePath();

	// write
	bool write();
	void writeToHexFile(FILE* fileHandler);
};

