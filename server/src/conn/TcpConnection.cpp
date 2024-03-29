#include "acs/conn/TcpConnection.hpp"
#include "acs/logic/ClientHandler.hpp"
#include "acs/message/EchoHandler.hpp"
#include <utility>

namespace acs::conn {

std::size_t TcpConnection::_MESSAGE_DESCR_SIZE = 0;

/**
 * \todo return socket's port number (opt. concat timestamp) ?
 */
std::size_t TcpConnection::_generateConnId() {
    static std::size_t nextId = 0;
    return nextId++;
}

// template <typename ClientHandlerT>
// TcpConnection::TcpConnection(asio::io_context &ioContext, ConnectionStateListener &observer, util::Identity<ClientHandlerT> handlerType)
//     : _socket(ioContext), _id(generateConnId()), _handler(std::make_unique<typename decltype(handlerType)::type>(*this)), _observer(observer) {}

void TcpConnection::start() {
    _handler->handleStart();
    _startReceive();
}

void TcpConnection::close() {
    _handler->handleServerClose();
    _observer.connectionClosed(*this);
}

void TcpConnection::send(const std::string &message) {
    //TODO only when write not locked (i.e. in progress) <- use AsyncWriter
    _sendMsg = message;
    _doSend();
}

void TcpConnection::send(std::string &&message) {
    //TODO only when write not locked (i.e. in progress) <- use AsyncWriter
    _sendMsg = std::move(message);
    _doSend();
}

void TcpConnection::_handleWrite(const std::error_code &error, [[maybe_unused]] std::size_t bytesSend) {
    _writeInProgress = false;
    if (error) {
        //TODO
        util::Logger::instance().logError()
            << "Connection::_handleWrite err: " << error << std::endl;
    } else {
        util::Logger::instance().log() << "handleWrite" << std::endl;
    }
    _handler->handleSendComplete();
    // close();
}

void TcpConnection::_handleRead(const std::string &inputMessageData) {
    //TODO move deserialization to AsyncReader (or some AsyncReader-decorator/adaptor)
    auto message = _protocol.deserialize(inputMessageData.data(), inputMessageData.size());
    message::EchoHandler::handleServer(*message.get(), *this);
}

void TcpConnection::_startReceive() {
    _reader.readAsyncProtoInfOccupy([this](std::string data) {
            _handleRead(std::move(data));
        },
        &_protocol
    );
}

void TcpConnection::_doSend(/*WritePolicy*/) {
    if (_writeInProgress) {
        //TODO execute policy / according to policy
        util::Logger::instance().logError()
            << "Connection::doSend err: Write operation initialized when another is in progress" << std::endl;
        return;
    }

    _writeInProgress = true;
    asio::async_write(_socket, asio::buffer(_sendMsg), [this](auto&&... params) {
        _handleWrite(std::forward<decltype(params)>(params)...);
    });
}

} // namespace acs::conn