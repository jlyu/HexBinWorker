#include "StdAfx.h"
#include "Bin.h"



bool Bin::openBinFile(const CString& binFileName) {

	CT2A asciiFileName(binFileName);
	fopen_s(&_pBinFileHandler, asciiFileName, "r");

	if(_pBinFileHandler == NULL) {  
        printf("Open file error.\n");  
        return false;  
    }  
	return true;
}

bool Bin::read() {

	if (!openBinFile(_fileName)) { return false; }
	if (_pBinFileHandler == NULL) { return false; }

	fseek(_pBinFileHandler, 0, SEEK_END);
	_dataSize = ftell(_pBinFileHandler);
	fseek(_pBinFileHandler, 0, SEEK_SET);

	const int bufferSize = FLASH_VOLUME * 64;
	if (_dataSize > bufferSize) {
		return false;
	}

	fread(_inDatas, 1, _dataSize, _pBinFileHandler);

	char ch[3];
	for (int i=0; i< _dataSize; i++) {
		sprintf(ch, "%02X", _inDatas[i]);
		_inStr += ch;
	}

	return true;
}

bool Bin::parse() {

	if (_inStr.empty()) {
		return false;
	}
	if (_inDatas == NULL) {
		return false;
	}

	//// TODO: verify inStr [0-9A-Za-z]
	////       trim all space and enter char
	////if (_inStr.size() != 2 * _dataSize) {
	////	return false;
	////}

	//// begin parse
	//char *inStrCopy = new char[_inStr.size()+1];
	//std::copy(_inStr.begin(), _inStr.end(), inStrCopy); 
	//inStrCopy[_inStr.size()] = '\0';

	BYTE dbSum, dbLen;
	CString bufferLine  = _T("");
	CString bufferBlock = _T("");

	// the 1st Line
	bufferLine.Format(_T(":02%04X040000"), _startAddr);
	bufferBlock += bufferLine;
	dbSum = 02 + (BYTE)(_startAddr>>8) + (BYTE)_startAddr + 04;
	dbSum = ~dbSum + 1;
	// printf("%02X\r\n", dbSum);
	bufferLine.Format(_T("%02X\r\n"), dbSum);
	bufferBlock += bufferLine;

	// the part of datas
	for (int l = 0; l < _dataSize; l++) {
		if (l % 16 == 0) {
			dbLen = static_cast<BYTE>((_dataSize - l >= 16) ? 16 : _dataSize - l);
			// printf(":%02X%04X00", dbLen, l);
			bufferLine.Format(_T(":%02X%04X00"), dbLen, l);
			bufferBlock += bufferLine;

			dbSum = dbLen + (BYTE)(l>>8) + (BYTE)l + 00;
		}
		// printf("%02X", inStrCopy[l]);
		bufferLine.Format(_T("%02X"), _inDatas[l]);
		bufferBlock += bufferLine;

		dbSum += _inDatas[l];
		if (l % 16 == dbLen-1) {
			dbSum = ~dbSum + 1;
			//printf("%02X\r\n", dbSum);
			bufferLine.Format(_T("%02X\r\n"), dbSum);
			bufferBlock += bufferLine;
		}
	}

	// the last line
	//printf(":00000001FF");
	bufferBlock += CString(_T(":00000001FF"));

	_outStr = CT2A(bufferBlock);
	//delete [] inStrCopy;

	return true;
}


// -MARK: text
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


string Bin::getEditFieldText() {
	return _binEditField;
}

string Bin::getFilePath() {
	string filePathStr = CT2A(_fileName);
	return filePathStr;
}

FILE* Bin::getFileWriteHandler() {

	CT2A asciiFileName(_fileName);
	//_pBinFileHandler = fopen(asciiFileName, "wb");
	fopen_s(&_pBinFileHandler, asciiFileName, "wb");
	return _pBinFileHandler;
}

void Bin::writeToHexFile(FILE* fileHandler) {

	if (fileHandler == NULL) {  
        printf("Open hex file error.\n");  
        return;  
    }


	fwrite(_binEditField.c_str(), 1, _binEditField.length(), fileHandler);

	fclose(fileHandler);
	fileHandler = NULL;
}