#include "StdAfx.h"
#include "IntelHex.h"
#include <string.h>
#include <regex> 

using namespace std;

const int FLASHVOLUME = 16; //Flash的容量，以K为单位 

bool IntelHex::open() {

	CT2A asciiFileName(fileName);
	pHexFile = fopen(asciiFileName, "r");

	if(pHexFile == NULL) {  
        printf("Open file error.\n");  
        return false;  
    }  
	return true;
}

bool IntelHex::checkLine(const char *src) {
	
	string checkPattern("[:0-9A-F\\r\\n]*");
	string lineString = src;

	std::regex_constants::syntax_option_type optionType = std::regex_constants::icase;
	std::regex regExpress(checkPattern, optionType);
	//std::smatch matchResult;

	if(std::regex_match(lineString, regExpress))  
    {  
        return matchLine(src);  
    }  
    else  
    {  
        return false;  
    }  
}


bool IntelHex::matchLine(const char *src) {
	string matchPattern("^:(\\w{2})(\\w{4})(\\w{2})(\\w*)(\\w{2})$");
	string lineString = src;
	std::regex_constants::syntax_option_type optionType = std::regex_constants::icase;
	std::regex regExpress(matchPattern, optionType);
	std::smatch matchResult;

	if(std::regex_match(lineString, matchResult, regExpress))  
    {  
		// http://blog.csdn.net/zhulinu/article/details/17148511

        return true;  
    }  
    else  
    {  
        return false;  
    }  

}

bool IntelHex::formatParse(const char *src, const int lineNo) {
	//解析前格式   :llaaaattddcc               :020000000828CE
	//解析后格式   : ll aaaa tt [dd] cc        : 02 0000 00 [0828] CE
	//按域分别存入 

	const int srcLength = strlen(src);
	int srcIndex = 0, dstIndex = 0; //索引，p为src的索引，q为dst索引  
    //int index = 0;//循环标志 

	HexRecord hexRecord;

	while (srcIndex < srcLength) {  

		if (srcIndex == 0) {
			if (src[srcIndex] != ':') {
				printf("Wrong Format.\n"); // TODO:
				break;
			}
			srcIndex++;
		}

		if(srcIndex == 1) {   //提取两位的"ll"
            for(int index = 0; index < 2; index++) { 	
				hexRecord.dataLength += src[srcIndex++];
            }  
        }

		if(srcIndex == 3) {  //提取四位的"aaaa"
            for(int index = 0; index < 4; index++) {  
				hexRecord.startAddress += src[srcIndex++];
            }
        }

		if(srcIndex == 7) {  //提取两位的"tt"
            for(int index = 0; index < 2; index++) {  
				hexRecord.recordType += src[srcIndex++];
            }  
        }

		if(srcIndex == 9) { //提取2N位的"[dd]"
			const int dataLen = srcLength - 2 - srcIndex;
			for(int index = 0; index < dataLen; index++) { 
				hexRecord.data += src[srcIndex++];
			}
		}

		if (srcIndex == srcLength-2) { //提取两位的"cc"
			for(int index = 0; index < 2; index++) {  
				hexRecord.sumCheck += src[srcIndex++];
            }  
		}

	}
	return true;
}

bool hexFormatParse(const char *src, char *dst) {
	//解析前格式   :llaaaattddcc               :020000000828CE
	//解析后格式   : ll aaaa tt [dd] cc        : 02 0000 00 [0828] CE
	int srcLength = strlen(src);
	int srcIndex = 0, dstIndex = 0; //索引，p为src的索引，q为dst索引  
    int index = 0;//循环标志 

	while (srcIndex <= srcLength) { //循环，对src按格式解析 
		if (srcIndex == 0) {
			if (src[srcIndex] != ':') {
				printf("Wrong Format.\n");
				break;
			} else {  
                dst[dstIndex++] = ':'; 
				dst[dstIndex++] = ' ';  
                srcIndex++;
				continue;  
            }  
		}

		if(srcIndex == 1) {  
            for(index = 0; index < 2; index++) {  //提取两位的"ll"  
                dst[dstIndex++] = src[srcIndex++];  
            }  
            dst[dstIndex++] = ' ';
			continue;  
        }  

	}

	return false;
}


void IntelHex::parse() {
	const int  flashSize = 2 * FLASHVOLUME * 1024 * sizeof(char); //flash的存储单元个数  
    char *lineBuffer = new char[sizeof(char) * 100];        //存储hex文件的一行内容  
    char *parseBuffer = new char[sizeof(char) * 200];       //存储hex文件解析后的内容  
    char *flashBuffer = new char[flashSize];                //存储Flash中的内容 

    if (lineBuffer == NULL|| parseBuffer == NULL || flashBuffer == NULL) {  
		printf("Apply for memory failed.!\n");  
        return;  
    }  

	memset(flashBuffer, 'F', flashSize); //将Flash初始时全部清1

	if (!open()) return;
	
	int lineNo = 1;
	while (fscanf(pHexFile, "%s", lineBuffer) != EOF) {
		printf("%s\n", lineBuffer);
		checkLine(lineBuffer);
		formatParse(lineBuffer, lineNo);
		lineNo++;
	}

	delete(lineBuffer);
	delete(parseBuffer);
	delete(flashBuffer);

}