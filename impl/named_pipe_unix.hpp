//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Geoff Shannon 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_PIPE_UNIX_HPP
#define BOOST_INTERPROCESS_NAMED_PIPE_UNIX_HPP

#include <cstddef>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

#include <time.h>
#include <errno.h>

#include "helper.hpp"

#define PATH_PREFIX "boost/interprocess"
#define CLI_PERM S_IRWXU // rwx for user only

#define QLEN 10
#define STALE 30

namespace boost {
namespace interprocess {
namespace impl {

  using namespace boost::system;

  class named_pipe_impl
  {
  public:
    named_pipe_impl(const std::string &name);

    named_pipe_impl(const std::string &name, int fd):
      _name(name), _fd(fd) {}

    const std::string &get_name() {
      return _name;
    }

    std::size_t read(char *buffer, const int length);

    std::size_t write(const char *buffer, const int length);

  private:
    const std::string _name;

    int _fd;
  };

  named_pipe_impl::named_pipe_impl(const std::string &name): _name(name) {

    _fd = make_local_socket();

    struct sockaddr_un un;
    char buff[128];
    int len = snprintf(buff,128, "%s/%s/%s",
                       get_temp_path(), PATH_PREFIX, name.c_str());

    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, buff);
    len += offsetof(struct sockaddr_un, sun_path);

    if (connect(_fd, (struct sockaddr *)&un, len) < 0) {
      error_code ec(errno, system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
  }

  inline std::size_t named_pipe_impl::read(char *buffer, const int length) {
    std::size_t size = ::read(_fd, buffer, length);
    if (size < 0) {
      error_code ec(errno, system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
    buffer[size] = '\0';
    return size;
  }

  inline std::size_t named_pipe_impl::write(const char *buffer, const int length) {
    std::size_t size = ::write(_fd, buffer, length);
    if (size < 0) {
      error_code ec(errno, system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
    return size;
  }

  // End named_pipe_impl


  class named_pipe_server_impl
  {
  public:
    named_pipe_server_impl(const std::string &name);

    const std::string &get_name() {
      return _name;
    }

    named_pipe_impl *accept();

  private:

    const std::string _name;

    int _fd;
  };

  named_pipe_server_impl::named_pipe_server_impl(const std::string &name):
    _name(name), _fd(-1) {

    _fd = make_local_socket();

    char buff[128];
    snprintf(buff,128, "%s/%s/%s",
             get_temp_path(), PATH_PREFIX, name.c_str());
    unlink(buff);
    bind_local_socket(_fd, buff);

    // Tell kernel we're a server
    if (listen(_fd, QLEN) < 0) {
      error_code ec(errno, system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
  }

  named_pipe_impl *named_pipe_server_impl::accept() {
    int clifd;
    struct sockaddr_un un;

    socklen_t len = sizeof(un);

    if ((clifd = ::accept(_fd, (struct sockaddr *)&un, &len)) < 0) {
      error_code ec(errno, system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }

    return new named_pipe_impl(_name, clifd);
  }

  // End named_pipe_server_impl

}  //namespace impl {
}  //namespace interprocess {
}  //namespace boost {

#endif  //BOOST_INTERPROCESS_NAMED_PIPE_UNIX_HPP
