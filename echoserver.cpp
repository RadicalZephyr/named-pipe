
#include <cstdio>
#include <cstring>
#include <string>

#include "testdata.h"
#include "named_pipe.hpp"

#define BUFSIZE 512

int main() {
  printf("Starting the echo server...\n");
  boost::interprocess::named_pipe_server server(pipename);
  boost::interprocess::named_pipe pipe = server.accept();

  printf("Received pipe connection!");

  const char *exit = "exit";
  const std::string yousaid("You said: ");

  char buffer[BUFSIZE];
  int read = 0;

  printf("\nStarting to echo...\n--\n");
  bool running = true;
  while (running) {
    read = pipe.read(buffer, BUFSIZE);
    printf("received: '%s'\n", buffer);
    pipe.write((yousaid+buffer).c_str(), yousaid.length()+read);
    if (strncmp(exit, buffer, 4) == 0) {
        running = false;
        pipe.write("Goodbye!!", 9);
      }
  }
  printf("\nDone echoing.\n");
}
