#include "acs/conn/AsyncTcpServer.hpp"
#include "acs/util/Logger.hpp"
#include <asio/io_context.hpp>
#include <system_error>
#include <iostream>
#include <cstdlib>

using namespace acs;

constexpr unsigned short DEFAULT_PORT = 54321;

/// The entry point to the server application.
/**
 * \todo use argv for \a portNumber and \a service
 */
int main(int argc, char *argv[]) {
    asio::io_context context{};

    auto portNum = DEFAULT_PORT;
    if (argc > 1) {
        //TODO use std::from_chars (C++17)
        portNum = std::atoi(argv[1]);
    }

    util::Logger logger(std::cout, std::cerr);
    util::Logger::registerInstance(logger);

    try {
        conn::AsyncTcpServer tcpServer(context, portNum);

        util::Logger::instance().log() << "Server listening on port " << portNum << " on any interface."
            << std::endl;

        context.run();
    } catch (const std::system_error &err) {
        std::cerr << "EXCEPTION CAUGHT:\n"
                << err.code() << ": " << err.what()
                << std::endl;
    }

    return 0;
}