// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at <a href="http://www.boost.org/LICENSE_1_0.txt">http://www.boost.org/LICENSE_1_0.txt</a>)
//

#include "connection.hh"
#include <utility>
#include <vector>
#include <boost/asio.hpp>

Connection::Connection(boost::asio::ip::tcp::socket socket)
  : socket_(std::move(socket))
{
}

void Connection::stop()
{
  socket_.close();
}


void 
Connection::async_read_until(const std::string& delim, 
	std::function<void(const boost::system::error_code &, size_t)> handler)
{
	boost::asio::async_read_until(socket_, buffer_, delim, handler);
}
	
void 
Connection::async_write(std::function<
	void(const boost::system::error_code&, size_t)> handler)
{
	boost::asio::async_write(socket_, buffer_, std::bind(
		[handler](const boost::system::error_code& e, 
			size_t n, ConnectionPtr) {	/**< 为了不让connection过早析构 */
			handler(e, n);
		}, 
		std::placeholders::_1, std::placeholders::_2, shared_from_this()));
}