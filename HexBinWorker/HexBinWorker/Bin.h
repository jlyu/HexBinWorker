#pragma once
#include <iostream>

using namespace std;

class Bin
{
private:
	CString _fileName;
	FILE* _pBinFileHandler;

	
	bool openBinFile(const CString& binFileName);

public:
	Bin(void) { } 
	Bin(CString& binFileName) {
		_fileName = binFileName;
		_pBinFileHandler = NULL;
		//openBinFile(_fileName);
	}

	~Bin(void) {
		if (_pBinFileHandler != NULL) {
			fclose(_pBinFileHandler);
			_pBinFileHandler = NULL;
		}
	}

	FILE* getFileWriteHandler();
	void parse();


	string _binEditField;
	string getEditFieldText();
	string getFilePath();

	//output
	void writeToHexFile(FILE* fileHandler);
};

