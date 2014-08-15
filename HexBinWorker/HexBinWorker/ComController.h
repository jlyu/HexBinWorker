#pragma once
#include "CnComm.h"

class ComController {
public:
	ComController(void);
	virtual ~ComController(void);

private:
	CnComm _hCom;

};

