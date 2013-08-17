
#include <stdio.h>
#include <boost/system/system_error.hpp>

#include "chat.h"

#include "named_pipe.hpp"

#define BUFSIZE 512

using namespace boost::interprocess;

int main() {

  printf("\nStarting chat client...");

  printf("\nConnecting to chat server...");
  named_pipe pipe(pipename);

  printf("\nConnected to server!  Enter your name (31 character max.):");

  char name[32];
  fgets(name, 32, stdin);
  {
    int len = strlen(name);
    name[len-1] = '\0'; // Remove newline/EOF from name
  }

  printf("\nSending your name to server...");
  {
    std::string transmit("[name]:");
    transmit = transmit + name;
    pipe.write(transmit.c_str(), transmit.length());
    printf("\nSuccessfully logged in as '%s'!", name);

    char otherusers[34*10];
    pipe.read(otherusers, 32*10);
    printf("\n\nOther users currently present: %s", otherusers);
  }

  printf("\nOkay, start chatting!!\n--\n");

  bool running = true;
  char buffer[BUFSIZE];
  int read;
  while (running) {
    printf("\n[%s]: ", name);
    fgets(buffer, BUFSIZE, stdin);

    try {
      pipe.write(buffer, BUFSIZE);
    } catch (boost::system::system_error e) {

    }

    pipe.read(buffer, BUFSIZE);
    printf("%s", buffer);
  }
}
