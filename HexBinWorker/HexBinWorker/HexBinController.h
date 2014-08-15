#pragma once
#include "Bin.h"
#include "IntelHex.h"


class HexBinController {

	enum ProcessType{
		PROCESS_HEX_TO_BIN,
		PROCESS_BIN_TO_HEX,
	};
public:
	// type
	void typeHexToBin() { _processType = PROCESS_HEX_TO_BIN; }
	void typeBinToHex() { _processType = PROCESS_BIN_TO_HEX; }

private:
	IntelHex _hex;
	Bin      _bin;
	ProcessType _processType;

public:
	HexBinController(void) { }
	~HexBinController(void) { }

	void init(CString& fileName);

	bool isHexFile(const CString& fileName);
	void read(const CString& fileName);
	void readHex();
	void readBin();

	bool parse(const CString& fileName);
	bool parseHex(string& inStr);
	void parseBin(BYTE *pDatas, int dataSize);
	

	// text
	//void getText(CString& hexText, CString& binText);
	void getBinText(CString& binText);
	void getHexText(CString& hexText);
	void getFilePath(CString& hexPath, CString& binPath);

	// getter / setter
	void getBinDatas(BYTE* &datas, int &dataSize);
	void setHexData(string hexData);


	// write
	void writeHex();
	void writeToBinFile();
	void writeToHexFile();

};

