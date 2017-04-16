#ifndef RAPIDLOGGER_PLATFORM_H
#define RAPIDLOGGER_PLATFORM_H
//判断系统
#ifdef __CYGWIN__
#define RAPIDLOGGER_OS_CYGWIN 1
#elif defined __SYMBIAN32__
# define RAPIDLOGGER_OS_SYMBIAN 1
#elif defined _WIN32
# define RAPIDLOGGER_OS_WINDOWS 1

#ifdef _WIN32_WCE
#define RAPIDLOGGER_OS_WINDOWS_MOBILE 1
#elif defined(__MINGW__) || defined(__MINGW32__)
#define RAPIDLOGGER_OS_WINDOWS_MINGW 1
#else
#define RAPIDLOGGER_OS_WINDOWS_DESKTOP 1
#endif  // _WIN32_WCE

#elif defined __APPLE__
#define RAPIDLOGGER_OS_MAC 1

#if TARGET_OS_IPHONE
#define RAPIDLOGGER_OS_IOS 1
#if TARGET_IPHONE_SIMULATOR
#define RAPIDLOGGER_OS_IOS_SIMULATOR 1
#endif

#endif

#elif defined __linux__
#define RAPIDLOGGER_OS_LINUX 1
#if defined __ANDROID__
#define RAPIDLOGGER_OS_LINUX_ANDROID 1
#endif
#elif defined __MVS__
#define RAPIDLOGGER_OS_ZOS 1
#elif defined(__sun) && defined(__SVR4)
#define RAPIDLOGGER_OS_SOLARIS 1
#elif defined(_AIX)
#define RAPIDLOGGER_OS_AIX 1
#elif defined(__hpux)
#define RAPIDLOGGER_OS_HPUX 1
#elif defined __native_client__
#define RAPIDLOGGER_OS_NACL 1
#elif defined __OpenBSD__
#define RAPIDLOGGER_OS_OPENBSD 1
#elif defined __QNX__
#define RAPIDLOGGER_OS_QNX 1
#endif  // __CYGWIN__

//判断环境是否完全支持c++11
#ifndef RAPIDLOGGER_LANG_CXX11
# if __GXX_EXPERIMENTAL_CXX0X__ || __cplusplus >= 201103L
// Compiling in at least C++11 mode.
#  define RAPIDLOGGER_LANG_CXX11 1
# else
#  define RAPIDLOGGER_LANG_CXX11 0
# endif
#endif

//判断系统是32位还是64位
#ifdef RAPIDLOGGER_OS_WINDOWS
#	ifdef _WIN64
#	define RAPIDLOGGER_OS_64
#	elif defined _WIN32
#	define RAPIDLOGGER_OS_32
#	endif
#endif // RAPIDLOGGER_OS_WINDOWS

///unused variables
#define UNUSED(x) (void)x


#endif // RAPIDLOGGER_PLATFORM_H
