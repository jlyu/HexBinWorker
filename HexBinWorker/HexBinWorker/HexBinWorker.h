// HexBinWorker.h : PROJECT_NAME 应用程序的主头文件
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		


// CHexBinWorkerApp:
class CHexBinWorkerApp : public CWinApp
{
public:
	CHexBinWorkerApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CHexBinWorkerApp theApp;