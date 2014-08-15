#pragma once
#include "CnComm.h"

class ComController {
public:
	ComController(void);
	virtual ~ComController(void);

public:
    bool openCom(int comNumber);
    bool getCommand();
    bool eraseMemory();




private:
	CnComm _hCom;
    int _comNumber;
};

