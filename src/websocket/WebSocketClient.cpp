#include "WebSocketClient.h"
#include <iostream>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>

WebSocketClient::WebSocketClient(net::io_context& ioc, const std::string& host, const std::string& port, const std::string& target)
    : m_ioc(ioc),
      m_resolver(net::make_strand(ioc)),
      m_ws(nullptr),
      m_host(host),
      m_port(port),
      m_target(target),
      m_connected(false) {
    
    // Configure SSL context
    m_ctx.set_verify_mode(ssl::verify_none); // For testing only, use verify_peer in production
}

WebSocketClient::~WebSocketClient() {
    disconnect();
}

void WebSocketClient::connect(MessageCallback callback) {
    m_callback = std::move(callback);
    
    m_resolver.async_resolve(
        m_host,
        m_port,
        beast::bind_front_handler(
            &WebSocketClient::onConnect,
            this));
}

void WebSocketClient::disconnect() {
    if (!m_connected || !m_ws) return;
    
    beast::error_code ec;
    m_ws->close(websocket::close_code::normal, ec);
    m_connected = false;
}

void WebSocketClient::onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type endpoint) {
    if (ec) {
        std::cerr << "Error resolving: " << ec.message() << std::endl;
        return;
    }
    
    // Create the SSL stream
    m_ws = std::make_unique<websocket::stream<beast::ssl_stream<beast::tcp_stream>>>(net::make_strand(m_ioc), m_ctx);
    
    // Set SNI hostname
    if(!SSL_set_tlsext_host_name(m_ws->next_layer().native_handle(), m_host.c_str())) {
        ec = beast::error_code(static_cast<int>(::ERR_get_error()),
                             net::error::get_ssl_category());
        return;
    }
    
    beast::get_lowest_layer(*m_ws).expires_after(std::chrono::seconds(30));
    beast::get_lowest_layer(*m_ws).async_connect(
        endpoint,
        beast::bind_front_handler(
            &WebSocketClient::onSSLHandshake,
            this));
}

void WebSocketClient::onSSLHandshake(beast::error_code ec) {
    if (ec) {
        std::cerr << "SSL Handshake failed: " << ec.message() << std::endl;
        return;
    }
    
    m_ws->next_layer().async_handshake(
        ssl::stream_base::client,
        beast::bind_front_handler(
            &WebSocketClient::onHandshake,
            this));
}

void WebSocketClient::onHandshake(beast::error_code ec) {
    if (ec) {
        std::cerr << "Error connecting: " << ec.message() << std::endl;
        return;
    }
    
    m_ws->set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
    m_ws->set_option(websocket::stream_base::decorator(
        [](websocket::request_type& req) {
            req.set(http::field::user_agent, "GoQuant-Client/1.0");
        }));
    
    m_ws->async_handshake(m_host, m_target,
        beast::bind_front_handler(
            &WebSocketClient::doRead,
            this));
    
    m_connected = true;
}

void WebSocketClient::doRead() {
    m_ws->async_read(
        m_buffer,
        beast::bind_front_handler(
            &WebSocketClient::onRead,
            this));
}

void WebSocketClient::onRead(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    
    if (ec) {
        std::cerr << "Error reading: " << ec.message() << std::endl;
        m_connected = false;
        return;
    }
    
    std::string message = beast::buffers_to_string(m_buffer.data());
    m_buffer.consume(m_buffer.size());
    
    if (m_callback) {
        m_callback(message);
    }
    
    doRead();
}