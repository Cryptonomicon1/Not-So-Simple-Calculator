// Includes
//------------------------------------------------------------------
#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#include<vector>
using std::vector;

// Function Declarations
//------------------------------------------------------------------
double prcsXprsn();
double prcsTrm();
double prcsPrmry(); 

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
		double vl = 0;
		Tnk Tk;
		while(cin) {
			Tk = TknStrm.gt();

			if(Tk.knd == 'q') break;
			if(Tk.knd == ';') cout << "=" << vl << '\n';
			else TknStrm.ptBck(Tk);

			vl = prcsXprsn();
		};
		//keep_window_open();
	/*
	} catch() {
		cout << "Error 1: " << '\n';
		//keep_window_open();
		return 1;
	*/
	} catch(...) {
		cout << "Error!\n";
		//keep_window_open();
		return 2;
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
	if(fll) cout << "putback into full buffer\n";
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
	case ';' :
	case 'q' :
	case '(' : case ')' : case '+' :
	case '-' : case '*' : case '/' :
		return Tkn {chr, 0};
	case '.' :
	case '0' : case '1' : case '2' : case '3' : case '4' :
	case '5' : case '6' : case '7' : case '8' : case '9' : {
		cin.putback(chr);
		double vl;
		cin >> vl;
		return Tkn {'8', vl};
	};
	default:
		cout << "Bad Token\n";
	};
};
// Function Definitions
//------------------------------------------------------------------
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
		default:
			Tk = TknStrm.gt();
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
	case '8' :
		return Tk.vl;
		break;
	default:
		cout << "primary expected\n";
	};
};
