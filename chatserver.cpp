
#include <stdio.h>
#include <boost/thread.hpp>
#include <boost/system/system_error.hpp>

#include "chat.h"

#include "named_pipe.hpp"

using namespace boost::interprocess;
using boost::thread;

struct DoChat {
  void operator()(named_pipe pipe);
};

int main() {
  printf("\nStarting chat server...");

  printf("\nCreating pipe server...");

  named_pipe_server server(pipename);

  while(true) {
    named_pipe clipipe = server.accept();
    thread t(DoChat(), clipipe);
  }
}


void DoChat::operator ()(named_pipe pipe) {

}
