#pragma once

#ifndef _INETCODES_H
#define _INETCODES_H

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////   WININET   //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#ifndef _WININET_
#define INTERNET_INVALID_PORT_NUMBER    0           // use the protocol-specific default
#define INTERNET_DEFAULT_FTP_PORT       21          // default for FTP servers
#define INTERNET_DEFAULT_GOPHER_PORT    70          //    "     "  gopher "
#define INTERNET_DEFAULT_HTTP_PORT      80          //    "     "  HTTP   "
#define INTERNET_DEFAULT_HTTPS_PORT     443         //    "     "  HTTPS  "
#define INTERNET_DEFAULT_SOCKS_PORT     1080        // default for SOCKS firewall servers.


 //
// maximum field lengths (arbitrary)
//

#define INTERNET_MAX_HOST_NAME_LENGTH   256
#define INTERNET_MAX_USER_NAME_LENGTH   128
#define INTERNET_MAX_PASSWORD_LENGTH    128
#define INTERNET_MAX_PORT_NUMBER_LENGTH 5           // INTERNET_PORT is unsigned short
#define INTERNET_MAX_PORT_NUMBER_VALUE  65535       // maximum unsigned short value
#define INTERNET_MAX_PATH_LENGTH        2048
#define INTERNET_MAX_SCHEME_LENGTH      32          // longest protocol name length
#define INTERNET_MAX_URL_LENGTH         (INTERNET_MAX_SCHEME_LENGTH \
                                        + sizeof("://") \
                                        + INTERNET_MAX_PATH_LENGTH)


#endif // #ifndef _WININET_
///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////   WININET   //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//#define INET_MAX_RECV_CALL_COUNT 
#define INET_MAX_REDIRECT_COUNT   10UL


#define INET_FTP 21
#define INET_HTTP 80
#define INET_HTTPS 443
#define INET_MAIL 110

#define INET_FTP_PREFIX  "ftp"
#define INET_FTP_PREFIX_  "ftp://"
#define INET_HTTP_PREFIX  "http"
#define INET_HTTP_PREFIX_  "http://"
#define INET_HTTPS_PREFIX  "https"
#define INET_HTTPS_PREFIX_  "https://"
#define INET_MAILTO_PREFIX  "mailto"
#define INET_MAILTO_PREFIX_  "mailto:"




#define iEOF "\r\n"
#define iEOFr (char)13
#define iEOFn (char)10


#define INET_HEADER_GET  0x00000001
#define INET_HEADER_POST 0x00000002

#define INET_BROVSER_CHROME_537  0x00010537
#define INET_BROVSER_IE_7        0x00020700
#define INET_BROVSER_FIREFOX_3   0x00030300



#define INET_RESPONCECODE_200_OK  200UL 
#define INET_RESPONCECODE_301_REDIRECT  301UL 
#define INET_RESPONCECODE_NONE      0UL

#define INET_CONTENT_ENCODING_NONE       0x00000001
#define INET_CONTENT_ENCODING_OTHER      0x00000002
#define INET_CONTENT_ENCODING_CHUNKED    0x00000004
#define INET_CONTENT_ENCODING_DEFLATE    0x00000010     
#define INET_CONTENT_ENCODING_GZIP       0x00000020
//#define INET_CONTENT_ENCODING_SDCH     0x00000100
// gzip" is the gzip format, and "deflate" is the zlib format.

#define INET_CONTENT_TYPE_TEXTHTML       0x00000001
#define INET_CONTENT_TYPE_TEXTXML        0x00000002
#define INET_CONTENT_TYPE_MULTIPART_BINARY 0x00000010
#define INET_CONTENT_TYPE_OTHER          0x0000F000


#define INET_CONNECTION_CLOSE            0x00000001
#define INET_CONNECTION_KEEP_ALIVE       0x00000002

#define INET_CHUNK_CHAR_MAX              7UL




#endif  //_INETCODES_H
















