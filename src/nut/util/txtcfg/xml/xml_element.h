﻿
#ifndef ___HEADFILE_B694AC90_3B03_460A_A7B6_AAE8EF5A3560_
#define ___HEADFILE_B694AC90_3B03_460A_A7B6_AAE8EF5A3560_

#include <assert.h>
#include <vector>
#include <string>
#include <map>

#include <nut/rc/rc_new.h>

#include "xml_writer.h"

namespace nut
{

class XmlElement
{
    NUT_REF_COUNTABLE

    class Comment
    {
    public:
        size_t pos;
        std::string text;

        Comment()
            : pos(0)
        {}

        Comment(size_t _pos, const std::string& _text)
            : pos(_pos), text(_text)
        {}
    };

    std::string m_name, m_text;
    typedef std::map<std::string, std::string> attr_map_t;
    attr_map_t m_attrs;
    std::vector<rc_ptr<XmlElement> > m_children;
    std::vector<Comment> m_comments; // sorted ascending
    bool m_dirty;

public:
    typedef attr_map_t::iterator attr_iter_t;
    typedef attr_map_t::const_iterator const_attr_iter_t;

public:
    XmlElement();

    XmlElement(const std::string& name);

    bool is_dirty() const;

    void set_dirty(bool dirty);

    const std::string& get_name() const
    {
        return m_name;
    }

    void set_name(const std::string& name);

    const std::string& get_text() const
    {
        return m_text;
    }

    void set_text(const std::string& text);

    size_t get_children_count() const
    {
        return m_children.size();
    }

    rc_ptr<XmlElement> get_child(size_t i) const;

    rc_ptr<XmlElement> get_child(const std::string& name) const;

    void append_child(rc_ptr<XmlElement> child);

    void insert_child(size_t pos, rc_ptr<XmlElement> child);

    void remove_child(size_t pos);

    void clear_children();

    /**
     * 获取属性
     *
     * @param attr 用来存储返回的属性值，可以为 NULL
     * @param 改属性是否存在
     */
    bool get_attribute(const std::string& name, std::string *attr) const;

    /**
     * 添加属性
     *
     * @return 是否成功，如果属性已经存在，则添加失败
     */
    bool add_attribute(const std::string& name, const std::string& value);

    /**
     * 存在属性则设置属性值，否则添加新属性
     */
    void set_attribute(const std::string& name, const std::string& value);

    bool remove_attribute(const std::string& name);

    void clear_attributes();

    void add_comment(size_t pos, const std::string& text);
    void remove_comment(size_t pos);

    void clear();

    const_attr_iter_t attr_const_begin() const;
    const_attr_iter_t attr_const_end() const;

    attr_iter_t attr_begin();
    attr_iter_t attr_end();

    void parse(const std::string& s, size_t start_index = 0, bool ignore_text_blank = true);

    /*
     * @param format 格式化输出，以便于阅读
     */
    void serielize(std::string *appended, bool format = true) const;

    void serielize(XmlWriter &writer, int tab) const;
};

}

#endif