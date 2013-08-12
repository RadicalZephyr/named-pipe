//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright geoff@minty-dark-tower 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_PIPE_HELPER_HPP
#define BOOST_INTERPROCESS_NAMED_PIPE_HELPER_HPP

#include <cstddef>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <sys/un.h>

#include <errno.h>

namespace boost {
namespace interprocess {
namespace impl {

  int make_local_socket() {
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
      // TODO: Do things for failure
    }

    return fd;
  }

  void bind_local_socket(int fd, const std::string &name) {
    struct sockaddr_un un;

    un.sun_family = AF_UNIX;

    strcpy(un.sun_path, name.c_str());
    int len = name.length() + offsetof(struct sockaddr_un, sun_path);

    if (bind(fd, (struct sockaddr *)&un, len) < 0) {
      // TODO: Do things for failure
    }

  }

}  //namespace impl {
}  //namespace interprocess {
}  //namespace boost {

#endif //BOOST_INTERPROCESS_NAMED_PIPE_HELPER_HPP
