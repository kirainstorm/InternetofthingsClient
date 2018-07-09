#ifndef _broadcaset_search_ddd_tttt234324sdvxvxc2341
#define _broadcaset_search_ddd_tttt234324sdvxvxc2341

#include "PlatformDefine.h"
#include "syscmdinfo.h"

#pragma pack(1)

//当用户发起搜索时发送XSEARCH_MESSAGE_HEAD，设备返回SEARCH_MESSAGE_HEAD+SEARCH_MESSAGE
#define XSEARCH_MESSAGE_HEAD rqst_head
#define XSEARCH_MESSAGE_T _uuid_t

#pragma pack()



#ifdef _PLATFORM_WINDOW
typedef void xxxxsearch_callback(const char *ip, const XSEARCH_MESSAGE_T  *msg, void * pUser);
#endif


#endif // !_broadcaset_search_tttt






















