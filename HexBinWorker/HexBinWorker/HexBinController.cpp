#include "StdAfx.h"
#include "HexBinController.h"


HexBinController::~HexBinController(void) {
	//TODO
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
		
		_processType = PROCESS_HEX;
	} else {
		binFileName = fileName;

		int pos = fileNameStr.find(".bin");
		fileNameStr.replace(pos, 4, ".hex");
		
		hexFileName = fileNameStr.c_str();
		_processType = PROCESS_BIN;
	}

	_hex = IntelHex(hexFileName);
	_bin = Bin(binFileName);
}

bool HexBinController::isHexFile(const CString& fileName) {

	CString fileExtension = PathFindExtension(fileName);
	fileExtension.MakeLower();

	if (fileExtension == _T(".hex"))
	{
		return true;
	}

	return false;
}

void HexBinController::read(const CString& fileName){
	if (isHexFile(fileName)) {
		_hex.read();
	} else {
		_bin.read();
	}
}
void HexBinController::readHex() { _hex.read(); }
void HexBinController::readBin() { /*_bin.parse();*/ }

void HexBinController::parse(const CString& fileName) {
	
	if (isHexFile(fileName)) {
		_hex.parse();
	} else {
		_bin.parse();
	}
}
void HexBinController::parseHex() { _hex.parse(); }
void HexBinController::parseBin() { _bin.parse(); }

void HexBinController::getText(CString& hexText, CString& binText) {
	if (_processType == PROCESS_HEX) {
		hexText = _hex.getHex().c_str();
		binText = _hex.getBin().c_str();
	} else if (_processType == PROCESS_BIN) {

	}
}
void HexBinController::getFilePath(CString& hexPath, CString& binPath) {
	hexPath = _hex.getFilePath().c_str();
	binPath = _bin.getFilePath().c_str();
}

// MARK: text
void HexBinController::getBinText(CString& binText) {
	binText = _hex.getBin().c_str();
}


// MARK: write
void HexBinController::writeToBinFile() {

	FILE* pFileHandler = _bin.getFileWriteHandler();
	_hex.writeToBinFile(pFileHandler);

}
void HexBinController::writeToHexFile() {
	FILE* pFileHandler = _hex.getFileWriteHandler();
	_bin.writeToHexFile(pFileHandler);
}