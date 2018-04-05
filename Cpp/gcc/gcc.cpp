#include "Cpp/gcc/gcc.h"

string Gcc::compile(string src) {
	int pos = src.rfind('.');
	string dst = string(src.begin(), src.begin() + pos) + ".exe";

	string output;
	pipe.RunProcess("cmd.exe");
	pipe.GetOutput("", 3000, output);
	pipe.SetInput(string("gcc ") + src + " -o " + dst + '\n');
	pipe.GetOutput("", 3000, output);
	pipe.StopProcess();
	cout << output;

	return dst;
}
void Gcc::run(string exe) {
	pipe.RunProcess(exe);

	string outstr;
	while (1) {
		pipe.GetOutput("", 100, outstr);
		if (outstr.empty())break;;
		cout << outstr << endl;
	}

	pipe.StopProcess();
	cout << "Process end." << endl;
}