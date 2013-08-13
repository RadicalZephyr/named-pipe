//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Geoff Shannon 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_PIPE_WINDOWS_HPP
#define BOOST_INTERPROCESS_NAMED_PIPE_WINDOWS_HPP

#include <cstddef>
#include <string>

#include <boost/asio/buffer.hpp>

namespace boost {
namespace interprocess {

  class named_pipe_impl
  {
  public:
    named_pipe_impl(const std::string &name);

    static named_pipe_impl *create_and_accept(const std::string &name);

    const std::string &get_name() {
      return _name;
    }

    std::size_t read(char *buffer, const int length);

    std::size_t write(const char *buffer, const int length);

  private:
    const std::string _name;

  };

  named_pipe_impl::named_pipe_impl(const std::string &name): _name(name)
  {}

  inline named_pipe_impl *named_pipe_impl::create_and_accept(const std::string &name) {
    return new named_pipe_impl(name);
  }

  inline std::size_t named_pipe_impl::read(char *buffer, const int length) {
    return 0;
  }

  inline std::size_t named_pipe_impl::write(const char *buffer, const int length) {
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
    const std::string _name;
  };

  named_pipe_server_impl::named_pipe_server_impl(const std::string &name):
    _name(name) {

  }

  inline named_pipe_impl *named_pipe_server_impl::accept() {
    return new named_pipe_impl(_name);
  }

  // End named_pipe_server_impl

}  //namespace interprocess {
}  //namespace boost {

#endif  //BOOST_INTERPROCESS_NAMED_PIPE_WINDOWS_HPP
