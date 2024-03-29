#include <gtest/gtest.h>
#include "acs/util/Logger.hpp"
#include <sstream>

using namespace acs::util;

namespace {

class CommonLoggerTest : public testing::Test {
protected:
    std::ostringstream _ostream;

    /**
     * \todo propagate assertion failure to the caller
     */
    inline void checkStreamDataEqualTo(const std::string &expected) {
        EXPECT_EQ(_ostream.str(), expected) << "Stream data is not equal to the expected string data";
    }
};

namespace dummy {

struct OutputDummy {};
std::ostream& operator<<(std::ostream &out, const OutputDummy&) {
    return out << "dummy object";
}

} // namespace dummy

TEST_F(CommonLoggerTest, OutputsCorrectStringMessage) {
    std::string toLog = "a\tb. c  \n\ndef\n8";
    std::string expectedOutput = toLog + '\n';

    log(toLog, _ostream);

    checkStreamDataEqualTo(expectedOutput);
}

TEST_F(CommonLoggerTest, OutputsCorrectStringMessageUsingBLSOperator) {
    std::string expectedOutput = "abc\t120.01dummy objecta\n nara\n";

    log(_ostream) << "abc\t" << 12 << 0.01 << dummy::OutputDummy{} << 'a' << '\n' << " nara" << std::endl;

    checkStreamDataEqualTo(expectedOutput);
}

} // namespace