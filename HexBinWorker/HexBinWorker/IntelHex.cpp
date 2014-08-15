#include "StdAfx.h"
#include "IntelHex.h"
#include <string>
#include <regex> 
using namespace std;

/*
打开并读取文件
逐行，内容校验
正则 match hex 的域
校验数据长度及校验和
	通过：分析 recordType，发现 04 类型就直接 new 一个新 block
		  实时计算数据的有效长度
		  是否要记录每条记录的长度？？
	最后按顺序把 hex 转成 bin 写入
以上，Hex -> Bin 
- - -
反之， bin -> hex 
直接填充即可，长度固定 8 bit * 16 block
起始地址 0x00
8bit 二进制转16进制

*/
/****************************************************************************** 
*                                Methods                                      *
******************************************************************************/
bool IntelHex::openHexFile(CString& hexFileName) {

	CT2A asciiFileName(hexFileName);
	fopen_s(&_pHexFileHandler, asciiFileName, "r");

	if(!_pHexFileHandler) {  
        printf("Open file error.\n");  
        return false;  
    }  
	return true;
}
bool IntelHex::checkLine(const char *src) {
	
	string checkPattern("[:0-9A-F\\r\\n]*");
	string lineString = src;

	std::regex_constants::syntax_option_type optionType = std::regex_constants::icase;
	std::regex regExpress(checkPattern, optionType);

	if(std::regex_match(lineString, regExpress)) {  
        return true;  
    } else {  
        return false;  
    }  
}
bool IntelHex::matchLine(const char *src, HexRecord& hexRecord) {
	string matchPattern("^:(\\w{2})(\\w{4})(\\w{2})(\\w*)(\\w{2})$");
	string lineString = src;
	std::regex_constants::syntax_option_type optionType = std::regex_constants::icase;
	std::regex regExpress(matchPattern, optionType);
	std::smatch matchResult;

	if(std::regex_match(lineString, matchResult, regExpress)) {  
		hexRecord.dataLength = string(matchResult[1].first, matchResult[1].second);
		hexRecord.startAddress = string(matchResult[2].first, matchResult[2].second);
		hexRecord.recordType = string(matchResult[3].first, matchResult[3].second);
		hexRecord.data = string(matchResult[4].first, matchResult[4].second);
		hexRecord.sumCheck = string(matchResult[5].first, matchResult[5].second);
		hexRecord.origRecord = lineString;
		return true;
	}
	return false;
}
bool IntelHex::verifyLine(const HexRecord& hexRecord) {
	// verify length of datas
	const unsigned int dateSize = hexRecord.data.size();
	unsigned int dataLength = hexToDec(hexRecord.dataLength);

	if (dateSize != dataLength * 2) {
		return false;
	}
	
	// verify sumcheck
	const int origRecordSize = hexRecord.origRecord.size();
	unsigned int sumCheck = 0;
	string origRecordCopy = hexRecord.origRecord;
	typedef string::const_iterator cIter;
	for (cIter i = origRecordCopy.begin()+1 ; i < origRecordCopy.end() - 2; i += 2) {
		string hexStr = string(i, i+2);
		sumCheck += hexToDec(hexStr);
		sumCheck &= 0xFF;
	}
	sumCheck = (~sumCheck + 1) & 0xFF;  // 206 -> CE

	string hexSumCheckStr = "0x" + hexRecord.sumCheck;
	unsigned int decSumCheck = strtol(hexSumCheckStr.c_str(), NULL, 16);
	
	if (sumCheck != decSumCheck) {
		return false;
	}

	return true;
}
void IntelHex::splitHexData(const string& inData, vector<BYTE>& outData) {
	//08280032345A -> [08,28,00,32,34,5A]
	typedef string::const_iterator cIter;
	for (cIter i = inData.begin() ; i < inData.end(); i += 2) {

		const string hexStr = string(i, i+2);
		BYTE decByte = (BYTE)strtol(hexStr.c_str(), NULL, 16);  // hex -> dec
		outData.push_back(decByte);
	}
}
bool IntelHex::appendDatas(const HexRecord& hexRecord){
	unsigned int recordTypeInt = hexToDec(hexRecord.recordType);

	switch (recordTypeInt)
	{
		case 0: {
			if (_hexBlocks.empty()) {
				HexBlock newHexBlock = HexBlock();
				_hexBlocks.push_front(newHexBlock);
			}

			list<HexBlock>::iterator hexBlocksIterator = _hexBlocks.begin();
			const int startAddr = hexToDec(hexRecord.startAddress);
			const int dataLength = hexToDec(hexRecord.dataLength);

			vector<BYTE> splitedData;
			splitHexData(hexRecord.data, splitedData);

			for (int i = startAddr; i < startAddr + dataLength; i++) {
				hexBlocksIterator->datas[i] = splitedData[i-startAddr];
			}
				
			const int dataValidLength = hexBlocksIterator->validLength;
			if (startAddr > dataValidLength) {
				hexBlocksIterator->validLength += startAddr - dataValidLength;
			}
			hexBlocksIterator->validLength += dataLength;
				
			break;
		}
		case 1: { //end of file
			return true;
			break;
		}
		case 2: {
			break;
		}
		case 4: {
			HexBlock newHexBlock = HexBlock();
			// TODO: parse type 04 here
			// ...
			_hexBlocks.push_front(newHexBlock);
			break;
		}
		case 5: {
			break;
		}

	} 

	return true;
}
void IntelHex::hexBlocksToOutDatas() {
	// renew outDatas
	if (_outDatas != NULL) {
		delete [] _outDatas;
	} 
	_outDatas = new BYTE[FLASH_VOLUME * 64];
	
	typedef list<HexBlock>::reverse_iterator ListRevIter;
	ListRevIter rIter = _hexBlocks.rbegin();

	if (rIter == _hexBlocks.rend()) {
		return; 
	}

	_dataSize = rIter->validLength;

	// TODO: copy the 1st datasBlock only
	for (int i=0; i<_dataSize; i++) {
		_outDatas[i] = rIter->datas[i];
	}

}	
unsigned int IntelHex::hexToDec(const string& str) {
	
	unsigned int result = 0;
	
	string strCopy = str;
	transform(strCopy.begin(), strCopy.end(), strCopy.begin(), ::toupper);
	const int strSize = strCopy.size();

	for (int i = 0; i < strSize; i++) {
		if (strCopy[i] >= '0' && strCopy[i] <= '9') {
			result = result * 16 + (unsigned int)(strCopy[i] - '0');
		} else if (str[i] >= 'A' && str[i] <= 'F') {
			result = result * 16 + (unsigned int)(strCopy[i] - 'A' + 10); 
		} else {
			result = result * 16 + (unsigned int)(strCopy[i] - 'A' + 10); 
		}
	}

    return result;
}




/****************************************************************************** 
*                                Interface                                    *
******************************************************************************/
bool IntelHex::read() {

	if (!openHexFile(_fileName)) return false;
	if (_pHexFileHandler == NULL) return false;

	const int bufferSize = sizeof(char) * 128;
	char *lineBuffer = new char[bufferSize];
	if (lineBuffer == NULL ) {  
		TRACE("Apply for memory failed! -> @[IntelHex.cpp IntelHex::read()]\n");  
        return false;  
    } 

	while (fscanf_s(_pHexFileHandler, "%s", lineBuffer, bufferSize) != EOF) {
		_inStr += lineBuffer;
		_inStr += "\r\n";
	}

	return true;
}

bool IntelHex::parse(string& inStr) {

	_inStr = inStr;

	if (!_hexBlocks.empty()) {
		_hexBlocks.clear();
	}
		
	return parse();
}
bool IntelHex::parse() {

	if (_inStr.empty()) {
		return false;
	}

	char *pWritableCopy = new char[_inStr.size() + 1];
	std::copy(_inStr.begin(), _inStr.end(), pWritableCopy);
	pWritableCopy[_inStr.size()] = '\0';

	char* lineBuffer;
	lineBuffer = strtok(pWritableCopy, "\r\n");
	

	// parse hex file each line
	bool parseError = false;
	while (lineBuffer != NULL) { 
		
		bool checkLineOK = checkLine(lineBuffer);
		if (!checkLineOK) {
			parseError = true;
			break;
		}

		HexRecord hexRecord;
		bool matchLineOK = matchLine(lineBuffer, hexRecord);
		if (!matchLineOK) {
			parseError = true;
			break;
		}

		bool verifyLineOK = verifyLine(hexRecord);
		if (!verifyLineOK) {
			parseError = true;
			break;
		}	

		bool appendDatasOK = appendDatas(hexRecord);
		if (!appendDatasOK)	{
			parseError = true;
			break;
		}

		lineBuffer = strtok (NULL, "\r\n");
	}


	delete [] pWritableCopy;

	if (parseError) {
		_hexBlocks.clear();
		return false;
	} else {
		hexBlocksToOutDatas();
		return true;
	}
}


// - Getter / Setter
string IntelHex::getHex() {
	return _inStr;
}
void IntelHex::getBin(BYTE* &outDatas, int &dataSize) {
	outDatas = _outDatas;
	dataSize = _dataSize;
}
string IntelHex::getFilePath() {
	string filePathStr = CT2A(_fileName);
	return filePathStr;
}


// - Write
FILE* IntelHex::getFileWriteHandler() {
	if (_pHexFileHandler != NULL) {
		fclose(_pHexFileHandler);
		_pHexFileHandler = NULL;
	}

	CT2A asciiFileName(_fileName); 
	fopen_s(&_pHexFileHandler, asciiFileName, "wb");
	return _pHexFileHandler;
}
bool IntelHex::write() {

	_pHexFileHandler = getFileWriteHandler();

	if (_pHexFileHandler == NULL) {
		TRACE("Open Hex file error.\n");
		return false;
	}
	
	fwrite(_inStr.c_str(), 1, _inStr.length(), _pHexFileHandler);

	fclose(_pHexFileHandler);
	_pHexFileHandler = NULL;
	return true;
}
void IntelHex::writeToBinFile(FILE* fileHandler) { 
	
	if (fileHandler == NULL) {  
        printf("Open file error.\n");  
        return;  
    }

	//start to write  
	typedef list<HexBlock>::reverse_iterator ListRevIter;
	for(ListRevIter rIter = _hexBlocks.rbegin(); rIter != _hexBlocks.rend(); rIter++) {
		int validLength = rIter->validLength;
		fwrite(rIter->datas, 1, validLength, fileHandler);
	}

	fclose(fileHandler);
	fileHandler = NULL;
}