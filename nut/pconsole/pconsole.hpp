﻿/**
 * @file -
 * @author jingqi
 * @date 2012-12-10
 * @last-edit 2012-12-10 21:34:35 jingqi
 * @brief
 */

#ifndef ___HEADFILE_823DB076_2993_4B27_A90B_0072994B4D37_
#define ___HEADFILE_823DB076_2993_4B27_A90B_0072994B4D37_

#include <assert.h>
#include <string>
#include <vector>

#include <nut/gc/gc.hpp>
#include <nut/util/string/stringutil.hpp>

#include "icommand.hpp"
#include "iconsole.hpp"
#include "cmdexit.hpp"
#include "cmdhelp.hpp"

namespace nut
{

/**
 * Pseudo console
 */
class PConsole : public IConsole
{
    char m_promptChar;
    std::string m_promptString;
    std::string m_consoleName;
    std::vector<ref<ICommand> > m_commands;
    bool m_exit;
    int m_exit_value;

public:
    PConsole(const std::string& name = "PConsole")
        : m_promptChar('>'), m_promptString("PConsole"), m_consoleName(name),
          m_exit(false), m_exit_value(0)
    {
        // 内建命令
        addACommand(gc_new<CmdExit>(this));
        addACommand(gc_new<CmdHelp>(this));
    }

    char getPromptChar() const { return m_promptChar; }
    void setPromptChar(char c) { m_promptChar = c; }
    const std::string& getPromptString() const { return m_promptString; }
    void setPromptString(const std::string& s) { m_promptString = s; }
    

    /** 添加一个命令 */
    void addACommand(ref<ICommand> cmd)
    {
        m_commands.push_back(cmd);
    }

    /** 读取并执行一次 */
    void readAndExecute()
    {
        // 打印命令提示符
        printf("%s%c ", m_promptString.c_str(), m_promptChar);

        // 从命令行读取命令并执行
        std::string l;
        char c = '\0';
        while (true)
        {
            const char c = getchar();
            if ('\n' == c)
                break;
            else
                l += c;
        }
        executeLine(l.c_str());
    }

    /** 执行直到用户输入exit命令 */
    int execute()
    {
        m_exit = false;
        m_exit_value = 0;
        while (!m_exit)
            readAndExecute();
        return m_exit_value;
    }

    /** 打印一般信息 */
    virtual void printGeneralInfo() const
    {
        printf("%s\n", m_consoleName.c_str());
        for (size_t i = 0, size = m_commands.size(); i < size; ++i)
        {
            ref<ICommand> cmd = m_commands.at(i);
            assert(!cmd.isNull());
            const char* cmd_name = cmd->getCommandName();
            const char* cmd_info = cmd->getGeneralInfo();
            printf("\t%s %s\n", (NULL == cmd_name ? "(null)" : cmd_name), (NULL == cmd_info ? "" : cmd_info));
        }
    }

    /** 获取命令列表 */
    virtual const std::vector<ref<ICommand> >& getCommands() const
    {
        return m_commands;
    }

    /** 设置退出标记 */
    virtual void exit(int e)
    {
        m_exit_value = e;
        m_exit = true;
    }

private:
    void executeLine(const char* l)
    {
        assert(NULL != l);
        // 首先取出命令名
        size_t start = 0;
        while (l[start] != '\0' && (l[start] == ' ' || l[start] == '\t'))
            ++start;
        size_t end = start;
        while (l[end] != '\0' && l[end] != ' ')
            ++end;
        if (start == end)
            return;
        const std::string cmd_name(l + start, l + end);

        // 匹配命令
        std::vector<ref<ICommand> > matched_cmds = matchCommands(m_commands, cmd_name);

        // 无匹配
        if (matched_cmds.size() == 0)
        {
            printf("\nERROR: command not found, \"%s\"\n", cmd_name.c_str());
            return;
        }

        // 匹配数多于1
        if (matched_cmds.size() > 1)
        {
            printf("\nMore than one command matched:\n\t");
            for (size_t i = 0, size = matched_cmds.size(); i < size; ++i)
            {
                const char *n = matched_cmds.at(i)->getCommandName();
                printf(" %s", (NULL == n ? "(null)" : n));
            }
            return;
        }

        // 执行
        matched_cmds.at(0)->execute(l);
    }
};

}

#endif