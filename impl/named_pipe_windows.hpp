//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Geoff Shannon 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_PIPE_WINDOWS_HPP
#define BOOST_INTERPROCESS_NAMED_PIPE_WINDOWS_HPP

#include <windows.h>

#include <cstddef>
#include <string>

#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>

#define BUFFSIZE 512
#define QLEN 10

namespace boost {
namespace interprocess {
namespace impl {

  using namespace boost::system;

  std::string &getPipePrefix(void) {
    static std::string prefix("\\\\.\\pipe\\boost\\interprocess\\");
    return prefix;
  }

  class named_pipe_impl
  {
  public:
    named_pipe_impl(const std::string &name);

    named_pipe_impl(const std::string &name, HANDLE pipe): _name(name), _pipe(pipe)
    {}

    static named_pipe_impl *create_and_accept(const std::string &name);

    const std::string &get_name() {
      return _name;
    }

    std::size_t read(char *buffer, const int length);

    std::size_t write(const char *buffer, const int length);

  private:
    const std::string _name;

    HANDLE _pipe;

  };

  named_pipe_impl::named_pipe_impl(const std::string &name):
    _name(name) {
    std::string whole = getPipePrefix() + _name;
    _pipe = CreateFile(whole.c_str(),
                       GENERIC_READ |  // read and write access
                       GENERIC_WRITE,
                       0,              // no sharing
                       NULL,           // default security attributes
                       OPEN_EXISTING,  // opens existing pipe
                       0,              // default attributes
                       NULL);
    if (_pipe == INVALID_HANDLE_VALUE) {
      error_code ec(GetLastError(), system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
  }

  inline named_pipe_impl *named_pipe_impl::create_and_accept(const std::string &name) {
    return new named_pipe_impl(name);
  }

  inline std::size_t named_pipe_impl::read(char *buffer, const int length) {
    DWORD read;
    bool success = ReadFile(_pipe, buffer, length, &read, NULL);
    if (!success && GetLastError() != ERROR_MORE_DATA ) {
      error_code ec(GetLastError(), system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
    return read;
  }

  inline std::size_t named_pipe_impl::write(const char *buffer, const int length) {
    DWORD written;
    bool success = WriteFile(_pipe, buffer, length, &written, NULL);
    if (!success) {
      error_code ec(GetLastError(), system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
    return written;
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
  };

  named_pipe_server_impl::named_pipe_server_impl(const std::string &name):
    _name(name) {

  }

  inline named_pipe_impl *named_pipe_server_impl::accept() {
    std::string whole = getPipePrefix() + _name;
    HANDLE pipe = CreateNamedPipe(whole.c_str(),
                                  PIPE_ACCESS_DUPLEX,       // read/write access
                                  PIPE_TYPE_BYTE |
                                  PIPE_READMODE_BYTE |
                                  PIPE_WAIT |
                                  PIPE_REJECT_REMOTE_CLIENTS,
                                  QLEN,
                                  BUFFSIZE,
                                  BUFFSIZE,
                                  0,
                                  NULL);
    if (pipe == INVALID_HANDLE_VALUE) {
      error_code ec(GetLastError(), system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }
    bool connected = ConnectNamedPipe(pipe, NULL) ?
         TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

    if (!connected) {
      error_code ec(GetLastError(), system_category());
      system_error e(ec);
      boost::throw_exception(e);
    }

    return new named_pipe_impl(_name, pipe);
  }

  // End named_pipe_server_impl

}  //namespace impl {
}  //namespace interprocess {
}  //namespace boost {

#endif  //BOOST_INTERPROCESS_NAMED_PIPE_WINDOWS_HPP
