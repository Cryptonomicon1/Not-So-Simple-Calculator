// Includes
//------------------------------------------------------------------
#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#include<vector>
using std::vector;

#include"stdlib.h"
using std::exit;
using std::cerr;
using std::runtime_error;

#include<string>
using std::string;

// Global Constants
//------------------------------------------------------------------
static const char qt = 'q';
static const char prnt = ';';
static const char prmpt = '>';
static const char rslt = '=';

// Function Declarations
//------------------------------------------------------------------
void clcltVls();
double prcsXprsn();
double prcsTrm();
double prcsPrmry();
void psWndw(string);

// Class Definitions
//------------------------------------------------------------------
class Tkn {
public:
	Tkn();
	Tkn(char, double);
	char knd;
	float vl;
};
class TokenStream {
public:
	TokenStream();
	Tkn gt();
	void ptBck(Tkn);
private:
	bool fll;
	Tkn bffr;
};
// Instantiations
//------------------------------------------------------------------
TokenStream TknStrm;

// Main Program
//------------------------------------------------------------------
int main() {
	try {
		clcltVls();
	} catch(runtime_error& e) {
		cerr << e.what() << '\n';
		psWndw("~~");
	} catch(...) {
		cerr << "Exception\n";
		psWndw("~~");
	};
}
// Class Function Definitions
//------------------------------------------------------------------
Tkn::Tkn() {
};
Tkn::Tkn(char k, double v) {
	knd = k; vl = v;
};
TokenStream::TokenStream() {
	fll = false;
};
void TokenStream::ptBck(Tkn Tk) {
	if(fll) {
		cout << "putback into full buffer\n";
		exit(0);
	};
	bffr = Tk;
	fll = true;
};
Tkn TokenStream::gt() {
	if(fll) {
		fll = false;
		return bffr;
	};
	char chr;
	cin >> chr;

	switch(chr) {
	case prnt :
	case qt :
	case '(' :
	case ')' :
	case '+' :
	case '-' :
	case '*' :
	case '/' :
	case '%' :
		return Tkn {chr, 0};
	case '.' :
	case '0' :
	case '1' :
	case '2' :
	case '3' :
	case '4' :
	case '5' :
	case '6' :
	case '7' :
	case '8' :
	case '9' : {
		cin.putback(chr);
		double vl;
		cin >> vl;
		return Tkn {'#', vl};
	};
	default:
		cout << "Bad Token\n";
		psWndw("~~");
	};
};
// Function Definitions
//------------------------------------------------------------------

void clcltVls() {
	double vl = 0;
	Tkn Tk;
	while(cin) {
		cout << prmpt;
		Tk = TknStrm.gt();
		while(Tk.knd==prnt) Tk=TknStrm.gt(); // eat ';'s
		if(Tk.knd == qt) exit(0);
		TknStrm.ptBck(Tk);
		cout << rslt << prcsXprsn() << '\n';
	};
};
double prcsXprsn() {
	double lft = prcsTrm();
	Tkn Tk = TknStrm.gt();
	while(true) {
		switch(Tk.knd) {
		case '+' :
			lft += prcsTrm();
			Tk = TknStrm.gt();
			break;
		case '-' :
			lft -= prcsTrm();
			Tk = TknStrm.gt();
			break;
		default:
			TknStrm.ptBck(Tk);
			return lft;
		};
	};
};
double prcsTrm() {
	double lft = prcsPrmry();
	Tkn Tk = TknStrm.gt();
	while(true) {
		switch(Tk.knd) {
		case '*' :
			lft *= prcsPrmry();
			Tk = TknStrm.gt();
			break;
		case '/' :
			lft /= prcsPrmry();
			Tk = TknStrm.gt();
			break;
		case '%' : {
			int lfti = int(lft);
			int rghti = int(prcsPrmry());
			if(rghti == 0) {
				cout << "Error with %: divide by zero\n";
				psWndw("~~");
			};
			lft = lfti % rghti;
			Tk = TknStrm.gt();
			break;
			}
		default:
			TknStrm.ptBck(Tk);
			return lft; 
		};
	};
};
double prcsPrmry() {
	Tkn Tk = TknStrm.gt();
	switch(Tk.knd) {
	case '(' : {
		double d = prcsXprsn();
		Tk = TknStrm.gt();
		if(Tk.knd !=')') cout << "')' expected\n";
		return d;
	} break;
	case '#' :
		return Tk.vl;
		break;
	case qt :
		TknStrm.ptBck(Tk);
		return 0;
	case '-' :
		return - prcsPrmry();
	case '+' :
		return prcsPrmry();
	default:
		cout << "primary expected\n";
		psWndw("~~");
	};
};
void psWndw(string s) {
	cout << "Please Enter " << s << " to exit program.\n";
	for(string ch; cin >> ch;) if(ch == s) exit(0);
};
