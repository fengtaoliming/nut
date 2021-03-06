﻿
#ifndef ___HEADFILE___AD10542C_56CB_4BD3_B17A_1355A3FC0EF6_
#define ___HEADFILE___AD10542C_56CB_4BD3_B17A_1355A3FC0EF6_

#include <stdlib.h> // for nullptr

#include "../nut_config.h"


namespace nut
{

/**
 * 单元测试失败时所抛出的异常
 */
class NUT_API TestCaseFailureException
{
    const char *_description = nullptr;
    const char *_file = nullptr;
    int _line = -1;

public:
    TestCaseFailureException(const char *des, const char *file, int line);

    const char* get_description() const;

    const char* get_file() const;

    int get_line() const;
};

}

#endif /* head file guarder */
