//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Geoff Shannon 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_PIPE_HPP
#define BOOST_INTERPROCESS_NAMED_PIPE_HPP

#include <boost/config.hpp>

#include <boost/scoped_ptr.hpp>

#ifdef BOOST_WINDOWS

#include "impl/named_pipe_windows.hpp"

#else // !defined BOOST_WINDOWS

#include "impl/named_pipe_unix.hpp"

#endif

namespace boost {
namespace interprocess {

  class named_pipe
  {

  public:
    /// Construct and open a named_pipe_object
    /**
     * This constructor opens a connection to an already existing named
     * pipe with the specified name.
     *
     * @param name The name of the named pipe to connect to.
     */
    named_pipe(const std::string &name): _name(name),
                                         _pimpl(new named_pipe_impl(name))
    {}

    /// Returns the name of the named pipe object.
    const std::string &get_name() {
      return _name;
    }

    /// Read some data from the named pipe.
    /**
     * This function is used to read data to the named pipe.  The
     * function call will block until one or more bytes of data have
     * been read successfully, or until an error occurs.
     *
     * @param buffer A buffer into which the data will be read.
     *
     * @return The number of bytes read.
     *
     * @throws boost::system::system_error Thrown on failure.
     *
     * @note The read_some operation may not completely fill the
     * buffer.
     */
    std::size_t read_some(boost::asio::mutable_buffer &buffer) {
      return _pimpl->read_some(buffer);
    }

    /// Write some data to the named pipe.
    /**
     * This function is used to write data to the named pipe.  The
     * function call will block until one or more bytes of data have
     * been written successfully, or until an error occurs.
     *
     * @param buffer A buffer to be written to the named pipe.
     *
     * @return The number of bytes written.
     *
     * @throws boost::system::system_error Thrown on failure.
     *
     * @note The write_some operation may not transmit all of the data
     * to the peer.
     */
    std::size_t write_some(boost::asio::const_buffer &buffer) {
      return _pimpl->write_some(buffer);
    }

  private:

    const std::string &_name;

    scoped_ptr<named_pipe_impl> _pimpl;
  };

  class named_pipe_server
  {

  public:
    /// Construct a named_pipe_server.
    /**
     * This constructor creates an object which can be used to listen
     * for named pipe connections under the specified name.
     *
     * @param name The name of the new pipe server.
     *
     * @note Nothing is actually opened or setup until a call to accept
     * is made.
     */
    named_pipe_server(const std::string &name):
      _name(name),
      _pimpl(new named_pipe_server_impl(name))
    {}

    /// Returns the name of the named pipe.
    const std::string &get_name() {
      return _name;
    }

    /// Waits for a new connection from a client process.
    /**
     * This function is used to wait for and accept a new connection
     * on this named pipe.
     *
     * @return The named_pipe for communicating with the new client.
     */
     named_pipe accept() {
     }

  private:

    const std::string &_name;

    boost::scoped_ptr<named_pipe_server_impl> _pimpl;

  };

}  //namespace interprocess {
}  //namespace boost {

#endif  //BOOST_INTERPROCESS_NAMED_PIPE_HPP
