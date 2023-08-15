Solution for keygenme_easy_1 by subminking https://crackmes.one/crackme/5ab77f6533c5d40ad448cb99

This keygenme does not show any message for wrong serial, but there are success strings:
```
.rdata:00404200 Caption         db 'You did it!',0
.rdata:0040420C Text            db 'Congrats! Make me a little tutorial plz....',0
```

Entered name & serial are stored in global buffers:
```
.data:00417668 ; char name[]
.data:004177F8 ; char serial[]
```

sub_401040 calculates valid serial from name and stores it in ```.data:00417730 ; char validSerial[]```

```.text:00401180 ; BOOL __stdcall DialogFunc``` 
contains loop at loc_401202 that compares serial and validSerial.

I found a bug: if seral length is greater than 10 then valid serial is different at every program launch. Code tries to concatenate first 5 chars and last 5 chars of serial using strncpy, but forgets to put terminating '\0' bytes into buffers.

```c++
strncpy(var_194, name, 5);
// no var_194[5] = '\0';
strncpy(var_CC, name + strlen(name) - 5, 5);
// no var_CC[5] = '\0';
```

As a result ```loc_4010A7``` and ```loc_4010C0``` get incorrect string lengths of var_CC and var_194. Trash bytes in buffer affect name hash calculation and lead to undefined behaviour.

There are examples of valid name & serial:
* name="sa2304", serial="-628320019"
* name="123", serial="11340974"
* name="my name", serial="-1876870580"

Here is the code for keygen:
```c++
#include <string>
#include <iostream>

using namespace std;

int main() {
  cout << "Enter name:\n"s;
  string name;
  getline(cin, name);
  if (10 < name.length()) throw invalid_argument{"Name should not be longer than 10 characters"};
  
  
  int d = 3;
  int result = 3;
  for (char c: name) {
    result = 7 * c + (d + c) * result + 0x1341;
    ++d;
  }

  result = result & 0xDEADBEEF;
  cout << result << endl;
}
```