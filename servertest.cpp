
#include <cstdio>
#include <cstring>
#include <chrono>
#include <thread>

#include "testdata.h"
#include "named_pipe.hpp"

#define BUFSIZE 120

using namespace boost::interprocess;

int main() {
  named_pipe_server server(pipename);

  if (strcmp(pipename, server.get_name().c_str()) != 0) {
    printf("error: received '%s'; expected '%s'\n",
           server.get_name().c_str(), pipename);
    return 1;
  }

  named_pipe pipe = server.accept();

  if (strcmp(pipename, pipe.get_name().c_str()) != 0) {
    printf("error: received '%s'; expected '%s'\n",
           pipe.get_name().c_str(), pipename);
    return 2;
  }

  char buff[BUFSIZE];

  pipe.read(buff, BUFSIZE);

  if (strcmp(clientString, buff) != 0) {
    printf("error: received '%s'; expected '%s'\n",
           buff, clientString);
    return 3;
  }

  pipe.write(serverString, strlen(serverString));

  std::chrono::milliseconds dura( 2000 );
  std::this_thread::sleep_for(dura);

  return 0;
}
