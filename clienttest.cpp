#include <cstdio>
#include "named_pipe.hpp"

using namespace boost::interprocess;

int main() {
  const char *pipename = "my/named/pipe";
  named_pipe pipe(pipename);

  printf("%s == %s", pipe.get_name().c_str(), pipename);

}
