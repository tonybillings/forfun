#include <jni.h>
#include <errno.h>

#include <cmath>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unistd.h>
#include <sys\resource.h>
#include <thread>
#include <atomic>

#include <EGL\egl.h>
#include <EGL\eglext.h>
#include <GLES3\gl32.h>

#include <android\sensor.h>

#include <android\log.h>
#include "android_native_app_glue.h"

#include <vmmlib\vector.hpp>
#include <vmmlib\matrix.hpp>
#include <vmmlib\frustum.hpp>

#include <asio/asio.hpp>

#include <bones/bones.hpp>
