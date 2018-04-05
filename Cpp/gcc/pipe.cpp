#include "Cpp/gcc/pipe.h"

Pipe::Pipe() {
	m_hChildInputWrite = NULL;
	m_hChildInputRead = NULL;
	m_hChildOutputWrite = NULL;
	m_hChildOutputRead = NULL;
	ZeroMemory(&m_cmdPI, sizeof(m_cmdPI));
}
Pipe::~Pipe() {
	StopProcess();
}

bool Pipe::RunProcess(const string &process) {
	SECURITY_ATTRIBUTES   sa;
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(sa);

	if (FALSE == ::CreatePipe(&m_hChildOutputRead, &m_hChildOutputWrite, &sa, 0)) {
		return false;
	}

	if (FALSE == CreatePipe(&m_hChildInputRead, &m_hChildInputWrite, &sa, 0)) {
		::CloseHandle(m_hChildOutputWrite);
		::CloseHandle(m_hChildOutputRead);
		::CloseHandle(m_hChildOutputWrite);
		::CloseHandle(m_hChildOutputRead);
		return false;
	}

	ZeroMemory(&m_cmdPI, sizeof(m_cmdPI));
	STARTUPINFO  si;
	GetStartupInfo(&si);

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = m_hChildInputRead;     //重定向子进程输入     
	si.hStdOutput = m_hChildOutputWrite;   //重定向子进程输入      
	si.hStdError = m_hChildOutputWrite;

	if (FALSE == ::CreateProcess(NULL, (LPSTR)(process.c_str()),
		NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &m_cmdPI)) {
		::CloseHandle(m_hChildInputWrite);
		::CloseHandle(m_hChildInputRead);
		::CloseHandle(m_hChildOutputWrite);
		::CloseHandle(m_hChildOutputRead);
		m_hChildInputWrite = NULL;
		m_hChildInputRead = NULL;
		m_hChildOutputWrite = NULL;
		m_hChildOutputRead = NULL;
		ZeroMemory(&m_cmdPI, sizeof(m_cmdPI));
		return false;
	}

	return true;
}
bool Pipe::StopProcess(void) {
	::CloseHandle(m_hChildInputWrite);
	::CloseHandle(m_hChildInputRead);
	::CloseHandle(m_hChildOutputWrite);
	::CloseHandle(m_hChildOutputRead);
	m_hChildInputWrite = NULL;
	m_hChildInputRead = NULL;
	m_hChildOutputWrite = NULL;
	m_hChildOutputRead = NULL;
	::TerminateProcess(m_cmdPI.hProcess, -1);
	::CloseHandle(m_cmdPI.hProcess);
	::CloseHandle(m_cmdPI.hThread);
	ZeroMemory(&m_cmdPI, sizeof(m_cmdPI));

	return 0;
}
bool Pipe::GetOutput(const string &endStr, int timeout, string &outstr) {
	if (NULL == m_hChildOutputRead) {
		return false;
	}

	outstr = "";
	char buffer[4096] = { 0 };
	DWORD readBytes = 0;
	while (timeout > 0) {
		if (FALSE == PeekNamedPipe(m_hChildOutputRead, buffer, sizeof(buffer) - 1, &readBytes, 0, NULL)) {
			return false;
		}

		if (0 == readBytes) {
			::Sleep(200);
			timeout -= 200;
			continue;
		}

		readBytes = 0;
		if (::ReadFile(m_hChildOutputRead, buffer, sizeof(buffer) - 1, &readBytes, NULL)) {
			outstr.insert(outstr.end(), buffer, buffer + readBytes);
			if (outstr.rfind(endStr) == -1)continue;
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}
bool Pipe::SetInput(const string &cmd) {
	if (NULL == m_hChildInputWrite) {
		return "";
	}

	string tmp = cmd + "\r\n";
	DWORD writeBytes = 0;
	if (FALSE == ::WriteFile(m_hChildInputWrite, tmp.c_str(), tmp.size(), &writeBytes, NULL)) {
		return false;
	}
	return true;
}
