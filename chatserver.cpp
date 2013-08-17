
#include <stdio.h>
#include <boost/thread.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread/synchronized_value.hpp>

#include <vector>
#include <queue>
#include <string>

#include "chat.h"

#include "named_pipe.hpp"

#define BUFSIZE 512

using namespace std;
using namespace boost::interprocess;
using boost::thread;

struct DoReceive {
  void operator()(named_pipe pipe);
};

struct DoWrite {
  void operator()();
};

boost::synchronized_value<bool> running = true;
boost::synchronized_value< vector<named_pipe > > pipelist;
boost::synchronized_value< vector<string > > namelist;
boost::synchronized_value< queue<string> > msgqueue;

int main() {
  printf("\nStarting chat server...");

  printf("\nCreating pipe server...");

  named_pipe_server server(pipename);

  while(*running) {
    named_pipe clientpipe = server.accept();
    pipelist->push_back(clientpipe);
    char name[32];
    clientpipe.read(name, 32);
    sscanf(name, "[name]:%s", &name);
    string names("");
    const char *sep = "";
    for (vector<string>::iterator itr = namelist->begin();
         itr != namelist->end(); itr++) {
      names += sep;
      names += *itr;
      sep = ", ";
    }
    clientpipe.write(names.c_str(), names.length());
    namelist->push_back(name);
    thread t(DoReceive(), clientpipe);
  }
}


void DoReceive::operator ()(named_pipe pipe) {
  while(*running) {
    char buffer[BUFSIZE];
    pipe.read(buffer, BUFSIZE);
    msgqueue->push(buffer);
  }
}

void DoWrite::operator ()() {
  while (*running) {
    string msg = msgqueue->front();
    printf("%s\n", msg.c_str());
    for (vector<named_pipe>::iterator i = pipelist->begin();
         i != pipelist->end(); i++) {
      i->write(msg.c_str(), msg.length());
    }
    msgqueue->pop();
  }
}
