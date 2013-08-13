#include <cstdio>
#include <cstring>

#include "testdata.h"
#include "named_pipe.hpp"

#define BUFSIZE 120

using namespace boost::interprocess;

int main() {
  named_pipe pipe(pipename);

  if (strcmp(pipename, pipe.get_name().c_str()) != 0)
    return 1;

  char buff[BUFSIZE];

  pipe.write(boost::asio::buffer(clientString));

  pipe.read(boost::asio::buffer(buff));

  if (strcmp(serverString, buff) != 0)
    return 1;

  return 0;
}