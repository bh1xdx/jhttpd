

#ifndef __JHTTPD_CONFIG_H__
#define __JHTTPD_CONFIG_H__

#ifdef WIN32
#define JHTTPD_SERVER_WROOT             "C:/tmp"
#else
#define JHTTPD_SERVER_WROOT             "/tmp"
#endif
#define JHTTPD_SERVER_INDEX             "index.html"
#define JHTTPD_SERVER_PORT              80
#define JHTTPD_SERVER_NAME              "jhttpd"


#define JHTTPD_VERSION_MAJOR            0
#define JHTTPD_VERSION_MINOR            0
#define JHTTPD_BUGSFIX                  0
#define JHTTPD_RELEASENO                0
#define JHTTPD_REVISION                 0x0


#define JHTTPD_SERVER_TX_BUF_SIZE       0x100000
#define JHTTPD_SERVER_BACKLOG           SOMAXCONN
#define JHTTPD_CONNECTION_TIMEOUT       120 //secs

#endif // __JHTTPD_CONFIG_H__


