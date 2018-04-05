#ifndef RWE_CPP_GCC_PIPE
#define RWE_CPP_GCC_PIPE

#include "Frame/main/main.h"

class Pipe {
public:
	Pipe();
	~Pipe();

	bool RunProcess(const string &process);
	bool StopProcess(void);
	bool GetOutput(const string &endStr, int timeout, string &outstr);
	bool SetInput(const string &cmd);
private:
	HANDLE m_hChildInputWrite;  //用于重定向子进程输入的句柄  
	HANDLE m_hChildInputRead;
	HANDLE m_hChildOutputWrite; //用于重定向子进程输出的句柄    
	HANDLE m_hChildOutputRead;
	PROCESS_INFORMATION m_cmdPI;//cmd进程信息  
};

#endif
