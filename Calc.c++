#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#include<string>
using std::string;

string in;

int main() {
	cout << "Enter: ";
	while(!(cin >> in)) cout << "\nTry again: ";

	//cout << "\n" << in.size() << "\n"; //TStool

	for(int i(0); i < in.size(); i++) {
		if(in[i] == '+' && i > 0 && i+1 < in.size()) {
			cout << "\nAnswer: ";
			cout << float(in[i-1])-48 + float(in[i+1])-48;
			cout << endl;

		} else if(in[i] == '-' && i > 0 && i+1 < in.size()) {
			cout << "\nAnswer: ";
			cout << float(in[i-1])-48 + float(in[i+1])-48;

		};
	};
}
