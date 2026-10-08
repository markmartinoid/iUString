#pragma once

#ifndef _ICODES_H
#define _ICODES_H


//65000 utf-7 Unicode (UTF-7) 
//65001 utf-8 

#ifndef CP_UTF7
#define CP_UTF7                   65000       // UTF-7 translation
#endif
#define CP_UTF7_ "UTF-7"
#define CP_UTF7_m "utf-7"
#ifndef CP_UTF8
#define CP_UTF8                   65001       // UTF-8 translation
#endif
#define CP_UTF8_ "UTF-8"
#define CP_UTF8_m "utf-8"
#ifndef CP_UTF16
#define CP_UTF16                   12000      // little endian byte order (BMP of ISO 10646);  
#endif
#define CP_UTF16_ "UTF-16"
#define CP_UTF16_m "utf-16"
#ifndef CP_UTF16be
#define CP_UTF16be                   12001      // big endian byte order (BMP of ISO 10646);  
#endif
#define CP_UTF16_ "UTF-16"
#define CP_UTF16_m "utf-16"



#ifndef CP_W1251
#define CP_W1251                   1251      
#endif
#define CP_W1251_ "WINDOWS-1251"
#define CP_W1251_m "windows-1251"

#ifndef CP_W1250
#define CP_W1250                   1250      
#endif
#define CP_W1250_ "WINDOWS-1250"
#define CP_W1250_m "windows-1250"

#ifndef CP_W1252
#define CP_W1252                   1250      
#endif
#define CP_W1252_ "WINDOWS-1250"
#define CP_W1252_m "windows-1250"






#define UL_ERROR 0xFFFFFFFF


#define IC_MIN  0xFE000000
//general
#define IC_GENERAL_MIN  0xFE000001


//ERRORS
#define IC_ERROR  0xFE00E000
#define IC_ERROR_INDEX_OUT_OF_BOUNDS  0xFE00E001
#define IC_ERROR_PARAMETER_NOT_SET    0xFE00E002
#define IC_ERROR_PATH                 0xFE00E003
#define IC_BUFFER_TO_SMALL            0xFE00E004
#define IC_OUT_OF_MEMORY              0xFE00E005
#define IC_ERROR_PARAMETER_ERROR      0xFE00E006
#define IC_ERROR_AUTHENTICATION       0xFE00E007




 #define IC_GENERAL_MAX  0xFE00FFFF
//end general


//INET
#define INET_MIN                     0xFE010001
#define IC_ERROR_DNS                 0xFE010002
#define IC_ERROR_CONNECT             0xFE010003
#define IC_ERROR_HEADER              0xFE010004
#define IC_ERROR_ZLIB                0xFE010005
#define IC_ERROR_NO_DNS_SERVERS      0xFE010006
#define IC_ERROR_NO_INIT             0xFE010007
#define IC_ERROR_SOCKET              0xFE010008
#define IC_TO_MANY_REDIRECTS         0xFE010009
#define IC_TO_MANY_RECV_CALL         0xFE01000A
#define IC_SMTP_NO_MESSAGE           0xFE01000B
#define IC_ERROR_SEND                0xFE01000C
#define IC_ERROR_MAKE_SMTP_HEADER    0xFE01000D





#define INET_MAX  0xFE01FFFF
//end INET




#define IC_MAX  0xFE0FFFFF





char* IC_ErrorDescription(uint32_t Code)
{
switch (Code)
{
case(IC_ERROR):return "IC_ERROR";
case(IC_ERROR_INDEX_OUT_OF_BOUNDS):return "IC_ERROR_INDEX_OUT_OF_BOUNDS";
case(IC_ERROR_PARAMETER_NOT_SET):return "IC_ERROR_PARAMETER_NOT_SET";
case(IC_ERROR_PATH):return "IC_ERROR_PATH";
case(IC_BUFFER_TO_SMALL):return "IC_BUFFER_TO_SMALL";
case(IC_OUT_OF_MEMORY):return "IC_OUT_OF_MEMORY";
case(IC_ERROR_PARAMETER_ERROR):return "IC_ERROR_PARAMETER_ERROR";
case(IC_ERROR_AUTHENTICATION):return "IC_ERROR_AUTHENTICATION";


case(IC_ERROR_DNS):return "IC_ERROR_DNS";
case(IC_ERROR_CONNECT):return "IC_ERROR_CONNECT";
case(IC_ERROR_HEADER):return "IC_ERROR_HEADER";
case(IC_ERROR_ZLIB):return "IC_ERROR_ZLIB";
case(IC_ERROR_NO_DNS_SERVERS):return "IC_ERROR_NO_DNS_SERVERS";
case(IC_ERROR_NO_INIT):return "IC_ERROR_NO_INIT";
case(IC_ERROR_SOCKET):return "IC_ERROR_SOCKET";
case(IC_TO_MANY_REDIRECTS):return "IC_TO_MANY_REDIRECTS";
case(IC_TO_MANY_RECV_CALL):return "IC_TO_MANY_RECV_CALL";
case(IC_SMTP_NO_MESSAGE):return "IC_SMTP_NO_MESSAGE";
case(IC_ERROR_SEND):return "IC_ERROR_SEND";
case(IC_ERROR_MAKE_SMTP_HEADER):return "IC_ERROR_MAKE_SMTP_HEADER";

}
return NULL;
}














#endif  //_ICODES_H
















