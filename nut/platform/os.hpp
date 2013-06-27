﻿/**
 * @file -
 * @author jingqi
 * @date 2012-06-23
 * @last-edit 2012-11-13 21:28:38 jingqi
 */

#ifndef ___HEADFILE_291DFB4C_7D29_4D61_A691_EF83FB86CD36_
#define ___HEADFILE_291DFB4C_7D29_4D61_A691_EF83FB86CD36_

#include <assert.h>
#include <string>
#include <vector>
#include <stdio.h> // for sprintf()

#include "platform.hpp"

#if defined(NUT_PLATFORM_OS_WINDOWS)
#   include <windows.h>
#   include <io.h>    // for mkdir()
#else
#   include <dirent.h>  // for DIR, dirent
#   include <limits.h>   // for PATH_MAX
#   include <sys/stat.h> // for lstat()
#   include <sys/types.h>  // for mkdir()
#   include <unistd.h> // for rmdir()
#endif

#include <nut/util/string/stringutil.hpp>

namespace nut
{

class OS
{
private:
    OS() {}

public:

    /**
     * 枚举目录下的文件/文件夹
     *
     * @param except_file 如果传入true, 则返回值不会包含文件
     * @parma except_dir 如果传入true, 则返回值不会包含文件夹
     * @parma except_initial_dot 如果传入true, 则返回值不会包含以'.'开头的文件/文件夹
     */
    static std::vector<std::string> listdir(const char *path, bool except_file = false,
            bool except_dir = false, bool except_initial_dot = false)
    {
        assert(NULL != path);
        std::vector<std::string> ret;

#if defined(NUT_PLATFORM_OS_WINDOWS)
        char search_path[MAX_PATH];
        ::sprintf(search_path, "%s\\*", path); /* 加上通配符 */

        WIN32_FIND_DATAA wfd;
        const HANDLE hFind = ::FindFirstFileA(search_path, &wfd);
        if (hFind == INVALID_HANDLE_VALUE)
            return ret;

        do
        {
            if (except_initial_dot && '.' == wfd.cFileName[0])
                continue;
            if (except_file && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                continue;
            if (except_dir && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                continue;

            ret.push_back(wfd.cFileName);
        } while (::FindNextFileA(hFind, &wfd));

        // 关闭查找句柄
        ::FindClose(hFind);
        return ret;
#else
        DIR *dp = NULL;
        struct dirent *dirp = NULL;
        if ((dp = ::opendir(path)) == NULL)
            return ret;

        while ((dirp = ::readdir(dp)) != NULL)
        {
            if (except_initial_dot && dirp->d_name[0] == '.')
                continue;

            if (except_file || except_dir)
            {
                char file_path[PATH_MAX];
                ::sprintf(file_path, "%s/%s", path, dirp->d_name);
                struct stat buf;
                if (::lstat(file_path, &buf) < 0)
                    continue;
                if (except_file && !S_ISDIR(buf.st_mode))
                    continue;
                if (except_dir && S_ISDIR(buf.st_mode))
                    continue;
            }

            ret.push_back(dirp->d_name);
        }

        // 释放DIR (struct dirent是由DIR维护的，无需额外释放)
        ::closedir(dp);
        return ret;
#endif
    }

    static inline std::vector<std::string> listdir(const std::string& path, bool except_file = false,
            bool except_dir = false, bool except_initial_dot = false)
    {
        return listdir(path.c_str(), except_file, except_dir, except_initial_dot);
    }

    static std::vector<std::wstring> listdir(const wchar_t *path, bool except_file = false,
        bool except_dir = false, bool except_initial_dot = false)
    {
        assert(NULL != path);
        std::vector<std::wstring> ret;

#if defined(NUT_PLATFORM_OS_WINDOWS)
        wchar_t search_path[MAX_PATH];
        ::swprintf(search_path, L"%s\\*", path); /* 加上通配符 */

        WIN32_FIND_DATAW wfd;
        const HANDLE hFind = ::FindFirstFileW(search_path, &wfd);
        if (hFind == INVALID_HANDLE_VALUE)
            return ret;

        do
        {
            if (except_initial_dot && L'.' == wfd.cFileName[0])
                continue;
            if (except_file && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                continue;
            if (except_dir && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                continue;

            ret.push_back(wfd.cFileName);
        } while (::FindNextFileW(hFind, &wfd));

        // 关闭查找句柄
        ::FindClose(hFind);
        return ret;
#else
        const std::string p = wstr2str(path);
        const std::vector<std::string> dirs = listdir(p.c_str(), except_file, except_dir, except_initial_dot);
        for (int i = 0, size = dirs.size(); i < size; ++i)
            ret.push_back(str2wstr(dirs[i]));
        return ret;
#endif
    }

    static inline std::vector<std::wstring> listdir(const std::wstring& path, bool except_file = false,
        bool except_dir = false, bool except_initial_dot = false)
    {
        return listdir(path.c_str(), except_file, except_dir, except_initial_dot);
    }

    /**
     * 复制文件
     */
    static bool copyfile(const char *src, const char *dest)
    {
        assert(NULL != src && NULL != dest);

#if defined(NUT_PLATFORM_OS_WINDOWS)
        return FALSE != ::CopyFileA(src, dest, TRUE);
#else
        FILE *inFile = ::fopen(src, "rb");
        if (NULL == inFile)
            return false;
            
        FILE *outFile = ::fopen(dest, "wb+");
        if (NULL == outFile)
        {
            ::fclose(inFile);
            return false;
        }
        
        const int BUF_LEN = 4096;
        char buf[BUF_LEN];
        int readed = -1;
        while ((readed = ::fread(buf, 1, BUF_LEN, inFile)) > 0)
        {
            ::fwrite(buf, 1, readed, outFile);
        }
        ::fclose(inFile);
        ::fclose(outFile);
        return true;
#endif
    }

    static inline bool copyfile(const std::string& src, const std::string& dest) { return copyfile(src.c_str(), dest.c_str()); }

    static bool copyfile(const wchar_t *src, const wchar_t *dest)
    {
        assert(NULL != src && NULL != dest);

#if defined(NUT_PLATFORM_OS_WINDOWS)
        return FALSE != ::CopyFileW(src, dest, TRUE);
#else
        const std::string s = wstr2str(src), d = wstr2str(dest);
        return copyfile(s.c_str(), d.c_str());
#endif
    }

    static inline bool copyfile(const std::wstring& src, const std::wstring& dest) { return copyfile(src.c_str(), dest.c_str()); }

    static inline bool removefile(const char *path)
    {
        assert(NULL != path);
        return -1 != ::remove(path);
    }

    static inline bool removefile(const std::string& path) { return removefile(path.c_str()); }

    static inline bool removefile(const wchar_t *path)
    {
        assert(NULL != path);
#if defined(NUT_PLATFORM_OS_WINDOWS)
        return FALSE != ::DeleteFileW(path);
#else
        const std::string p = wstr2str(path);
        return removefile(p.c_str());
#endif
    }

    static inline bool removefile(const std::wstring& path) { return removefile(path.c_str()); }

    static inline bool mkdir(const char *path)
    {
        assert(NULL != path);
#if defined(NUT_PLATFORM_OS_WINDOWS)
        return FALSE != ::CreateDirectoryA(path, NULL);
#else
        return 0 == ::mkdir(path, S_IWRITE);
#endif
    }

    static inline bool mkdir(const std::string& path) { return mkdir(path.c_str()); }

    static inline bool mkdir(const wchar_t *path)
    {
        assert(NULL != path);
#if defined(NUT_PLATFORM_OS_WINDOWS)
        return FALSE != ::CreateDirectoryW(path, NULL);
#else
        const std::string p = wstr2str(path);
        return OS::mkdir(p.c_str());
#endif
    }

    static inline bool mkdir(const std::wstring& path) { return mkdir(path.c_str()); }

    /**
     * 删除空目录
     */
    static inline bool removedir(const char *path)
    {
        assert(NULL != path);
#if defined(NUT_PLATFORM_OS_WINDOWS)
        return FALSE != ::RemoveDirectoryA(path);
#else
        return 0 == ::rmdir(path);
#endif
    }

    static inline bool removedir(const std::string& path) { return removedir(path.c_str()); }

    static inline bool removedir(const wchar_t *path)
    {
        assert(NULL != path);
#if defined(NUT_PLATFORM_OS_WINDOWS)
        return FALSE != ::RemoveDirectoryW(path);
#else
        const std::string p = wstr2str(path);
        return removedir(p.c_str());
#endif
    }

    static inline bool removedir(const std::wstring& path) { return removedir(path.c_str()); }

    /**
     * 删除目录树
     */
    static bool removetree(const char *path)
    {
        assert(NULL != path);
#if defined(NUT_PLATFORM_OS_WINDOWS)
        // 删除文件
        if (0 == (FILE_ATTRIBUTE_DIRECTORY & ::GetFileAttributesA(path)))
            return FALSE != ::DeleteFileA(path);

        // 遍历文件夹
        char full_path[MAX_PATH];
        ::sprintf(full_path, "%s\\*", path); /* 加上通配符 */

        WIN32_FIND_DATAA wfd;
        const HANDLE hFind = ::FindFirstFileA(full_path, &wfd);
        if (hFind == INVALID_HANDLE_VALUE)
            return false;

        bool ret = true;
        do
        {
            // 忽略 . 和 ..
            if (('.' == wfd.cFileName[0] && '\0' == wfd.cFileName[1]) ||
                ('.' == wfd.cFileName[0] && '.' == wfd.cFileName[1] && '\0' == wfd.cFileName[2]))
                continue;

            ::sprintf(full_path, "%s\\%s", path, wfd.cFileName);
            ret = removetree(full_path);
        } while (ret && ::FindNextFileA(hFind, &wfd));

        // 关闭查找句柄
        ::FindClose(hFind);

        // 移除空文件夹
        if (ret)
            ret = (FALSE != ::RemoveDirectoryA(path));
        return ret;
#else
        struct stat info;
        if (0 != ::stat(path, &info))
            return false;

        // 删除文件
        if (!S_ISDIR(info.st_mode))
            return 0 == ::unlink(path); // 这里就不用 remove() 了

        // 遍历文件夹
        DIR *dp = NULL;
        struct dirent *dirp = NULL;
        if ((dp = ::opendir(path)) == NULL)
            return false;

        bool ret = true;
        char full_path[PATH_MAX];
        while (ret && (dirp = ::readdir(dp)) != NULL)
        {
            // 忽略 . 和 ..
            if (('.' == dirp->d_name[0] && '\0' == dirp->d_name[1]) ||
                ('.' == dirp->d_name[0] && '.' == dirp->d_name[1] && '\0' == dirp->d_name[2]))
                continue;

            ::sprintf(full_path, "%s/%s", path, dirp->d_name);
            ret = removetree(full_path);
        }

        // 释放DIR (struct dirent是由DIR维护的，无需额外释放)
        ::closedir(dp);

        // 删除空目录
        if (ret)
            ret = ::rmdir(path);
        return ret;
#endif
    }

    static bool removetree(const std::string& path) { return removetree(path.c_str()); }

    static bool removetree(const wchar_t *path)
    {
        assert(NULL != path);
#if defined(NUT_PLATFORM_OS_WINDOWS)
        // 删除文件
        if (0 == (FILE_ATTRIBUTE_DIRECTORY & ::GetFileAttributesW(path)))
            return FALSE != ::DeleteFileW(path);

        // 遍历文件夹
        wchar_t full_path[MAX_PATH];
        ::swprintf(full_path, L"%s\\*", path); /* 加上通配符 */

        WIN32_FIND_DATAW wfd;
        const HANDLE hFind = ::FindFirstFileW(full_path, &wfd);
        if (hFind == INVALID_HANDLE_VALUE)
            return false;

        bool ret = true;
        do
        {
            // 忽略 . 和 ..
            if ((L'.' == wfd.cFileName[0] && L'\0' == wfd.cFileName[1]) ||
                (L'.' == wfd.cFileName[0] && L'.' == wfd.cFileName[1] && L'\0' == wfd.cFileName[2]))
                continue;

            ::swprintf(full_path, L"%s\\%s", path, wfd.cFileName);
            ret = removetree(full_path);
        } while (ret && ::FindNextFileW(hFind, &wfd));

        // 关闭查找句柄
        ::FindClose(hFind);

        // 移除空文件夹
        if (ret)
            ret = (FALSE != ::RemoveDirectoryW(path));
        return ret;
#else
        const std::string p = wstr2str(path);
        return removetree(p.c_str());
#endif
    }

    static bool removetree(const std::wstring& path) { return removetree(path.c_str()); }
};

}

#endif

