#include "proto/example.pb.h"

#include <iostream>
#include <asio.hpp>


using asio::ip::udp;

class udp_server
{
public:
    udp_server(asio::io_service& io_service)
            : m_socket(io_service, udp::endpoint(udp::v4(), 40010))
    {
        start_receive();
    }

private:
    void start_receive()
    {
        m_socket.async_receive_from(
                asio::buffer(m_recv_buffer),
                m_remote_endpoint,
                [this](const asio::error_code& error, const std::size_t& bytes_received)
                {
                    if (!error)
                    {
                        example::proto::Event event;
                        event.ParseFromArray(&m_recv_buffer, bytes_received);

                        if (event.IsInitialized())
                        {
                            std::cout << "Received data from: " << m_remote_endpoint << '\n' << std::endl;
                            std::cout << event.DebugString() << '\n' << std::endl;
                        }
                        else
                        {
                            std::cerr << "Failed to decode message!" << std::endl;
                        }

                        start_receive();
                    }
                    else
                    {
                        std::cerr << "An error occurred!" << std::endl;
                    }
                }
        );
    }

    udp::socket m_socket;
    udp::endpoint m_remote_endpoint;
    std::array<char, 64000> m_recv_buffer;
};

int main()
{
    try
    {
        asio::io_service io_service;
        udp_server server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

