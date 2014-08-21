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
	HexBinController(void)  { }
	~HexBinController(void) { }

	void init(CString& fileName);
	bool isHexFile(const CString& fileName);

	void read(const CString& fileName);

	bool parse(const CString& fileName);
	bool parseHex(string& inStr);
	bool parseBin(BYTE *pDatas, int dataSize);

	// text
	void getHexText(CString& hexText);
	void getFilePath(CString& hexPath, CString& binPath);

	// getter / setter
	void getBinDatas(BYTE* &datas, int &dataSize);
    void getHexDatas(BYTE* &datas, int &dataSize);
	void setHexDatas(string hexData);
    void setBinDatas(BYTE* datas, int dataSize);

	// write
	bool writeHex();
	bool writeBin();
	void writeToBinFile();
	void writeToHexFile();
};

