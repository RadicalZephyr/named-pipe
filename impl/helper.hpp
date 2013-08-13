//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright geoff@minty-dark-tower 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_PIPE_HELPER_HPP
#define BOOST_INTERPROCESS_NAMED_PIPE_HELPER_HPP

#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <sys/un.h>

#include <errno.h>

#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>

#include <boost/filesystem.hpp>

namespace boost {
namespace interprocess {
namespace impl {

  using namespace boost::system;

  const char *get_temp_path() {
    char *temp = getenv("TEMP");
    if (temp == NULL) {
      temp = getenv("TMP");
    }
    if (temp == NULL) {
      temp = getenv("TMPDIR");
    }
    if (temp == NULL) {
      return "/tmp";
    }
    return temp;
  }

  void ensure_path_exists(const std::string path) {
    size_t index = path.find_first_of('/');
    index = path.find_first_of('/', index+1);

    while (index != std::string::npos) {
      boost::filesystem::create_directory(path.substr(0, index));
      index = path.find_first_of('/', index+1);
    }
  }

  int make_local_socket() {
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
      error_code ec(errno, system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }

    return fd;
  }

  void bind_local_socket(int fd, const std::string &name) {
    ensure_path_exists(name);
    struct sockaddr_un un;

    un.sun_family = AF_UNIX;

    strcpy(un.sun_path, name.c_str());
    int len = name.length() + offsetof(struct sockaddr_un, sun_path);

    if (bind(fd, (struct sockaddr *)&un, len) < 0) {
      error_code ec(errno, system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }

  }

}  //namespace impl {
}  //namespace interprocess {
}  //namespace boost {

#endif //BOOST_INTERPROCESS_NAMED_PIPE_HELPER_HPP
