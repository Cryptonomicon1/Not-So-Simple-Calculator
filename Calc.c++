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
class Variable;

static const char qt = 'q';
static const char prnt = ';';
static const char prmpt = '>';
static const char rslt = '=';
vector<Variable> vrbls;
static const char name = 'a';
static const char let = 'L';
static const string declkey = "let";

// Function Declarations
//------------------------------------------------------------------
void clcltVls();
double prcsXprsn();
double prcsTrm();
double prcsPrmry();
void psWndw(string);
void clnMss();
double gtVl(string, vector<Variable>);
void stVl(string, double);
double prcsSttmnt();
double prcsDclrtn();
bool sDclrd(string, vector<Variable>);
double dfnNm(string, double);

// Class Definitions
//------------------------------------------------------------------
class Tkn {
public:
	Tkn();
	Tkn(char);
	Tkn(char, double);
	Tkn(char ch, string n);
	char knd;
	float vl;
	string nm;
};
class TokenStream {
public:
	TokenStream();
	Tkn gt();
	void ptBck(Tkn);
	void gnrStrng(char);
private:
	bool fll;
	Tkn bffr;
};
class Variable {
public:
	string nm;
	double vl;
};
// Instantiations
//------------------------------------------------------------------
TokenStream TknStrm;
Variable Vrbl;

// Main Program
//------------------------------------------------------------------
int main() {
	try {
		clcltVls();
	} catch(runtime_error& e) {
		cerr << e.what() << '\n';
		clnMss();
	} catch(...) {
		cerr << "Exception\n";
		psWndw("~~");
	};
}
// Class Function Definitions
//------------------------------------------------------------------
Tkn::Tkn() {
};
Tkn::Tkn(char ch) {
	knd = ch;
};
Tkn::Tkn(char k, double v) {
	knd = k; vl = v;
};
Tkn::Tkn(char k, string v) {
	knd = k; nm = v;
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
	case '0' : case '1' : case '2' : case '3' : case '4' :
	case '5' : case '6' : case '7' : case '8' : case '9' : {
		cin.putback(chr);
		double vl;
		cin >> vl;
		return Tkn {'#', vl};
	};
	case '=' :
		return Tkn {'='};
	default:
		if(isalpha(chr)) {
			string str1;
			str1+=chr;
			while(cin.get(chr) && (isalpha(chr) || isdigit(chr)))
				str1+=chr;
			cin.putback(chr);
			//cout << (str1.compare(declkey)==0) << ' ' << str1 << '\n';
			if(str1.compare(declkey) == 0) return Tkn {let};
			else if(sDclrd(str1, vrbls)) {
				//cout << str1 << " " << '\n';
				return Tkn {'#', gtVl(str1, vrbls)};
			};
			return Tkn {name, str1};
		};
		cout << "Bad Token\n";
		clnMss();
		return Tkn {';'};
		//psWndw("~~");
	};
};
void TokenStream::gnrStrng(char chr) {
	if(fll && chr==bffr.knd) {
		fll = false;
		return;
	};
	fll = false;

	//search input
	char c = 0;
	while(cin>>c) if(c==chr) return;
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
		cout << rslt << prcsSttmnt() << '\n';
	};
};
double prcsSttmnt() {
	Tkn Tk = TknStrm.gt();
	//cout << "Func: prcsSttmnt " << Tk.knd << '\n';
	switch(Tk.knd) {
	case let :
		return prcsDclrtn();
	default :
		TknStrm.ptBck(Tk);
		return prcsXprsn();
	};
};
double prcsDclrtn() {
	Tkn Tk1 = TknStrm.gt();
	if(Tk1.knd != name) {
		cout << "Error: name expected in declaration\n";
		psWndw("~~");
	};
	string vr_nm = Tk1.nm;

	//cout << "Func: prcsDclrtn " << vr_nm << '\n';
	Tkn Tk2 = TknStrm.gt();

	//cout << "Func: prcsDclrtn " << Tk2.knd << '\n';

	if(Tk2.knd != '=') {
		cout << "Error: = missing in delcaration of ";
		cout << vr_nm << '\n';
	};
	double dbl1 = prcsXprsn();
	dfnNm(vr_nm, dbl1);
	return dbl1;
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
				clnMss();
				//psWndw("~~");
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
		cout << "Error: primary expected\n";
		clnMss();
		//psWndw("~~");
	};
};
void psWndw(string s) {
	cout << "Please Enter " << s << " to exit program.\n";
	for(string ch; cin >> ch;) if(ch == s) exit(0);
};
void clnMss() {
	TknStrm.gnrStrng(prnt);
};
double gtVl(string str1, vector<Variable> vrbls = vrbls) {
	for(int i(0); i < vrbls.size(); i++) {
		if(vrbls[i].nm.compare(str1)==0) return vrbls[i].vl;
	};
	cout << "Error get: undefined variable " << str1 << '\n';
	psWndw("~~");
};
void stVl(string str1, double dbl1) {
	for(int i(0); i < vrbls.size(); i++) {
		if(vrbls[i].nm.compare(str1)==0) {
			vrbls[i].vl = dbl1;
			return;
		};
	};
	cout << "Error set: undefined variable " << str1 << '\n';
	psWndw("~~");
};
bool sDclrd(string vrble1, vector<Variable> vrbls = vrbls) {
	for(int i(0); i < vrbls.size(); i++)
		if(vrbls[i].nm.compare(vrble1)==0) return true;
	return false;
};
double dfnNm(string vrbl1, double vl1) {
	if(sDclrd(vrbl1)) {
		cout << "Error " << vrbl1 << " : declared twice\n";
		psWndw("~~");
	};
	vrbls.push_back(Variable{vrbl1, vl1});
	//cout << "Func: dfnNm  " << vrbl1 << ' ' << vl1 << '\n';
	return vl1;
};
