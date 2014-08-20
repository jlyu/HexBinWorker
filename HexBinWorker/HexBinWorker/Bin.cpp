#include "StdAfx.h"
#include "Bin.h"


bool Bin::openBinFile(const CString& binFileName) {

	CT2A asciiFileName(binFileName);
	fopen_s(&_pBinFileHandler, asciiFileName, "r");

	if(_pBinFileHandler == NULL) {  
        TRACE("Open BIN file error.\n");  
        return false;  
    }  
	return true;
}
bool Bin::read() {

	if (!openBinFile(_fileName)) { 
		return false; 
	}
	if (_pBinFileHandler == NULL) { 
		return false; 
	}


	fseek(_pBinFileHandler, 0, SEEK_END);
	_dataSize = ftell(_pBinFileHandler);
	fseek(_pBinFileHandler, 0, SEEK_SET);

	const int bufferSize = FLASH_VOLUME * 1024;
	if (_dataSize > bufferSize) {
		return false;
	}

	if (_inDatas == NULL) {
		_inDatas = new BYTE[bufferSize];
	}
	
	fread(_inDatas, 1, _dataSize, _pBinFileHandler);

	char ch[3];
	for (int i=0; i< _dataSize; i++) {
		sprintf(ch, "%02X", _inDatas[i]);
		_inStr += ch;
	}

	return true;
}

bool Bin::parse(BYTE *pDatas, int dataSize) {
	_inDatas = pDatas;
	_dataSize = dataSize;
	return parse();
}
bool Bin::parse() {

	if (_inDatas == NULL) {
		return false;
	}

	BYTE dbSum, dbLen;
	CString bufferLine  = _T("");
	CString bufferBlock = _T("");

	// the 1st Line
	bufferLine.Format(_T(":02%04X040000"), _startAddr);
	bufferBlock += bufferLine;
	dbSum = 02 + (BYTE)(_startAddr>>8) + (BYTE)_startAddr + 04;
	dbSum = ~dbSum + 1;
	bufferLine.Format(_T("%02X\r\n"), dbSum);
	bufferBlock += bufferLine;

	// the part of datas
	for (int l = 0; l < _dataSize; l++) {
		if (l % 16 == 0) {
			dbLen = static_cast<BYTE>((_dataSize - l >= 16) ? 16 : _dataSize - l);
			bufferLine.Format(_T(":%02X%04X00"), dbLen, l);
			bufferBlock += bufferLine;

			dbSum = dbLen + (BYTE)(l>>8) + (BYTE)l + 00;
		}
		bufferLine.Format(_T("%02X"), _inDatas[l]);
		bufferBlock += bufferLine;

		dbSum += _inDatas[l];
		if (l % 16 == dbLen-1) {
			dbSum = ~dbSum + 1;
			bufferLine.Format(_T("%02X\r\n"), dbSum);
			bufferBlock += bufferLine;
		}
	}

	// the last line
	bufferBlock += CString(_T(":00000001FF"));

	_outStr = CT2A(bufferBlock);
	return true;
}


// MARK: -text
string Bin::getBin() {

	string resultStr = "";
	const int inStrSize = _inStr.size();
	for (int i=0; i<inStrSize; i+=2) {
		resultStr += _inStr[i];
		resultStr += _inStr[i+1];
		resultStr += " ";

		if ( (i/2) % RECORD_LENGTH == RECORD_LENGTH -1) {
			resultStr += "\r\n";
		}
	}
	return resultStr;
}
string Bin::getHex() {
	return _outStr;
}
string Bin::getFilePath() {
	string filePathStr = CT2A(_fileName);
	return filePathStr;
}


// MARK: - Write
FILE* Bin::getFileWriteHandler() {
	if (_pBinFileHandler != NULL) {
		fclose(_pBinFileHandler);
		_pBinFileHandler = NULL;
	}

	CT2A asciiFileName(_fileName);
	fopen_s(&_pBinFileHandler, asciiFileName, "wb");
	return _pBinFileHandler;
}
bool Bin::write() {

	_pBinFileHandler = getFileWriteHandler();

	if (_pBinFileHandler == NULL) {
		TRACE("Open Bin file error.\n");
		return false;
	}

	fwrite(_inDatas, 1, _dataSize, _pBinFileHandler);
	fclose(_pBinFileHandler);
	_pBinFileHandler = NULL;

	return true;
}
void Bin::writeToHexFile(FILE* fileHandler) {

	if (fileHandler == NULL) {  
        printf("Open hex file error.\n");  
        return;  
    }

	fwrite(_outStr.c_str(), 1, _outStr.length(), fileHandler);

	fclose(fileHandler);
	fileHandler = NULL;
}