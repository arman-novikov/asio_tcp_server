#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(nullptr);
  return ctime(&now);
}

static const std::string HTML_PAGE =
    "<!DOCTYPE html> \
    <html>\
    <head>\
        <title></title>\
    </head>\
    <body>\
    <h1>Worked!</h1>\
    </body>\
    </html>";

static const std::string HTML_HEADER =
    "HTTP/1.1 200 OK\r\n\
    Host: 127.0.0.1\r\n\
    Content-Type: text/html; charset=UTF-8\r\n\
    Connection: close\r\n\
    Content-Length: ";

int main(int argc, char* argv[])
{
    boost::asio::ip::address ip{};
    uint16_t port;
    if (argc < 3) {
        std::cerr << "use argv: <ip> <port>" << std::endl;
        return -22;
    }

    ip = boost::asio::ip::address::from_string(argv[1]);
    port = static_cast<uint16_t>(std::stoi(argv[2]));
  try
  {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(ip, port)); // use tcp::v4() instead of ip to have a default ipaddr

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      const std::string len = std::to_string(HTML_PAGE.length()) + std::string("\r\n\r\n");
      const std::string message = HTML_HEADER + len + HTML_PAGE;

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message),
          boost::asio::transfer_all(), ignored_error);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
