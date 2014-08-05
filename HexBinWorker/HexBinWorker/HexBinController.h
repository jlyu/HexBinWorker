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
	void parse();
	bool isHexFile(const CString& fileName); 

	void getFilePath(CString& hexPath, CString& binPath);
	void getEditFieldText(CString& hexField, CString& binField);


	// output
	void writeToBinFile();

};

