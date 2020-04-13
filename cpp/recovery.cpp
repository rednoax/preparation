#include <stdio.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <limits>

using namespace std;
template <typename T>
bool ParseInt(const char* s, T* out,//out:PO
              T min = std::numeric_limits<T>::min(),
              T max = std::numeric_limits<T>::max()) {
	std::cout << "min:" << min << ";max:" << max << std::endl;
  int base = (s[0] == '0' && s[1] == 'x') ? 16 : 10;
  errno = 0;
  char* end;
  long long int result = strtoll(s, &end, base);
  printf("errno %d: result %llx, end [%s]\n", errno, result, end);
  bool ret = true;
  if (errno != 0 || s == end || *end != '\0') {
    ret = false;
	goto EXIT;
  }
  if (result < min || max < result) {
    ret = false;
	goto EXIT;
  }
  *out = static_cast<T>(result);
EXIT:
	printf("ret %d\n", ret);
  return ret;
}

int main()
{
	string num_string = "eng.rednoah.20200319.131654";
	int build_numbder;
	ParseInt(num_string.c_str(), &build_numbder, 0);
	return 0;
}