#pragma once

#include "shared/logger/logger.h"

namespace alog {

class CatchSaver : public Saver
{
public:
    CatchSaver();
    void flushImpl(const MessageList&) override;
};

} // namespace alog

#define ALOG_FLUSH() \
   {alog::logger().flush(); \
    alog::logger().waitingFlush();}
