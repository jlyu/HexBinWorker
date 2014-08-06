#pragma once
#include "Bin.h"
#include "IntelHex.h"


class HexBinController
{
private:
	IntelHex _hex;
	Bin      _bin;


public:
	HexBinController(void) { }
	~HexBinController(void);

	void init(CString& fileName);

	void read(const CString& fileName);
	void readHex();
	void readBin();

	void parse(const CString& fileName);
	void parseHex();
	void parseBin();

	bool isHexFile(const CString& fileName); 

	void getFilePath(CString& hexPath, CString& binPath);
	void getEditFieldText(CString& hexField, CString& binField);


	// output
	void writeToBinFile();
	void writeToHexFile();

};

