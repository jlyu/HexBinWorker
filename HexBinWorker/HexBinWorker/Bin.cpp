#include "StdAfx.h"
#include "Bin.h"


FILE* Bin::getFileWriteHandler() {

	CT2A asciiFileName(_fileName);
	_pBinFileHandler = fopen(asciiFileName, "wb");
	//fopen_s(&_pBinFileHandler, asciiFileName, "wb");
	return _pBinFileHandler;
}

bool Bin::openBinFile(const CString& binFileName) {

	CT2A asciiFileName(binFileName);
	_pBinFileHandler = fopen(asciiFileName, "r");
	//fopen_s(&_pBinFileHandler, asciiFileName, "r");

	if(_pBinFileHandler == NULL) {  
        printf("Open file error.\n");  
        return false;  
    }  
	return true;
}

void Bin::parse() {

	const int bufferSize = 65536;
	BYTE* pBuffer = new BYTE[bufferSize];
	CString bufferLine  = _T("");
	CString bufferBlock = _T("");

	long lStartAddr = 0x00; //TODO
	BYTE dbSum, dbLen;
	
	
  
    if (pBuffer == NULL) {  
		printf("Apply for memory failed.!\n");  
        return;  
    }  

	if (!openBinFile(_fileName)) return;
	fseek(_pBinFileHandler, 0, SEEK_END);
	long dataSize = ftell(_pBinFileHandler);
	fseek(_pBinFileHandler, 0, SEEK_SET);

	if (dataSize > bufferSize) {
		return;
	}

	fread(pBuffer, 1, dataSize, _pBinFileHandler);

	printf(":02%04X04", lStartAddr);
	bufferLine.Format(_T(":02%04X040000"), lStartAddr);
	bufferBlock += bufferLine;

	dbSum = 02 + (BYTE)(lStartAddr>>8) + (BYTE)lStartAddr + 04;
	dbSum = ~dbSum + 1;
	printf("%02X\r\n", dbSum);
	bufferLine.Format(_T("%02X\r\n"), dbSum);
	bufferBlock += bufferLine;

	for (int l = 0; l < dataSize; l++) {
		if (l%16 == 0) {
			dbLen = (dataSize - l >= 16) ? 16 : dataSize - l;
			printf(":%02X%04X00", dbLen, l);
			bufferLine.Format(_T(":%02X%04X00"), dbLen, l);
			bufferBlock += bufferLine;

			dbSum = dbLen + (BYTE)(l>>8) + (BYTE)l + 00;
		}
		printf("%02X", pBuffer[l]);
		bufferLine.Format(_T("%02X"), pBuffer[l]);
		bufferBlock += bufferLine;

		dbSum += pBuffer[l];
		if (l%16 == dbLen-1) {
			dbSum = ~dbSum + 1;
			printf("%02X\r\n", dbSum);
			bufferLine.Format(_T("%02X\r\n"), dbSum);
			bufferBlock += bufferLine;
		}
	}

	printf(":00000001FF");
	bufferBlock += CString(_T(":00000001FF"));

	/*while (fscanf(_pBinFileHandler, "%s", lineBuffer) != EOF) {
		printf("%s\n", lineBuffer);
		
		_binEditField += lineBuffer;
		_binEditField += "\r\n";
	}*/

	//delete(lineBuffer);
	_binEditField = CT2A(bufferBlock);
	delete [] pBuffer;
}



string Bin::getEditFieldText() {
	return _binEditField;
}

string Bin::getFilePath() {
	string filePathStr = CT2A(_fileName);
	return filePathStr;
}

