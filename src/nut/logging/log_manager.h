﻿
#ifndef ___HEADFILE___6FFFC04E_FFB6_47F6_80AA_8FD1AF3B201F_
#define ___HEADFILE___6FFFC04E_FFB6_47F6_80AA_8FD1AF3B201F_

#include <nut/platform/platform.h>
#include <nut/util/txtcfg/property_dom.h>
#include <nut/util/txtcfg/txt_file.h>

#include "logger.h"

DLL_API void* nut_get_root_logger();

#define NUT_LOGGING_IMPL \
DLL_API void* nut_get_root_logger() \
{ \
    static nut::rc_ptr<nut::Logger> root; \
    if (root.is_null()) \
        root = nut::rc_new<nut::Logger>((nut::Logger*)NULL, ""); \
    return root.pointer(); \
}

namespace nut
{

class LogManager
{
    // make sure that no one can construct it
    LogManager();
    ~LogManager();

public:
    static Logger* get_logger(const std::string &path);

    static void load_config(rc_ptr<PropertyDom> config);
    static void load_config(const char *config_file);
};

}

#endif //head file guarder