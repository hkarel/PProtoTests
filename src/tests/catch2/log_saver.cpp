#include "catch2/log_saver.h"
#include "catch2/catch_message.hpp"

namespace alog {

CatchSaver::CatchSaver() : Saver("catch2", Level::Debug2)
{}

void CatchSaver::flushImpl(const MessageList& messages)
{
    if (messages.size() == 0)
        return;

    removeIdsTimeoutThreads();

    vector<char> lineBuff;
    if (maxLineSize() > 0)
    {
        lineBuff.resize(maxLineSize() + 1);
        lineBuff[maxLineSize()] = '\0';
    }

    Filter::List filters = this->filters();

    for (Message* m : messages)
    {
        if (m->level > level())
            continue;

        if (skipMessage(*m, filters))
            continue;

        string buff = m->prefix3;

        string str;
        string* pstr = &m->str;
        if (m->something && m->something->canModifyMessage())
        {
            str = m->something->modifyMessage(m->str);
            pstr = &str;
        }
        if ((maxLineSize() > 0) && (maxLineSize() < int(pstr->size())))
        {
            strncpy(&lineBuff[0], pstr->c_str(), maxLineSize());
            buff +=  (char*) &lineBuff[0];
        }
        else
            buff += *pstr;

        UNSCOPED_INFO( "ALOG: " << buff );
    }
}

} // namespace alog
