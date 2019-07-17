#ifndef ACS_MESSAGE_HPP__
#define ACS_MESSAGE_HPP__

#include <cstdint>
#include <string>

namespace acs::message {

/// Abstract base class (interface) for concrete Messages.
/**
 * Concrete Message types implement their handle() methods,
 * which the receiver of given message executes on reception.
 */
class Message {
public:
    using TypeId = std::uint32_t;
    using ContextId = std::size_t;

public:
    // Non-virtual public interface
    void handle(ContextId contextId) { // inline
        _doHandle(contextId);
    }

    void serializeAppendToString(std::string *appendOutput) const { // inline
        _doSerializeAppendToString(*appendOutput);
    }

    virtual TypeId getTypeId() const noexcept = 0;
    virtual std::size_t getByteSize() const = 0;

    virtual ~Message() = default;

private:
    virtual void _doHandle(ContextId contextId) = 0;
    virtual void _doSerializeAppendToString(std::string &str) const = 0;
};

//TODO?
struct Context {
    //? Sender &sender;
    //...
};

} // namespace acs::message

#endif // ACS_MESSAGE_HPP__
