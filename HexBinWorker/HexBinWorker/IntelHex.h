#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>
using namespace std;

class IntelHex
{
private:

	struct HexRecord {
		string dataLength;
		string startAddress;
		string recordType;
		string data;
		vector<string> datas;
		string sumCheck;
		string origRecord;
	};

	/*
	struct DecRecord {
		unsigned int dataLength;
		unsigned int startAddress;
		unsigned int recordType;
		vector<unsigned int> data;
		unsigned int sumCheck;
	};
	*/

	struct HexLine {
		//int lineNo;
		HexRecord hexRecord;
		//DecRecord decRecord;
	};


	CString fileName;
	FILE *pHexFile;
	vector<HexLine> fileContent;



	bool open();
	bool checkLine(const char *src);
	bool matchLine(const char *src);
	bool verifyLine(const HexRecord& hexRecord);

	void hexStringToDec();
	unsigned int hexToDec(const string& str);

	bool formatParse(const char *src, const int lineNo);
	bool hexFormatParse(const char *src, char *dst);

	//HEXTODEC

public:
	IntelHex(void) { }
	IntelHex(const CString& hexFileName) {
		fileName = hexFileName;
	}

	~IntelHex(void) { 
		if (pHexFile){
			fclose(pHexFile);
		}
	}

	void parse();

};

