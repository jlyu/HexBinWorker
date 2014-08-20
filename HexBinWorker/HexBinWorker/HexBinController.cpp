#include "StdAfx.h"
#include "HexBinController.h"


// MARK: init
bool HexBinController::isHexFile(const CString& fileName) {

	CString fileExtension = PathFindExtension(fileName);
	fileExtension.MakeLower();

	if (fileExtension == _T(".hex")) {
		return true;
	}

	return false;
}
void HexBinController::init(CString& fileName) {
	CString hexFileName; 
	CString binFileName;

	string fileNameStr = CT2A(fileName);

	if (isHexFile(fileName)) {
		hexFileName = fileName;

		int pos = fileNameStr.find(".hex");
		fileNameStr.replace(pos, 4, ".bin");
		
		binFileName = fileNameStr.c_str();
		
		_processType = PROCESS_HEX_TO_BIN;
	} else {
		binFileName = fileName;

		int pos = fileNameStr.find(".bin");
		fileNameStr.replace(pos, 4, ".hex");
		
		hexFileName = fileNameStr.c_str();
		_processType = PROCESS_BIN_TO_HEX;
	}

	_hex = IntelHex(hexFileName);
	_bin = Bin(binFileName);
}


// MARK: read
void HexBinController::read(const CString& fileName){
	if (isHexFile(fileName)) {
		_hex.read();
        typeHexToBin();
	} else {
		_bin.read();
        typeBinToHex();
	}
}


// MARK: parse
bool HexBinController::parse(const CString& fileName) {
	
	if (isHexFile(fileName)) {
		return _hex.parse();
	} else {
		return _bin.parse();
	}
}
bool HexBinController::parseHex(string& inStr) { 
	_processType = PROCESS_HEX_TO_BIN;
	return _hex.parse(inStr); 
}
void HexBinController::parseBin(BYTE *pDatas, int dataSize) { 
	_processType = PROCESS_BIN_TO_HEX;
	_bin.parse(pDatas, dataSize); 
}


// MARK: getter/setter 
void HexBinController::getHexText(CString& hexText) {
	if (_processType == PROCESS_HEX_TO_BIN) {
		hexText = _hex.getHex().c_str();
	} else if (_processType == PROCESS_BIN_TO_HEX) {
		hexText = _bin.getHex().c_str();
	}
}
void HexBinController::getFilePath(CString& hexPath, CString& binPath) {
	hexPath = _hex.getFilePath().c_str();
	binPath = _bin.getFilePath().c_str();
}
void HexBinController::getBinDatas(BYTE* &datas, int &dataSize) {

	if (_processType == PROCESS_HEX_TO_BIN) {

		_hex.getBin(datas, dataSize);

	} else if (_processType == PROCESS_BIN_TO_HEX) {

		_bin.getBin(datas, dataSize);
	}
	
}
void HexBinController::getHexDatas(BYTE* &datas, int &dataSize) {
    _hex.getBin(datas, dataSize);
}
void HexBinController::setHexDatas(string hexData) {
	_hex.setHex(hexData); 
}
void HexBinController::setBinDatas(BYTE* datas, int dataSize) {
    _bin.setBin(datas, dataSize); 
}

// MARK: write
bool HexBinController::writeHex() {
	return _hex.write();
}
bool HexBinController::writeBin() {
	return _bin.write();
}
void HexBinController::writeToBinFile() {

	FILE* pFileHandler = _bin.getFileWriteHandler();
	_hex.writeToBinFile(pFileHandler);

}
void HexBinController::writeToHexFile() {
	FILE* pFileHandler = _hex.getFileWriteHandler();
	_bin.writeToHexFile(pFileHandler);
}