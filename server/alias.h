//
// Created by YangGuang on 2019-03-26.
//

#ifndef CAMPUS_CHAT_ALIAS_H
#define CAMPUS_CHAT_ALIAS_H
//#include "base/threading/browser_thread.h"
//#include "base/threading/browser_process_sub_thread.h"

namespace sun {
class BrowserThread;
class BrowserProcessSubThread;
}   // namespace sun

namespace footbook {
using CampusChatThread = sun::BrowserThread;
using CampusChatSubThread = sun::BrowserProcessSubThread;
}   // namespace footbook

#endif //CAMPUS_CHAT_ALIAS_H
