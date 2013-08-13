
#include <cstdio>
#include <cstring>

#include "testdata.h"
#include "named_pipe.hpp"

#define BUFSIZE 120

using namespace boost::interprocess;

int main() {
  named_pipe_server server(pipename);

  if (strcmp(pipename, server.get_name().c_str()) != 0)
    return 1;

  named_pipe pipe = server.accept();

  if (strcmp(pipename, pipe.get_name().c_str()) != 0)
    return 1;

  char buff[BUFSIZE];

  pipe.read(buff, BUFSIZE);

  if (strcmp(clientString, buff) != 0)
    return 1;

  pipe.write(serverString, strlen(serverString));

  return 0;
}
