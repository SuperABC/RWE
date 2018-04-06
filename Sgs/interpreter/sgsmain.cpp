#include <fstream>
#include "Sgs/interpreter/sgsmach.h"
#include "Sgs/interpreter/sgsdebug.h"
#include "Sgs/interpreter/sgsmem.h"
#include "Text/editor/editor.h"

void sgsInterprete(Editor *editor) {
	SgsLex l = SgsLex();
	SgsSyntax s = SgsSyntax();
	SgsMachine m = SgsMachine();

	string input;

	editor->save();
	string fn = editor->actFile()->getPath();
	std::ifstream fin(fn);

	string tmp;
	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp + '\n';
	}
	try {
		l.input(input.data())->parse();
		s.input(l.strId, l.output)->parse();
		m.input(s.output, s.globeClassType, s.globeFunc, s.globeVar)->execute();
	}
	catch (SGSLexemeException *le) {
		cout << l.get() << endl;
		cout << le->message();
		delete le;
	}
	catch (SGSyntaxException *se) {
		cout << l.get() << endl;
		cout << se->message();
		delete se;
	}
	catch (MachineException *me) {
		cout << l.get() << endl;
		cout << me->message();
		delete me;
	}

	s.clearMem();
	m.clearMem();
}

void sgsDebug() {

}