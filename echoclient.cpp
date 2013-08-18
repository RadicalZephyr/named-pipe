
#include <cstdio>
#include <cstring>
#include <string>

#include "testdata.h"
#include "named_pipe.hpp"

#define BUFSIZE 512

int main() {
  printf("Starting echo client...");
  boost::interprocess::named_pipe pipe(pipename);
  printf("\nServer available!");

  char buffer[BUFSIZE];
  int read = 0;

  printf("\nStarting to write...\n--\n");
  bool running = true;
  while (running) {
    printf(">> ");
    fgets(buffer, BUFSIZE, stdin);
    read = strlen(buffer);
    buffer[read+1] = '\0';

    pipe.write(buffer, read);
    pipe.read(buffer, BUFSIZE);

    printf("echo: %s\n", buffer);
  }
}
