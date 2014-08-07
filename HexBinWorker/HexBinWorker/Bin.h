#pragma once
#include <iostream>

#include "defineV.h"

using namespace std;

class Bin
{
private:
	CString _fileName;
	FILE* _pBinFileHandler;

	long _startAddr; 
	long _dataSize;

	string _inStr;  // without formatting
	string _outStr;
	
	bool openBinFile(const CString& binFileName);

public:
	Bin(void) { _pBinFileHandler = NULL; } 
	Bin(CString& binFileName) {

		_fileName = binFileName;
		_pBinFileHandler = NULL;
		
		_inStr = "";
		_outStr = "";

		_startAddr = 0x00;
		_dataSize = 0;
	}

	~Bin(void) {
		if (_pBinFileHandler != NULL) {
			fclose(_pBinFileHandler);
			_pBinFileHandler = NULL;
		}
	}

	FILE* getFileWriteHandler();

	bool read();
	bool parse();

	// text
	string getBin();
	string getHex();
	

	string _binEditField;
	string getEditFieldText();
	string getFilePath();

	//output
	void writeToHexFile(FILE* fileHandler);
};

