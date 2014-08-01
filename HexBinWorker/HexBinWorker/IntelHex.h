#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>
using namespace std;

const int FLASHVOLUME = 64; //Flash的容量，以K为单位

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
		BYTE datas[FLASHVOLUME*1024]; // 分配64K 初始填充 FF
		
		int validLength;
		int maxAddress;  // 10进制

		HexBlock() {
			memset(datas, 0x00, FLASHVOLUME*1024);
			validLength = 0;
		}
	};

	//vector<BYTE> splitedData;
	void hexStringToByte(const char* src, const int srcLen, BYTE* dst);
	void IntelHex::byteToHexString(BYTE* source, char* dest, int sourceLen);

	void splitHexData(const string& inData, vector<BYTE>& outData);

	list<HexBlock> hexBlocks; 

	struct HexLine {
		//int lineNo;
		HexRecord hexRecord;
		//DecRecord decRecord;
	};


	CString fileName;
	FILE *pHexFile;
	vector<HexLine> fileContent;



	bool openHexFile();
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



	string _hexEditField;
	string _binEditField;
	string getHexEditFieldText();
	string getBinEditFieldText();

};

