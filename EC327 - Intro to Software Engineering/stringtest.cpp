#include <iostream>
#include <string>
using std::string;
using std::cerr;
using std::cout;
using std::cin;

int main(){
	string numstr;
	int frombase, tobase;
	cin >> numstr;
	cin >> frombase;
	cin >> tobase;
	int num = 0, exp = 1;
  for (int i = numstr.length() - 1; i >= 0; i--) {
    num += (numstr[i] - '0') * exp;
    exp = exp * frombase;
  }
  string result;
  for (; num; num /= tobase) {
    result.insert(result.begin(), num % tobase + '0');
  }
  cout << result;
  return 0;
}