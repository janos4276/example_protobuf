#include "proto/example.pb.h"

#include <iostream>
#include <asio.hpp>

using asio::ip::udp;

/**
 * A simple example showing how to create a protocol buffer message and transmitting it via UDP using libasio.
 *
 * This will send the message to localhost:40000
 *
 * You could use socat to "listen" for the message, e.g. socat -u udp-recv:40000 -
 *
 */

int main()
{
    try
    {
        // Set up udp networking service.
        asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), "localhost", "40000");

        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        // Create socket.
        udp::socket socket(io_service);
        socket.open(udp::v4());

        // Create protocol buffer message.
        example::proto::Event event{};
        event.set_message("Hello World!");
        example::proto::Programming* programming = event.add_language();
        programming->set_language("C++");

        asio::streambuf stream_buffer;
        std::ostream output(&stream_buffer);
        event.SerializeToOstream(&output);

        socket.send_to(stream_buffer.data(), receiver_endpoint);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}