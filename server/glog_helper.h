/**
* @Author: YangGuang
* @Date:   2019-01-11
* @Email:  guang334419520@126.com
* @Filename: disk_util_win.h
* @Last modified by:  YangGuang
*/

#ifndef BASE_LOG_GLOG_HELPER_H
#define BASE_LOG_GLOG_HELPER_H

#include <glog/logging.h>

namespace footbook {

class GlogHelper final {
 public:
	 // 传入程序名.
	 explicit GlogHelper(const char* programe);
	 ~GlogHelper();
};

}	// namespace log

#endif // !BASE_LOG_GLOG_HELPER_H
