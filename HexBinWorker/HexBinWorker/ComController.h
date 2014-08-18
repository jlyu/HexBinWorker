#pragma once
#include "CnComm.h"
#include "GlobalDefine.h"
#include <iostream>
using namespace std;


class ComController {
public:
	ComController(void);
	virtual ~ComController(void);

    bool openCom(int comNumber);
    bool getCommand();
    bool eraseMemory();
    bool writeMemory(BYTE* datas, int dataSize, long startAddress = 0x0800);
    bool readMemory();

private:
    bool ComController::sendWriteMemoryHead();
    bool ComController::sendWriteMemoryAddr(long MSB, long LSB);
    bool ComController::sendWriteMemorydata(BYTE* datas, int dataSize, int currentIndex);

private:
	CnComm _hCom;
    int _comNumber;
};

