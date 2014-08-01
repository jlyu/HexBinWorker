#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>

using namespace std;

const int FLASHVOLUME = 64; // KB

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

	struct HexBlock {
		BYTE datas[FLASHVOLUME*1024]; // alloc 64K, init with 00
		int validLength;

		HexBlock() {
			memset(datas, 0x00, FLASHVOLUME*1024);
			validLength = 0;
		}
	};


	CString fileName;
	FILE *pHexFile;
	list<HexBlock> hexBlocks;



	bool openHexFile(const CString& hexFileName);
	bool checkLine(const char *src);
	bool matchLine(const char *src);
	bool verifyLine(const HexRecord& hexRecord);

	void hexStringToDec();
	unsigned int hexToDec(const string& str);

	bool formatParse(const char *src, const int lineNo);
	bool hexFormatParse(const char *src, char *dst);

	void hexStringToByte(const char* src, const int srcLen, BYTE* dst);
	void byteToHexString(BYTE* source, char* dest, int sourceLen);
	void splitHexData(const string& inData, vector<BYTE>& outData);

	void byteToBin(BYTE *pByte, char* pBin);

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

	void parse(const CString& hexFileName);

		// output
	void writeToBinFile();

	string _hexEditField;
	string _binEditField;
	string getHexEditFieldText();
	string getBinEditFieldText();

};

