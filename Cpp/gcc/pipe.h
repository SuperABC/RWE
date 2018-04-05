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
	HANDLE m_hChildInputWrite;  //�����ض����ӽ�������ľ��  
	HANDLE m_hChildInputRead;
	HANDLE m_hChildOutputWrite; //�����ض����ӽ�������ľ��    
	HANDLE m_hChildOutputRead;
	PROCESS_INFORMATION m_cmdPI;//cmd������Ϣ  
};

#endif
