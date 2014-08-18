#include "StdAfx.h"
#include "ComController.h"


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


bool ComController::writeMemory(BYTE* datas, int dataSize, long startAddress){
/* 
if the write operation was successful, the bootloader transmits the ACK byte; 
otherwise it transmits a NACK byte to the user and aborts the command

The maximum length of the block to be written for the STM32F10xxx is 256 bytes.

When writting to the RAM, care must be taken to avoid overlapping with the first 512 bytes 
(0x200) in RAM because they are used by the bootloader firmware.
*/
    BYTE *revData = new BYTE[1];  //TODO: _revData ?
    int saveCount = 0;
    
    _hCom.Write(WRITE_MEMORY, 2);
	_hCom.Read(revData, 1);

    if (revData[0] == NACK) {
        return false; 
    }

    /* Send the start address (4 bytes) & checksum
       Byte 3 to byte 6:start address
         byte 3: MSB
         byte 6: LSB
       Byte 7: Checksum: XOR (Byte3, Byte4, Byte5, Byte6) 

      |  3  |  4  |  5  |  6  |  7  |
      | MSB |     |     | LSB | XOR |

       Wait for ACK
    */
    //BYTE overlappedAddress = 0x0200; //TODO: ??
    // 00-00 -> FF-FF

    BYTE checkSum;
    checkSum = (BYTE)(startAddress>>8) + (BYTE)startAddress;
    checkSum = ~checkSum + 1;

    const int sendAddressSize = 5;
    BYTE* sendAddress = new BYTE[sendAddressSize];
    memset(sendAddress, 0x00, sendAddressSize);
    sendAddress[2] = (BYTE)(startAddress>>8);
    sendAddress[3] = (BYTE)startAddress;
    sendAddress[4] = checkSum;

    _hCom.Write(sendAddress, sendAddressSize);
    _hCom.Read(revData, 1);

    if (revData[0] == NACK) {
        return false; 
    }

    /* Send the number of bytes to be written
        (1 byte), the data (N + 1) bytes) & checksum

    */


    return true;
}