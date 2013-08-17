#include <cstdio>
#include <cstring>
#include <chrono>
#include <thread>

#include "testdata.h"
#include "named_pipe.hpp"

#define BUFSIZE 120

using namespace boost::interprocess;

int main() {
  named_pipe pipe(pipename);

  if (strcmp(pipename, pipe.get_name().c_str()) != 0) {
    printf("error: received '%s'; expected '%s'\n",
           pipe.get_name().c_str(), pipename);
    return 1;
  }

  char buff[BUFSIZE];

  pipe.write(clientString, strlen(clientString));

  pipe.read(buff, BUFSIZE);

  if (strcmp(serverString, buff) != 0) {
    printf("error: received '%s'; expected '%s'\n",
           buff, serverString);
    return 2;
  }

  std::chrono::milliseconds dura( 2000 );
  std::this_thread::sleep_for(dura);

  return 0;
}
