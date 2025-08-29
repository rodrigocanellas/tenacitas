#include <utility> // Fix for Boost 1.74

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>

namespace beast     = boost::beast;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = net::ip::tcp;

int main()
{
  try
  {
    net::io_context ioc;

    tcp::resolver                  resolver{ioc};
    websocket::stream<tcp::socket> ws{ioc};

    auto const results = resolver.resolve("127.0.0.1", "9002");
    net::connect(ws.next_layer(), results.begin(), results.end());

    ws.handshake("127.0.0.1", "/");

    while (true)
    {

      std::string msg;
      std::cout << "enter one word: ";
      std::cin >> msg;
      ws.write(net::buffer(msg));

      beast::flat_buffer buffer;
      ws.read(buffer);

      std::cout << "Received: " << beast::make_printable(buffer.data()) << "\n";
    }

    ws.close(websocket::close_code::normal);
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
}
