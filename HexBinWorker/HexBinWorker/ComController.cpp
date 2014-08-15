#include "StdAfx.h"
#include "ComController.h"
#include "GlobalDefine.h"


ComController::ComController(void): 
				_hCom(false, 0) {
    _hCom.SetState(115200, 8, EVENPARITY, ONESTOPBIT);
}


ComController::~ComController(void) {
    _hCom.Close();
}

bool ComController::openCom(int comNumber){
    _comNumber = comNumber;
	bool openOK = _hCom.Open(_comNumber);
	if (openOK)	{
        return true;
    } else {
		CString errMessage;
        errMessage.Format(_T("无法打开串口：COM%d"), _comNumber);
		TRACE(errMessage);
		return false;
	}
}

bool ComController::getCommand() {

    _hCom.SetState(115200, 8, EVENPARITY, ONESTOPBIT); // 
    BYTE *revData = new BYTE[15];
    int saveCount = 0;

    while (true)
	{
	    _hCom.Write(GET_COMMAND, 2);
	    _hCom.Read(revData, 15);
		
        if (revData[0] == ACK) { // get!
            // TODO: ..and check ?
            break;
        }

        if (saveCount == 100) {
            return false;
        }

        saveCount++;
    }

    CString bufferCStr, bufferBlock;
	for (int i=0; i<15; i++) {
		bufferCStr.Format(_T("%02X"), revData[i]);
		bufferBlock += bufferCStr;
	}
		
	delete [] revData;
    return true;
}


