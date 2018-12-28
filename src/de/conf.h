#ifndef __DE_CONF_H__
#define __DE_CONF_H__
// #include "libs.h"

//debug
#define DE_DEBUG 1 // DE_LOG and DE_LOGE macros will not be ignored

// input
#define DE_FINGER_MAX 12

//shader tmp
// #ifdef ANDROID /*opengl es*/
#define DE_SHD_HEADERV "#version 100\n""precision highp float;\n"
#define DE_SHD_HEADERF DE_SHD_HEADERV
// #define DE_SHD_HEADERF "#version 100\n""precision highp float;\n"
// #else
// #define DE_SHD_HEADERV "#version 120\n"
// #define DE_SHD_HEADERF "#version 120\n"
// #endif



// assets temp
#ifndef ANDROID
#define DE_ASSET_DIR "assets/"
#else
#define DE_ASSET_DIR ""
#endif
#define DE_ASSET(x) DE_ASSET_DIR x



#endif
