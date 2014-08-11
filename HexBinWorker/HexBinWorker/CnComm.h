#ifndef _CN_COMM_H_
#define _CN_COMM_H_

#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)

#include <assert.h>
#include <stdio.h>
#include <windows.h>

//送到窗口的消息 WPARAM 端口号
#define ON_COM_RECEIVE WM_USER + 618
#define ON_COM_CTS WM_USER + 619 //LPARAM 1 valid
#define ON_COM_DSR WM_USER + 621 //LPARAM 1 valid
#define ON_COM_RING WM_USER + 623
#define ON_COM_RLSD WM_USER + 624
#define ON_COM_BREAK WM_USER + 625
#define ON_COM_TXEMPTY WM_USER + 626
#define ON_COM_ERROR WM_USER + 627 //LPARAM save Error ID
#define DEFAULT_COM_MASK_EVENT EV_RXCHAR | EV_ERR | EV_CTS | EV_DSR | EV_BREAK | EV_TXEMPTY | EV_RING | EV_RLSD

class CnComm
{
public:
	
	//------------------------------Construction-----------------------------------
	//第1个参数为是否在打开串口时启动监视线程， 第2个参数为IO方式 阻塞方式（0）/ 异步重叠方式（默认）
	CnComm(bool fAutoBeginThread = true, DWORD dwIOMode =
		FILE_FLAG_OVERLAPPED): _dwIOMode(dwIOMode), _fAutoBeginThread(fAutoBeginThread)
	{
		Init();
	}
	
	virtual ~CnComm()
	{
		Close();
		UnInit();
	}
	
	//----------------------------------Attributes----------------------------------
	//判断串口是否打开
	inline bool IsOpen()
	{
		return _hCommHandle != INVALID_HANDLE_VALUE;
	}
	//判断串口是否打开
	operator bool()
	{
		return _hCommHandle != INVALID_HANDLE_VALUE;
	}
	//获得串口句炳
	inline HANDLE GetHandle()
	{
		return _hCommHandle;
	}
	//获得串口句炳
	operator HANDLE()
	{
		return _hCommHandle;
	}
	//获得串口参数 DCB
	DCB *GetState()
	{
		return IsOpen() && ::GetCommState(_hCommHandle, &_DCB) == TRUE ?
			&_DCB: NULL;
	}
	//设置串口参数 DCB
	bool SetState(DCB *pdcb = NULL)
	{
		return IsOpen() ? ::SetCommState(_hCommHandle, pdcb == NULL ? &_DCB:pdcb) == TRUE: false;
	}


	//设置串口参数：波特率，停止位，等 支持设置字符串 "9600, 8, n, 1"
	bool SetState(char *szSetStr)
	{
		CString szSetCStr = CString(szSetStr);

		if (IsOpen())
		{
			if (::GetCommState(_hCommHandle, &_DCB) != TRUE)
				return false;
			if (::BuildCommDCB(szSetCStr, &_DCB) != TRUE)
				return false;
			return ::SetCommState(_hCommHandle, &_DCB) == TRUE;
		}
		return false;
	}


	//设置串口参数：波特率，停止位，等
	bool SetState(DWORD dwBaudRate, DWORD dwByteSize = 8, DWORD dwParity =
		NOPARITY, DWORD dwStopBits = ONESTOPBIT)
	{
		if (IsOpen())
		{
			if (::GetCommState(_hCommHandle, &_DCB) != TRUE)
				return false;
			_DCB.BaudRate = dwBaudRate;
			_DCB.ByteSize = (unsigned char)dwByteSize;
			_DCB.Parity = (unsigned char)dwParity;
			_DCB.StopBits = (unsigned char)dwStopBits;
			return ::SetCommState(_hCommHandle, &_DCB) == TRUE;
		}
		return false;
	}


	//获得超时结构
	LPCOMMTIMEOUTS GetTimeouts(void)
	{
		return IsOpen() && ::GetCommTimeouts(_hCommHandle, &_CO) == TRUE ?
			&_CO: NULL;
	}
	//设置超时
	bool SetTimeouts(LPCOMMTIMEOUTS lpCO)
	{
		return IsOpen() ? ::SetCommTimeouts(_hCommHandle, lpCO) == TRUE:false;
	}
	//设置串口的I/O缓冲区大小
	bool SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize)
	{
		return IsOpen() ? ::SetupComm(_hCommHandle, dwInputSize, dwOutputSize)== TRUE: false;
	}
	//关联消息的窗口句柄
	inline void SetWnd(HWND hWnd)
	{
		assert(::IsWindow(hWnd));
		_hNotifyWnd = hWnd;
	}
	//设定发送通知, 接受字符最小值
	inline void SetNotifyNum(DWORD dwNum)
	{
		_dwNotifyNum = dwNum;
	}
	//线程是否运行
	inline bool IsThreadRunning()
	{
		return _hThreadHandle != NULL;
	}
	//获得线程句柄
	inline HANDLE GetThread()
	{
		return _hThreadHandle;
	}
	//设置要监视的事件， 打开前设置有效
	void SetMaskEvent(DWORD dwEvent = DEFAULT_COM_MASK_EVENT)
	{
		_dwMaskEvent = dwEvent;
	}
	//获得读缓冲区的字符数
	int GetInputSize()
	{
		COMSTAT Stat;
		DWORD dwError;
		
		return ::ClearCommError(_hCommHandle, &dwError, &Stat) == TRUE ? Stat.cbInQue : (DWORD) - 1L;
	}
	
	//----------------------------------Operations----------------------------------
	//打开串口 缺省 9600, 8, n, 1
	bool Open(DWORD dwPort)
	{
		return Open(dwPort, 9600);
	}
	//打开串口 缺省 baud_rate, 8, n, 1
	bool Open(DWORD dwPort, DWORD dwBaudRate)
	{
		if (dwPort < 1 || dwPort > 1024)
			return false;
		BindCommPort(dwPort);
		
		if (!OpenCommPort())
			return false;
		
		if (!SetupPort())
			return false;
		
		return SetState(dwBaudRate);
	}
	bool Open(DWORD dwPort, DWORD dwBaudRate, DWORD dwParity)
	{
		if (dwPort < 1 || dwPort > 1024)
			return false;
		BindCommPort(dwPort);
		
		if (!OpenCommPort())
			return false;
		
		if (!SetupPort())
			return false;
		
		return SetState(dwBaudRate,8,dwParity);
	}
	//打开串口, 使用类似"9600, 8, n, 1"的设置字符串设置串口
	bool Open(DWORD dwPort, char *szSetStr)
	{
		if (dwPort < 1 || dwPort > 1024)
			return false;
		
		BindCommPort(dwPort);
		
		if (!OpenCommPort())
			return false;
		
		if (!SetupPort())
			return false;
		
		return SetState(szSetStr);
	}
	
	
	//读取串口 dwBufferLength个字符到 Buffer 返回实际读到的字符数 可读任意数据
	DWORD Read(LPVOID Buffer, DWORD dwBufferLength, DWORD dwWaitTime = 10)
	{
		if (!IsOpen())
			return 0;
		
		COMSTAT Stat;
		DWORD dwError;
		
		if (::ClearCommError(_hCommHandle, &dwError, &Stat) && dwError > 0)
		{
			::PurgeComm(_hCommHandle,PURGE_RXABORT | PURGE_RXCLEAR);
			return 0;
		}  
		if (!Stat.cbInQue)
			// 缓冲区无数据
			return 0;
		
		unsigned long uReadLength = 0;
		
		dwBufferLength = dwBufferLength > Stat.cbInQue ? Stat.cbInQue :dwBufferLength;
		
		if (!::ReadFile(_hCommHandle, Buffer, dwBufferLength, &uReadLength, &_ReadOverlapped))
		{
			if (::GetLastError() == ERROR_IO_PENDING)
			{
				WaitForSingleObject(_ReadOverlapped.hEvent, dwWaitTime);
				// 结束异步I/O
				if (!::GetOverlappedResult(_hCommHandle, &_ReadOverlapped,&uReadLength, false))
				{
					if (::GetLastError() != ERROR_IO_INCOMPLETE)
						uReadLength = 0;
				}
			}
			else
				uReadLength = 0;
		}
		
		return uReadLength;
	}
	//读取串口 dwBufferLength - 1 个字符到 szBuffer 返回ANSI C 模式字符串指针 适合一般字符通讯
	char *ReadString(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime =20)
	{
		unsigned long uReadLength = Read(szBuffer, dwBufferLength - 1,dwWaitTime);
		szBuffer[uReadLength] = '\0';
		return szBuffer;
	}


	//写串口 可写任意数据 "abcd" or "\x0\x1\x2"
	DWORD Write(LPVOID Buffer, DWORD dwBufferLength)
	{
		if (!IsOpen())
			return 0;
		
		DWORD dwError;
		
		if (::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)
			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);
		
		unsigned long uWriteLength = 0;
		
		if (!::WriteFile(_hCommHandle, Buffer, dwBufferLength, &uWriteLength,&_WriteOverlapped))
			if (::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;
			
			return uWriteLength;
    }
    //写串口 写ANSI C 模式字符串指针 
    DWORD Write(const char *szBuffer)
    {
		assert(szBuffer);
		
		return Write((void*)szBuffer, strlen(szBuffer));
    }


    //读串口 同步应用
    DWORD ReadSync(LPVOID Buffer, DWORD dwBufferLength)
    {
		if (!IsOpen())
			return 0;
		
		DWORD dwError;
		if (::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)
		{
			::PurgeComm(_hCommHandle,PURGE_RXABORT | PURGE_RXCLEAR);
			return 0;
		}
		
		DWORD uReadLength = 0;
		::ReadFile(_hCommHandle, Buffer, dwBufferLength, &uReadLength, NULL);
		return uReadLength;
    }
    //写串口 同步应用
    DWORD WriteSync(LPVOID Buffer, DWORD dwBufferLength)
    {
		if (!IsOpen())
			return 0;
		
		DWORD dwError;
		if (::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)
			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);
		
		unsigned long uWriteLength = 0;
		::WriteFile(_hCommHandle, Buffer, dwBufferLength, &uWriteLength, NULL);
		return uWriteLength;
    }
    //写串口 szBuffer 可以输出格式字符串 包含缓冲区长度
    DWORD Write(char *szBuffer, DWORD dwBufferLength, char *szFormat, ...)
    {
		if (!IsOpen())
			return 0;
		
		va_list va;
		va_start(va, szFormat);
		_vsnprintf(szBuffer, dwBufferLength, szFormat, va);
		va_end(va);
		
		return Write(szBuffer);
    }
    //写串口 szBuffer 可以输出格式字符串 不检查缓冲区长度 小心溢出
    DWORD Write(char *szBuffer, char *szFormat, ...)
    {
		if (!IsOpen())
			return 0;
		
		va_list va;
		va_start(va, szFormat);
		vsprintf(szBuffer, szFormat, va);
		va_end(va);
		
		return Write(szBuffer);
    }
    //关闭串口 同时也关闭关联线程
    virtual void Close()
    {
		if (IsOpen())
		{
			PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);
			
			EndThread();
			::CloseHandle(_hCommHandle);
			
			_hCommHandle = INVALID_HANDLE_VALUE;
		}
    }
    //DTR 电平控制
    bool SetDTR(bool OnOrOff)
    {
		return IsOpen() ? EscapeCommFunction(_hCommHandle, OnOrOff ? SETDTR :CLRDTR): false;
    }
    //RTS 电平控制
    bool SetRTS(bool OnOrOff)
    {
		return IsOpen() ? EscapeCommFunction(_hCommHandle, OnOrOff ? SETRTS :CLRRTS): false;
    }
    //
    bool SetBreak(bool OnOrOff)
    {
		return IsOpen() ? EscapeCommFunction(_hCommHandle, OnOrOff ? SETBREAK: CLRBREAK): false;
    }
    //辅助线程控制 建监视线程
    bool BeginThread()
    {
		if (!IsThreadRunning())
		{
			_fRunFlag = true;
			_hThreadHandle = NULL;
			
			DWORD id;
			
			_hThreadHandle = ::CreateThread(NULL, 0, CommThreadProc, this, 0,&id);
			
			return (_hThreadHandle != NULL);
		}
		return false;
    }
    //暂停监视线程
    inline bool SuspendThread()
    {
		return IsThreadRunning() ? ::SuspendThread(_hThreadHandle) !=0xFFFFFFFF: false;
    }
    //恢复监视线程
    inline bool ResumeThread()
    {
		return IsThreadRunning() ? ::ResumeThread(_hThreadHandle) !=0xFFFFFFFF: false;
    }
    //终止线程
    bool EndThread(DWORD dwWaitTime = 100)
    {
		if (IsThreadRunning())
		{
			_fRunFlag = false;
			::SetCommMask(_hCommHandle, 0);
			::SetEvent(_WaitOverlapped.hEvent);
			if (::WaitForSingleObject(_hThreadHandle, dwWaitTime) !=WAIT_OBJECT_0)
				if (!::TerminateThread(_hThreadHandle, 0))
					return false;
				
				::CloseHandle(_hThreadHandle);
				::ResetEvent(_WaitOverlapped.hEvent);
				
				_hThreadHandle = NULL;
				
				return true;
		}
		return false;
    }
	//获取端口号
	int GetComNum()
	{
		return _dwPort;
	}
	//绑定串口
	void BindCommPort(DWORD dwPort)
	{
		assert(dwPort >= 1 && dwPort <= 1024);
		
		char p[5];
		
		_dwPort = dwPort;
		strcpy(_szCommStr, "\\\\.\\COM");
		ltoa(_dwPort, p, 10);
		strcat(_szCommStr, p);
	}



  protected:
	  volatile DWORD _dwPort; //串口号
	  volatile HANDLE _hCommHandle; //串口句柄
	  char _szCommStr[20]; //保存COM1类似的字符串
	  
	  DCB _DCB; //波特率，停止位，等
	  COMMTIMEOUTS _CO; //超时结构
	  
	  DWORD _dwIOMode; // 0 同步 默认 FILE_FLAG_OVERLAPPED重叠I/O异步
	  OVERLAPPED _ReadOverlapped, _WriteOverlapped; // 重叠I/O
	  
	  volatile HANDLE _hThreadHandle; //辅助线程
	  volatile HWND _hNotifyWnd; // 通知窗口
	  volatile DWORD _dwNotifyNum; //接受多少字节(>=_dwNotifyNum)发送通知消息
	  volatile DWORD _dwMaskEvent; //监视的事件
	  volatile bool _fRunFlag; //线程运行循环标志
	  bool _fAutoBeginThread; //Open() 自动 BeginThread();
	  OVERLAPPED _WaitOverlapped; //WaitCommEvent use
	  
	  //初始化
	  void Init()
	  {
		  memset(_szCommStr, 0, 20);
		  memset(&_DCB, 0, sizeof(_DCB));
		  _DCB.DCBlength = sizeof(_DCB);
		  _hCommHandle = INVALID_HANDLE_VALUE;
		  
		  memset(&_ReadOverlapped, 0, sizeof(_ReadOverlapped));
		  memset(&_WriteOverlapped, 0, sizeof(_WriteOverlapped));
		  
		  _ReadOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
		  assert(_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE);
		  
		  _WriteOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
		  assert(_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE);
		  
		  _hNotifyWnd = NULL;
		  _dwNotifyNum = 0;
		  _dwMaskEvent = DEFAULT_COM_MASK_EVENT;
		  _hThreadHandle = NULL;
		  
		  memset(&_WaitOverlapped, 0, sizeof(_WaitOverlapped));
		  _WaitOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
		  assert(_WaitOverlapped.hEvent != INVALID_HANDLE_VALUE);
	  }
	  //析构
	  void UnInit()
	  {
		  if (_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE)
			  CloseHandle(_ReadOverlapped.hEvent);
		  
		  if (_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE)
			  CloseHandle(_WriteOverlapped.hEvent);
		  
		  if (_WaitOverlapped.hEvent != INVALID_HANDLE_VALUE)
			  CloseHandle(_WaitOverlapped.hEvent);
	  }
	  //打开串口
	  virtual bool OpenCommPort()
	  {
		  if (IsOpen()) {
			  Close();
		  }

		  wchar_t wszComm[20] = { '\0' };
		  mbstowcs(wszComm, _szCommStr, 20);
		  
		  _hCommHandle = ::CreateFile(wszComm, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | _dwIOMode, NULL);

		  if (_fAutoBeginThread)
		  {
			  if (IsOpen() && BeginThread())
				  return true;
			  else
			  {
				  Close(); //创建线程失败
				  return false;
			  }
		  }
		  return IsOpen();
	  }
	  //设置串口
	  virtual bool SetupPort()
	  {
		  if (!IsOpen())
			  return false;
		  
		  if (!::SetupComm(_hCommHandle, 4096, 4096))
			  return false;
		  
		  if (!::GetCommTimeouts(_hCommHandle, &_CO))
			  return false;
		  _CO.ReadIntervalTimeout = 0;
		  _CO.ReadTotalTimeoutMultiplier = 1;
		  _CO.ReadTotalTimeoutConstant = 1000;
		  _CO.WriteTotalTimeoutMultiplier = 1;
		  _CO.WriteTotalTimeoutConstant = 1000;
		  if (!::SetCommTimeouts(_hCommHandle, &_CO))
			  return false;
		  
		  if (!::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR | PURGE_RXCLEAR))
			  return false;
		  return true;
	  }
	  
	  //---------------------------------------threads callback-----------------------------------
	  //线程收到消息自动调用, 如窗口句柄有效, 送出消息, 包含串口编号， 均为虚函数可以在基层类中扩展
	  virtual void OnReceive() //EV_RXCHAR
	  {
		  if (::IsWindow(_hNotifyWnd))
			  ::PostMessage(_hNotifyWnd, ON_COM_RECEIVE, WPARAM(_dwPort), LPARAM (0));
	  }
	  
	  virtual void OnDSR()
	  {
		  if (::IsWindow(_hNotifyWnd))
		  {
			  DWORD Status;
			  if (GetCommModemStatus(_hCommHandle, &Status))
				  ::PostMessage(_hNotifyWnd, ON_COM_DSR, WPARAM(_dwPort),LPARAM((Status &MS_DSR_ON) ? 1 : 0));
		  }
	  }
	  
	  virtual void OnCTS()
	  {
		  if (::IsWindow(_hNotifyWnd))
		  {
			  DWORD Status;
			  if (GetCommModemStatus(_hCommHandle, &Status))
				  ::PostMessage(_hNotifyWnd, ON_COM_CTS, WPARAM(_dwPort), LPARAM( (Status &MS_CTS_ON) ? 1 : 0));
		  }
	  }
	  
	  virtual void OnBreak()
	  {
		  if (::IsWindow(_hNotifyWnd))
		  {
			  ::PostMessage(_hNotifyWnd, ON_COM_BREAK, WPARAM(_dwPort), LPARAM(0));
		  }
	  }
	  
	  virtual void OnTXEmpty()
	  {
		  if (::IsWindow(_hNotifyWnd))
			  ::PostMessage(_hNotifyWnd, ON_COM_TXEMPTY, WPARAM(_dwPort), LPARAM (0));
	  }
	  
	  virtual void OnError()
	  {
		  DWORD dwError;
		  ::ClearCommError(_hCommHandle, &dwError, NULL);
		  if (::IsWindow(_hNotifyWnd))
			  ::PostMessage(_hNotifyWnd, ON_COM_ERROR, WPARAM(_dwPort), LPARAM (dwError));
	  }
	  
	  virtual void OnRing()
	  {
		  if (::IsWindow(_hNotifyWnd))
			  ::PostMessage(_hNotifyWnd, ON_COM_RING, WPARAM(_dwPort), LPARAM(0));
	  }
	  
	  virtual void OnRLSD()
	  {
		  if (::IsWindow(_hNotifyWnd))
			  ::PostMessage(_hNotifyWnd, ON_COM_RLSD, WPARAM(_dwPort), LPARAM(0));
	  }
	  
	  virtual DWORD ThreadFunc()
	  {
		  if (!::SetCommMask(_hCommHandle, _dwMaskEvent))
		  {
			  char szBuffer[256];
			  _snprintf(szBuffer, 255,
				  "%s(%d) : COM%d Call WINAPI SetCommMask(%x, %x) Fail, thread work invalid! GetLastError() = %d;", __FILE__, __LINE__, _dwPort, _hCommHandle, _dwMaskEvent, GetLastError());
			  
			  CString messageCStr;
			  messageCStr.Format(_T("%s"), szBuffer);
			  MessageBox(NULL, messageCStr, _T("Class cnComm"), MB_OK);
			  return 1;
		  }
		  
		  COMSTAT Stat;
		  DWORD dwError;
		  
		  for (DWORD dwLength, dwMask = 0; _fRunFlag && IsOpen(); dwMask = 0)
		  {
			  if (!::WaitCommEvent(_hCommHandle, &dwMask, &_WaitOverlapped))
			  {
				  if (::GetLastError() == ERROR_IO_PENDING)
					  // asynchronous
					  ::GetOverlappedResult(_hCommHandle, &_WaitOverlapped,&dwLength, TRUE);
				  else
					  continue;
			  }
			  
			  if (dwMask == 0)
				  continue;
			  
			  switch (dwMask)
			  {
			  case EV_RXCHAR:
				  ::ClearCommError(_hCommHandle, &dwError, &Stat);
				  if (Stat.cbInQue >= _dwNotifyNum)
					  OnReceive();
				  break;
				  
			  case EV_TXEMPTY:
				  OnTXEmpty();
				  break;
				  
			  case EV_CTS:
				  OnCTS();
				  break;
				  
			  case EV_DSR:
				  OnDSR();
				  break;
				  
			  case EV_RING:
				  OnRing();
				  break;
				  
			  case EV_RLSD:
				  OnRLSD();
				  break;
				  
			  case EV_BREAK:
				  OnBreak();
				  break;
				  
			  case EV_ERR:
				  OnError();
				  break;
			  } //case
		  } //for
		  return 0;
	  }
	  
  private:
	  //the function protected
	  
	  CnComm(const CnComm &);
	  CnComm &operator = (const CnComm &);
	  
	  //base function for thread
	  static DWORD WINAPI CommThreadProc(LPVOID lpPara)
	  {
		  return ((CnComm*)lpPara)->ThreadFunc();
	  }
  };
#endif //_CN_COMM_H_