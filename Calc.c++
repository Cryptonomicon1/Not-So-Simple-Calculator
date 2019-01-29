#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#include<string>
using std::string;

string in;
float out;

int main() {
	cout << "Enter: ";
	while(!(cin >> in)) cout << "\nTry again: ";

	out = float(in[0])-48;
	for(int i(1); i < in.size(); i++) {
		switch(in[i]) {
			case '+' :
				out += float(in[i+1])-48;
				break;
			case '-' :
				out -= float(in[i+1])-48;
				break;
			case '*' :
				out *= float(in[i+1])-48;
				break;
			case '/' :
				out /= float(in[i+1])-48;
				break;
		};
	};
	cout << "Answer: " << out << endl;
}
