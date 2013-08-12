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

#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define PATH_PREFIX "boost/interprocess"
#define CLI_PERM S_IRWXU // rwx for user only
#define QLEN 10

namespace boost {
namespace interprocess {

  class named_pipe_impl
  {
  public:
    named_pipe_impl(const std::string &name);

    named_pipe_impl(const std::string &name, int fd):
      _name(name), _fd(fd) {}

    const std::string &get_name() {
      return _name;
    }

    std::size_t read(boost::asio::mutable_buffer &buffer);

    std::size_t write(boost::asio::const_buffer &buffer);

  private:
    const std::string _name;

    int _fd;
  };

  named_pipe_impl::named_pipe_impl(const std::string &name): _name(name)
  {}

  inline std::size_t named_pipe_impl::read(boost::asio::mutable_buffer &buffer) {
    return 0;
  }

  inline std::size_t named_pipe_impl::write(boost::asio::const_buffer &buffer) {
    return 0;
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

    typedef shared_ptr<struct sockaddr_un> sockAddrPtr;

    const std::string _name;

    int _fd;

    sockAddrPtr _un;
  };

  named_pipe_server_impl::named_pipe_server_impl(const std::string &name):
    _name(name), _fd(-1), _un(new struct sockaddr_un)
  {
    if ((_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
      // TODO: Do things for failure
    }

    _un->sun_family = AF_UNIX;

    int len = sprintf(_un->sun_path, "%s/%s",
                      PATH_PREFIX, name.c_str());
    len += offsetof(struct sockaddr_un, sun_path);

    // TODO: Unsure if this is a good idea...
    unlink(_un->sun_path); // in case it already exists

    if (bind(_fd, (struct sockaddr *)&(*_un), len) < 0) {
      // TODO: Do things for failure
    }

    // Tell kernel we're a server
    if (listen(_fd, QLEN) < 0) {
      // TODO: Do things for failure
    }
  }

  inline named_pipe_impl *named_pipe_server_impl::accept() {
    int clifd;
    struct sockaddr_un un;

    socklen_t len = sizeof(un);

    if ((clifd = accept(_fd, (struct sockaddr *)&un, &len)) < 0) {
      // TODO: Do things for failure
    }

    len -= offsetof(struct sockaddr_un, sun_path);
    un.sun_path[len] = 0; // null terminate string

    struct stat statbuf;
    if (stat(un.sun_path, &statbuf) < 0) {
      // TODO: Do things for failure
    }

#ifdef S_ISSOCK // not defined for SVR4
    if (S_ISSOCK(statbuf.st_mode) == 0) {
      // TODO: Do things for failure
    }
#endif
    time_t staletime = time(NULL) - STALE;
    if (statbuf.st_atime < staletime ||
        statbuf.st_ctime < staletime ||
        statbuf.st_mtime < staletime) {
      // TODO: Do things for failure
    }

    if (uidptr != NULL) {
      // TODO: Do things for failure
    }
    unlink(un.sun_path);
    return new named_pipe_impl(clifd);
  }

  // End named_pipe_server_impl

}  //namespace interprocess {
}  //namespace boost {

#endif  //BOOST_INTERPROCESS_NAMED_PIPE_UNIX_HPP
