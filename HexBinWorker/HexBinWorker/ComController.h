#pragma once
#include "CnComm.h"
#include "GlobalDefine.h"
#include <iostream>
using namespace std;


class ComController {
public:
	ComController(void);
	virtual ~ComController(void);

public:
    bool openCom(int comNumber);
    bool getCommand();
    bool eraseMemory();
    bool writeMemory(BYTE* datas, int dataSize, long startAddress);
    bool readMemory();

private:
	CnComm _hCom;
    int _comNumber;
};

