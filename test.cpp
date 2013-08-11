
#include "named_pipe.hpp"

using namespace boost::interprocess;

int main() {

  named_pipe_server server("my/named/pipe");
  named_pipe pipe = server.accept();
}
