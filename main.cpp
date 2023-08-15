#include <string>
#include <iostream>

using namespace std;

int main() {
  cout << "Enter name:\n"s;
  string name;
  getline(cin, name);
  if (10 < name.length()) {
    throw invalid_argument{"Name should not be longer than 10 characters"};
  }

  int d = 3;
  int result = 3;
  for (char c: name) {
    result = 7 * c + (d + c) * result + 0x1341;
    ++d;
  }
  result = result & 0xDEADBEEF;

  cout << result << endl;
}