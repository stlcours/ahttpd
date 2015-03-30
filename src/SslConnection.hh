#pragma once

#include <array>
#include <memory>
#include <string>
#include <utility>
#include <asio/ssl.hpp>
#include <iostream>
#include <iostream>
#include "connection.hh"

class Server;

class SslConnection : public Connection
{
public:
	explicit SslConnection(asio::io_service& service, 
		asio::ssl::context& context)
  		: socket_(service, context), ssl_shutdown_timer_(service)
	{}

	void stop() override;
	socket_t socket() override { return socket_t{ &socket_ }; }
	asio::ip::tcp::socket& nativeSocket() { return socket_.next_layer(); }
	void async_handshake(const std::function<void(const asio::error_code& e)>& handle);
private:
	asio::ssl::stream<asio::ip::tcp::socket> socket_;
	asio::deadline_timer ssl_shutdown_timer_;
	void stopNextLayer(const asio::error_code& ec);
};
