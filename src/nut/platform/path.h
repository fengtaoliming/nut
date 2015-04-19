﻿
#ifndef ___HEADFILE_E6D40B10_E5D6_4092_A38B_4E69E5B8E123_
#define ___HEADFILE_E6D40B10_E5D6_4092_A38B_4E69E5B8E123_

#include <string>

namespace nut
{

class Path
{
private:
    Path(); // invalid

public:
    /**
     * 路径分隔符
     */
    static char seperator();
    static wchar_t wseperator();

    /**
     * 检查字符是否是路径分隔符 '\\' '/'
     */
    static bool is_path_separator(char c);
    static bool is_path_separator(wchar_t c);

    static void get_cwd(std::string *appended);
    static void get_cwd(std::wstring *appended);
    static std::string get_cwd();
    static std::wstring get_wcwd();

    static bool is_abs(const std::string& p);
    static bool is_abs(const std::wstring& p);

    static void abs_path(const std::string& p, std::string *appended);
    static void abs_path(const std::wstring& p, std::wstring *appended);
    static std::string abs_path(const std::string& p);
    static std::wstring abs_path(const std::wstring& p);

    // TODO static std::string relative_path() {}
    // TODO static std::string real_path() {}

    /**
     * 从路径中划分出父路径和 文件/文件夹 名
     *
     * 例如：
     * "ab/c/d" -> "ab/c" "d"
     * "/ab.txt" -> "/" "ab.txt"
     * "c:\\tmp" -> "c:\\" "tmp"
     */
    static void split(const std::string &path, std::string *parent_appended, std::string *child_appended);
    static void split(const std::wstring& path, std::wstring *parent_appended, std::wstring *child_appended);

    /**
     * 从路径中划分出磁盘号和路径(linux路径的磁盘号假定为"")
     *
     * 例如：
     * "c:\\mn\\p" -> "c:" "\\mn\\p"
     * "/mnt/sdcard" -> "" "/mnt/sdcard"
     */
    static void split_drive(const std::string& path, std::string *drive_appended, std::string *rest_appended);
    static void split_drive(const std::wstring& path, std::wstring *drive_appended, std::wstring *rest_appended);

    /**
     * 从路径或者文件名中分离出后缀名
     *
     * 例如：
     * "a.txt" -> "a" ".txt"
     */
    static void split_ext(const std::string& path, std::string *prefix_appended, std::string *ext_appended);
    static void split_ext(const std::wstring& path, std::wstring *prefix_appended, std::wstring *ext_appended);

    // TODO static void splitunc() {}

    /**
     * 检查路径是否存在
     */
    static bool exists(const char *path);
    static bool exists(const std::string& path);
    static bool exists(const wchar_t *path);
    static bool exists(const std::wstring& path);

    /**
     * last access time
     */
    static time_t getatime(const char *path);
    static time_t getatime(const std::string& path);
    static time_t getatime(const wchar_t *path);
    static time_t getatime(const std::wstring& path);

    /**
     * last modified time
     */
    static time_t getmtime(const char *path);
    static time_t getmtime(const std::string& path);
    static time_t getmtime(const wchar_t *path);
    static time_t getmtime(const std::wstring& path);

    /**
     * created time
     */
    static time_t getctime(const char *path);
    static time_t getctime(const std::string& path);
    static time_t getctime(const wchar_t *path);
    static time_t getctime(const std::wstring& path);

    /**
     * 获取文件大小
     */
    static long getsize(const char *path);
    static long getsize(const std::string& path);
    static long getsize(const wchar_t *path);
    static long getsize(const std::wstring& path);

    static bool isdir(const char *path);
    static bool isdir(const std::string& path);
    static bool isdir(const wchar_t *path);
    static bool isdir(const std::wstring& path);

    static bool isfile(const char *path);
    static bool isfile(const std::string& path);
    static bool isfile(const wchar_t *path);
    static bool isfile(const std::wstring& path);

    static bool islink(const char *path);
    static bool islink(const std::string& path);
    static bool islink(const wchar_t *path);
    static bool islink(const std::wstring& path);

    // TODO static bool ismount() {}

    /**
     * 连接两个子路径
     *
     * 例如：
     * "a" "b" -> "a/b"
     * "/" "sd" -> "/sd"
     * "c:" "\\tmp" -> "c:\\tmp"
     */
    static void join(const std::string& a, const std::string& b, std::string *appended);
    static void join(const std::wstring& a, const std::wstring& b, std::wstring *appended);

    static std::string join(const std::string& a, const std::string& b);
    static std::wstring join(const std::wstring& a, const std::wstring& b);
    static std::string join(const std::string& a, const std::string& b, const std::string& c);
    static std::wstring join(const std::wstring& a, const std::wstring& b, const std::wstring& c);
    static std::string join(const std::string& a, const std::string& b, const std::string& c, const std::string& d);
    static std::wstring join(const std::wstring& a, const std::wstring& b, const std::wstring& c, const std::wstring& d);
};

}

#endif