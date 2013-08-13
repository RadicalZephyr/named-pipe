
#include <cstdio>
#include "named_pipe.hpp"

using namespace boost::interprocess;

int main() {
  const char *pipename = "my/named/pipe";
  named_pipe_server server(pipename);
  named_pipe pipe = server.accept();

  printf("%s == %s\n", pipe.get_name().c_str(), pipename);
}
