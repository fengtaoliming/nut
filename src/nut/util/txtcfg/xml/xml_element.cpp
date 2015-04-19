﻿
#include <assert.h>

#include <nut/util/string/string_util.h>

#include "xml_element.h"
#include "xml_parser.h"

namespace nut
{

XmlElement::XmlElement()
    : m_dirty(false)
{}

XmlElement::XmlElement(const std::string& name)
    : m_name(name), m_dirty(false)
{}

bool XmlElement::is_dirty() const
{
    if (m_dirty)
        return true;
    for (size_t i = 0, sz = m_children.size(); i < sz; ++i)
    {
        if (m_children.at(i)->is_dirty())
            return true;
    }
    return false;
}

void XmlElement::set_dirty(bool dirty)
{
    if (dirty)
    {
        m_dirty = true;
        return;
    }

    m_dirty = false;
    for (size_t i = 0, sz = m_children.size(); i < sz; ++i)
        m_children.at(i)->set_dirty(false);
}

void XmlElement::set_name(const std::string& name)
{
    if (name != m_name)
    {
        m_name = name;
        m_dirty = true;
    }
}

void XmlElement::set_text(const std::string& text)
{
    if (text != m_text)
    {
        m_text = text;
        m_dirty = true;
    }
}

rc_ptr<XmlElement> XmlElement::get_child(size_t i) const
{
    if (i >= m_children.size())
        return rc_ptr<XmlElement>();
    return m_children.at(i);
}

rc_ptr<XmlElement> XmlElement::get_child(const std::string& name) const
{
    for (size_t i = 0, s = m_children.size(); i < s; ++i)
    {
        rc_ptr<XmlElement> c = m_children.at(i);
        if (c.is_null())
            continue;
        if (c->m_name == name)
            return c;
    }
    return rc_ptr<XmlElement>();
}

void XmlElement::append_child(rc_ptr<XmlElement> child)
{
    assert(child.is_not_null());
    m_children.push_back(child);
    m_dirty = true;
}

void XmlElement::insert_child(size_t pos, rc_ptr<XmlElement> child)
{
    assert(pos <= m_children.size() && child.is_not_null());
    m_children.insert(m_children.begin() + pos, child);
    m_dirty = true;
}

void XmlElement::remove_child(size_t pos)
{
    assert(pos < m_children.size());
    m_children.erase(m_children.begin() + pos);
    m_dirty = true;
}

void XmlElement::clear_children()
{
    m_children.clear();
    m_dirty = true;
}

/**
 * 获取属性
 *
 * @param attr 用来存储返回的属性值，可以为 NULL
 * @param 改属性是否存在
 */
bool XmlElement::get_attribute(const std::string& name, std::string *attr) const
{
    const_attr_iter_t iter = m_attrs.find(name);
    if (iter == m_attrs.end())
        return false;
    if (NULL != attr)
        *attr = iter->second;
    return true;
}

/**
 * 添加属性
 *
 * @return 是否成功，如果属性已经存在，则添加失败
 */
bool XmlElement::add_attribute(const std::string& name, const std::string& value)
{
    if (m_attrs.find(name) != m_attrs.end())
        return false;
    m_attrs.insert(std::pair<std::string,std::string>(name, value));
    m_dirty = true;
    return true;
}

/**
 * 存在属性则设置属性值，否则添加新属性
 */
void XmlElement::set_attribute(const std::string& name, const std::string& value)
{
    m_attrs[name] = value;
    m_dirty = true;
}

bool XmlElement::remove_attribute(const std::string& name)
{
    attr_iter_t iter = m_attrs.find(name);
    if (iter == m_attrs.end())
        return false;
    m_attrs.erase(iter);
    m_dirty = true;
    return true;
}

void XmlElement::clear_attributes()
{
    m_attrs.clear();
    m_dirty = true;
}

void XmlElement::add_comment(size_t pos, const std::string& text)
{
    // binary search
    int left = -1, right = (int) m_comments.size();
    while (left + 1 < right)
    {
        const int mid = (left + right) / 2;
        if (m_comments.at(mid).pos <= pos)
            left = mid;
        else
            right = mid;
    }
    m_comments.insert(m_comments.begin() + right, Comment(pos, text));
}

void XmlElement::remove_comment(size_t pos)
{
    // binary search
    const int size = (int) m_comments.size();
    int left = -1, right = size;
    while (left + 1 < right)
    {
        const int mid = (left + right) / 2;
        if (m_comments.at(mid).pos < pos)
        {
            left = mid;
        }
        else if (m_comments.at(mid).pos > pos)
        {
            right = mid;
        }
        else
        {
            // found
            left = mid;
            while (left > 0 && m_comments.at(left - 1).pos == pos)
                --left;
            right = mid + 1;
            while (right < size && m_comments.at(right).pos == pos)
                ++right;
            m_comments.erase(m_comments.begin() + left, m_comments.begin() + right);
            return;
        }
    }
}

void XmlElement::clear()
{
    m_name.clear();
    m_text.clear();
    m_attrs.clear();
    m_children.clear();
    m_comments.clear();
    m_dirty = true;
}

XmlElement::const_attr_iter_t XmlElement::attr_const_begin() const
{
    return m_attrs.begin();
}

XmlElement::const_attr_iter_t XmlElement::attr_const_end() const
{
    return m_attrs.end();
}

XmlElement::attr_iter_t XmlElement::attr_begin()
{
    m_dirty = true; // in case of modification
    return m_attrs.begin();
}

XmlElement::attr_iter_t XmlElement::attr_end()
{
    m_dirty = true; // in case of modification
    return m_attrs.end();
}

void XmlElement::parse(const std::string& s, size_t start_index, bool ignore_text_blank)
{
    assert(start_index <= s.length());

    class Handler : public XmlElementHandler
    {
        XmlElement *m_elem;
        bool m_ignore_text_blank;
    public:
        Handler(XmlElement *e, bool ignore_text_blank)
            : m_elem(e), m_ignore_text_blank(ignore_text_blank)
        {}

        virtual void handle_attribute(const std::string &name, const std::string &value)
        {
            m_elem->add_attribute(name, value);
        }

        virtual void handle_text(const std::string& text)
        {
            m_elem->m_text += text;
        }

        virtual void handle_comment(const std::string& comment)
        {
            m_elem->add_comment(m_elem->m_children.size(), comment);
        }

        virtual XmlElementHandler* handle_child(const std::string &name)
        {
            rc_ptr<XmlElement> c = rc_new<XmlElement>(name);
            m_elem->append_child(c);
            return new Handler(c.pointer(), m_ignore_text_blank);
        }

        virtual void handle_child_finish(XmlElementHandler *child)
        {
            delete child;
        }

        virtual void handle_finish()
        {
            if (m_ignore_text_blank)
                m_elem->m_text = trim(m_elem->m_text);
        }
    };

    clear();
    Handler h(this, ignore_text_blank);
    XmlParser p(&h);
    p.input(s.c_str() + start_index, s.length() - start_index);
    p.finish();
}

/*
 * @param format 格式化输出，以便于阅读
 */
void XmlElement::serielize(std::string *appended, bool format) const
{
    assert(NULL != appended);
    StdStringWriter sw(appended);
    XmlWriter w(&sw);
    serielize(w, format ? 0 : -1);
}

void XmlElement::serielize(XmlWriter &writer, int tab) const
{
    // name
    for (int i = 0; i < tab; ++i)
        writer.write_text("\t");
    writer.start_element(m_name.c_str());

    // attributes
    for (const_attr_iter_t iter = m_attrs.begin(), end = m_attrs.end();
        iter != end; ++iter)
    {
        writer.write_attribute(iter->first.c_str(), iter->second.c_str());
    }

    // text
    bool has_child = false;
    if (!m_text.empty())
    {
        if (tab >= 0)
        {
            const std::string text = trim(m_text);
            if (!text.empty())
            {
                writer.write_text("\n");
                for (int i = 0; i < tab + 1; ++i)
                    writer.write_text("\t");
                writer.write_text(text.c_str());
                has_child = true;
            }
        }
        else
        {
            writer.write_text(m_text.c_str());
            has_child = true;
        }
    }

    // children and comments
    const size_t comments_size = m_comments.size();
    size_t comment_pos = 0;
    for (size_t i = 0, csize = m_children.size(); i < csize; ++i)
    {
        // serialize comment
        while (comment_pos < comments_size && m_comments.at(comment_pos).pos <= i)
        {
            if (tab >= 0)
            {
                writer.write_text("\n");
                for (int j = 0; j < tab + 1; ++j)
                    writer.write_text("\t");
            }
            writer.write_comment(m_comments.at(comment_pos).text.c_str());
            ++comment_pos;
            has_child = true;
        }

        // children element
        rc_ptr<XmlElement> c = m_children.at(i);
        if (c.is_null())
            continue;
        if (tab >= 0)
            writer.write_text("\n");
        c->serielize(writer, tab >= 0 ? tab + 1 : tab);
        has_child = true;
    }

    // tail comments
    while (comment_pos < comments_size)
    {
        if (tab >= 0)
        {
            writer.write_text("\n");
            for (int j = 0; j < tab + 1; ++j)
                writer.write_text("\t");
        }
        writer.write_comment(m_comments.at(comment_pos).text.c_str());
        ++comment_pos;
        has_child = true;
    }

    // finish
    if (has_child && tab >= 0)
    {
        writer.write_text("\n");
        for (int i = 0; i < tab; ++i)
            writer.write_text("\t");
    }
    writer.end_element();
}

}