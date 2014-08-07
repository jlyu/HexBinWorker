#pragma once
#include "Bin.h"
#include "IntelHex.h"


class HexBinController
{
	enum ProcessType{
		PROCESS_HEX,
		PROCESS_BIN,
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
	void parseBin();

	bool isHexFile(const CString& fileName); 

	void getFilePath(CString& hexPath, CString& binPath);
	

	// text
	void getText(CString& hexText, CString& binText);
	void getHexText(CString& hexText) {}
	void getBinText(CString& binText);
	


	// write
	void writeToBinFile();
	void writeToHexFile();

};

