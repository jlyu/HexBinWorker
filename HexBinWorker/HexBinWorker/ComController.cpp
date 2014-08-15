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
        _hCom.SetState(115200, 8, EVENPARITY, ONESTOPBIT);
        return true;
    } else {
		CString errMessage;
        errMessage.Format(_T("无法打开串口：COM%d"), _comNumber);
		TRACE(errMessage);
		return false;
	}
}

bool ComController::getCommand() {

    BYTE *revData = new BYTE[15];  //TODO: _revData ?
    int saveCount = 0;

    bool getCommandOK = false;
    while (true)
	{
	    _hCom.Write(GET_COMMAND, 2);
	    _hCom.Read(revData, 15);
		
        if (revData[0] == ACK) { // get!
            // TODO: ..and check ?
            getCommandOK = true;
            break;
        }

        if (saveCount == 100) {
            getCommandOK = false;
            break;
        }

        saveCount++;
    }


    if (getCommandOK) {
        CString bufferCStr, bufferBlock;
	    for (int i=0; i<15; i++) {
		    bufferCStr.Format(_T("%02X"), revData[i]);
		    bufferBlock += bufferCStr;
	    }
    }
		
	delete [] revData;
    return getCommandOK;
}

bool ComController::eraseMemory() {
    BYTE *revData = new BYTE[1];  //TODO: _revData ?
    int saveCount = 0;

    bool eraseMemoryOK = false;
    while (true) {
        _hCom.Write(ERASE_MEMORY, 2);
	    _hCom.Read(revData, 1);
    
        if (revData[0] == ACK) { // get!
            _hCom.Write(GLOBAL_ERASE, 2);
            _hCom.Read(revData, 1);

            if (revData[0] == ACK) {
                eraseMemoryOK = true;
            } else {
                eraseMemoryOK = false;
            }

            break;
        }

        if (saveCount == 100) {
           eraseMemoryOK = false;
           break;
        }

        saveCount++;

    }

    delete [] revData;
    return eraseMemoryOK;
}
