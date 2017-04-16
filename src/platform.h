#ifndef PLATFORM_H
#define PLATFORM_H
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


///vfs标记
#define RAPIDLOGGERGGER_OPEN_READONLY         0x00000001  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_READWRITE        0x00000002  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_CREATE           0x00000004  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_DELETEONCLOSE    0x00000008  /* VFS only */
#define RAPIDLOGGER_OPEN_EXCLUSIVE        0x00000010  /* VFS only */
#define RAPIDLOGGER_OPEN_AUTOPROXY        0x00000020  /* VFS only */
#define RAPIDLOGGER_OPEN_URI              0x00000040  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_MEMORY           0x00000080  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_MAIN_DB          0x00000100  /* VFS only */
#define RAPIDLOGGER_OPEN_TEMP_DB          0x00000200  /* VFS only */
#define RAPIDLOGGER_OPEN_TRANSIENT_DB     0x00000400  /* VFS only */
#define RAPIDLOGGER_OPEN_MAIN_JOURNAL     0x00000800  /* VFS only */
#define RAPIDLOGGER_OPEN_TEMP_JOURNAL     0x00001000  /* VFS only */
#define RAPIDLOGGER_OPEN_SUBJOURNAL       0x00002000  /* VFS only */
#define RAPIDLOGGER_OPEN_MASTER_JOURNAL   0x00004000  /* VFS only */
#define RAPIDLOGGER_OPEN_NOMUTEX          0x00008000  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_FULLMUTEX        0x00010000  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_SHAREDCACHE      0x00020000  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_PRIVATECACHE     0x00040000  /* Ok for sqlite3_open_v2() */
#define RAPIDLOGGER_OPEN_WAL              0x00080000  /* VFS only */
#define RAPIDLOGGER_OPEN_APPEND     0x00100000  /* Append data to a file atomicly */

///unused variables
#define UNUSED(x) (void)x


#endif // PLATFORM_H
