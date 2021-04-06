#if 0   // isatty() function (platform independent)
- executable        => isatty(STDOUT_FILENO)!=0
- executable >nul   => isatty(STDOUT_FILENO)!=0
- executable >file  => isatty(STDOUT_FILENO)==0
- executable |other => isatty(STDOUT_FILENO)==0
#endif
#pragma once

#if defined(_MSC_VER)           //======== Microsoft Visual Studio  ========


#include <io.h>
constexpr auto STDIN_FILENO  = 0;   //fileno(stdin)
constexpr auto STDOUT_FILENO = 1;   //fileno(stdout)
constexpr auto STDERR_FILENO = 2;   //fileno(stderr)

#define isatty _isatty  //to avoid compiler warning/error


#elif defined(__GNUG__)         //======== Linux GCC                ========


#include <unistd.h> //isatty()


#endif