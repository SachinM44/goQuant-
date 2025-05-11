#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>
#include <functional>
#include <string>
#include <memory>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;

class WebSocketClient {
public:
    using MessageCallback = std::function<void(const std::string&)>;
    
    WebSocketClient(net::io_context& ioc, const std::string& host, const std::string& port, const std::string& target);
    ~WebSocketClient();
    
    void connect(MessageCallback callback);
    void disconnect();
    bool isConnected() const;
    
private:
    void onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type);
    void onSSLHandshake(beast::error_code ec);
    void onHandshake(beast::error_code ec);
    void onRead(beast::error_code ec, std::size_t bytes_transferred);
    void doRead();
    
    net::io_context& m_ioc;
    ssl::context m_ctx{ssl::context::tlsv12_client};
    tcp::resolver m_resolver;
    std::unique_ptr<websocket::stream<beast::ssl_stream<beast::tcp_stream>>> m_ws;
    beast::flat_buffer m_buffer;
    std::string m_host;
    std::string m_port;
    std::string m_target;
    MessageCallback m_callback;
    bool m_connected;
};


### 1. WebSocket Client Implementation

Using Boost.Beast for WebSocket communication:
```cpp
#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <functional>
#include <string>
#include <memory>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class WebSocketClient {
public:
    using MessageCallback = std::function<void(const std::string&)>;
    
    WebSocketClient(net::io_context& ioc, const std::string& host, const std::string& port, const std::string& target);
    ~WebSocketClient();
    
    void connect(MessageCallback callback);
    void disconnect();
    bool isConnected() const;
    
private:
    void onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type);
    void onHandshake(beast::error_code ec);
    void onRead(beast::error_code ec, std::size_t bytes_transferred);
    void doRead();
    
    net::io_context& m_ioc;
    tcp::resolver m_resolver;
    std::unique_ptr<websocket::stream<beast::tcp_stream>> m_ws;
    beast::flat_buffer m_buffer;
    std::string m_host;
    std::string m_port;
    std::string m_target;
    MessageCallback m_callback;
    bool m_connected;
};