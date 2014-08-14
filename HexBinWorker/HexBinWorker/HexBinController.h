#pragma once
#include "Bin.h"
#include "IntelHex.h"


class HexBinController
{
	enum ProcessType{
		PROCESS_HEX_TO_BIN,
		PROCESS_BIN_TO_HEX,
	};

private:
	IntelHex _hex;
	Bin      _bin;
	ProcessType _processType;

public:
	HexBinController(void) { }
	~HexBinController(void);

	void init(CString& fileName);

	void read(const CString& fileName);
	void readHex();
	void readBin();

	bool parse(const CString& fileName);
	bool parseHex(string& inStr);
	void parseBin(BYTE *pDatas, int dataSize);

	bool isHexFile(const CString& fileName); 

	void getFilePath(CString& hexPath, CString& binPath);
	
	// type
	void typeHexToBin() { _processType = PROCESS_HEX_TO_BIN; }
	void typeBinToHex() { _processType = PROCESS_BIN_TO_HEX; }

	// text
	void getText(CString& hexText, CString& binText);
	
	void getBinText(CString& binText);

	// redo text
	void getHexText(CString& hexText);


	// getter / setter
	void getBinDatas(BYTE* &datas, int &dataSize);
	//void getHexDatas(BYTE* &datas, int &dataSize);


	// write
	void writeToBinFile();
	void writeToHexFile();

};

