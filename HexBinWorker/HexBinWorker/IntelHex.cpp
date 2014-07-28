#include "StdAfx.h"
#include "IntelHex.h"
#include <string.h>


using namespace std;

const int FLASHVOLUME = 16; //Flash的容量，以K为单位 

bool IntelHex::open() {

	pHexFile = fopen((LPSTR)(LPCTSTR)fileName, "r");  // TODO: ?
	if(pHexFile == NULL) {  
        printf("Open file error.\n");  
        return false;  
    }  
	return true;
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
	
	while (fscanf(pHexFile, "%s", lineBuffer) != EOF) {
		printf("%s\n", lineBuffer);

	}

	delete(lineBuffer);
	delete(parseBuffer);
	delete(flashBuffer);

}