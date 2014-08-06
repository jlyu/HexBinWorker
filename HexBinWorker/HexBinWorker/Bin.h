#pragma once
#include <iostream>

#include "defineV.h"

using namespace std;

class Bin
{
private:
	CString _fileName;
	FILE* _pBinFileHandler;

	string _inStr;
	string _outStr;
	
	bool openBinFile(const CString& binFileName);

public:
	Bin(void) { _pBinFileHandler = NULL; } 
	Bin(CString& binFileName) {

		_fileName = binFileName;
		_pBinFileHandler = NULL;
		
		_inStr = "";
		_outStr = "";
	}

	~Bin(void) {
		if (_pBinFileHandler != NULL) {
			fclose(_pBinFileHandler);
			_pBinFileHandler = NULL;
		}
	}

	FILE* getFileWriteHandler();

	bool read();
	void parse();


	string _binEditField;
	string getEditFieldText();
	string getFilePath();

	//output
	void writeToHexFile(FILE* fileHandler);
};

