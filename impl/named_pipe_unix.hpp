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

namespace boost {
namespace interprocess {

  class named_pipe_impl
  {
  public:
    named_pipe_impl(const std::string &name);

    const std::string &get_name() {
      return _name;
    }

    std::size_t read_some(boost::asio::mutable_buffer &buffer);

    std::size_t write_some(boost::asio::const_buffer &buffer);

  private:
    const std::string _name;

  };

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

}  //namespace interprocess {
}  //namespace boost {

#endif  //BOOST_INTERPROCESS_NAMED_PIPE_UNIX_HPP
