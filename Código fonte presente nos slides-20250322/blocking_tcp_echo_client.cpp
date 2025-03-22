#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

static const unsigned MAX_LENGTH = 1024;


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      cerr << "Usage: blocking_tcp_echo_client <host> <port>" << endl;
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    ret = boost::asio::connect(s, iterator);

    cout << "Enter message: ";
		char request[MAX_LENGTH];
		cin.getline(request, MAX_LENGTH-1);

		s.write_some(boost::asio::buffer(request,strlen(request)+1));

		char reply[MAX_LENGTH];
    int reply_length = s.read_some(boost::asio::buffer(reply, MAX_LENGTH));

    cout << "Reply is: " << reply << endl;
  }
  catch (std::exception& e)
  {
    cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
