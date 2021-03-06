﻿
#ifndef ___HEADFILE_B81E878E_513C_4792_A8F8_73215B57ACBE_
#define ___HEADFILE_B81E878E_513C_4792_A8F8_73215B57ACBE_

#include <assert.h>
#include <stdlib.h>
#include <map>

#include <nut/threading/sync/spinlock.h>
#include <nut/threading/sync/guard.h>

namespace nut
{

/**
 * most-recently-used data cache
 */
template <typename K>
class LRUDataCache
{
    struct Node
    {
        K key;
        void *data = nullptr;
        size_t size = 0;
        Node *pre = nullptr, *next = nullptr;

        Node(const K& k, const void *buf, size_t cb)
            : key(k), size(cb)
        {
            assert(nullptr != buf || 0 == cb);
            if (cb > 0)
            {
                data = ::malloc(cb);
                assert(nullptr != data);
                ::memcpy(data, buf, cb);
            }
        }

        ~Node()
        {
            clear();
        }

        void copy_from(const void *buf, size_t cb)
        {
            assert(nullptr != buf || 0 == cb);
            clear();
            if (cb > 0)
            {
                data = ::malloc(cb);
                assert(nullptr != data);
                ::memcpy(data, buf, cb);
                size = cb;
            }
        }

        void fast_set(void *buf, size_t cb)
        {
            assert(nullptr != buf || 0 == cb);
            clear();
            data = buf;
            size = cb;
        }

        void clear()
        {
            if (nullptr != data)
                ::free(data);
            data = nullptr;
            size = 0;
        }

    private:
        // Non-copyable
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };

    enum { DEFAULT_BYTES_CAPACITY = 5 * 1024 * 1024 }; // 单位: 字节
    typedef std::map<K,Node*> map_type;

    size_t _bytes_size = 0, _bytes_capacity = 0;
    map_type _map;
    Node *_list_head = nullptr, *_list_end = nullptr;
    SpinLock _lock; // 注意，linux下自旋锁不可重入

#ifndef NDEBUG
    size_t _hit_count = 0, _hit_size = 0, _miss_count = 0;
#endif

    static Node* alloc_node()
    {
        return (Node*)::malloc(sizeof(Node));
    }

    static void dealloc_node(Node *p)
    {
        assert(nullptr != p);
        ::free(p);
    }

    static Node* new_node(const K& k, const void *buf, size_t cb)
    {
        Node *p = alloc_node();
        assert(nullptr != p);
        new (p) Node(k, buf, cb);
        return p;
    }

    static void delete_node(Node *p)
    {
        assert(nullptr != p);
        p->~Node();
        dealloc_node(p);
    }

    void remove_from_list(Node *p)
    {
        assert(nullptr != p);
        if (nullptr != p->pre)
            p->pre->next = p->next;
        else
            _list_head = p->next;

        if (nullptr != p->next)
            p->next->pre = p->pre;
        else
            _list_end = p->pre;
    }

    void push_list_head(Node *p)
    {
        assert(nullptr != p);
        p->next = _list_head;
        p->pre = nullptr;
        if (nullptr != _list_head)
            _list_head->pre = p;
        else
            _list_end = p;
        _list_head = p;
    }

private:
    // Non-copyable
    LRUDataCache(const LRUDataCache<K>&) = delete;
    LRUDataCache<K>& operator=(const LRUDataCache<K>&) = delete;

public:
    LRUDataCache()
        : _bytes_capacity(DEFAULT_BYTES_CAPACITY)
    {}

    LRUDataCache(size_t bytes_capacity)
        : _bytes_capacity(bytes_capacity)
    {
        assert(bytes_capacity > 0);
    }

    ~LRUDataCache()
    {
        clear();
    }

    size_t size() const
    {
        return _map.size();
    }

    size_t bytes_size() const
    {
        return _bytes_size;
    }

    size_t bytes_capacity() const
    {
        return _bytes_capacity;
    }

    void set_bytes_capacity(size_t bytes_capacity)
    {
        assert(bytes_capacity > 0);
        _bytes_capacity = bytes_capacity;
    }

    void put(const K& k, const void *buf, size_t cb)
    {
        assert(nullptr != buf || 0 == cb);
        Guard<SpinLock> g(&_lock);

        typename map_type::const_iterator const n = _map.find(k);
        if (n == _map.end())
        {
            Node *const p = new_node(k, buf, cb);
            _map.insert(std::pair<K,Node*>(k,p));
            push_list_head(p);
            _bytes_size += cb;

            while (_bytes_size > _bytes_capacity)
            {
                assert(nullptr != _list_end);
                typename map_type::iterator const nn = _map.find(_list_end->key);
                assert(nn != _map.end());
                Node *const pp = nn->second;
                assert(nullptr != pp && _bytes_size >= pp->size);
                _bytes_size -= pp->size;
                _map.erase(nn);
                remove_from_list(pp);
                delete_node(pp);
            }
            return;
        }

        Node *const p = n->second;
        assert(nullptr != p && _bytes_size >= p->size);
        _bytes_size -= p->size;
        p->copy_from(buf, cb);
        remove_from_list(p);
        push_list_head(p);
        _bytes_size += cb;
    }

    void remove(const K& k)
    {
        Guard<SpinLock> g(&_lock);

        typename map_type::iterator const n = _map.find(k);
        if (n == _map.end())
            return;

        Node *const p = n->second;
        assert(nullptr != p && _bytes_size >= p->size);
        _bytes_size -= p->size;
        _map.erase(n);
        remove_from_list(p);
        delete_node(p);
    }

    bool has_key(const K& k)
    {
        return _map.find(k) != _map.end();
    }

    bool get(const K& k, const void **pdata, size_t *psize)
    {
        assert(nullptr != pdata && nullptr != psize);
        Guard<SpinLock> g(&_lock);

        typename map_type::const_iterator const n = _map.find(k);
        if (n == _map.end())
        {
#ifndef NDEBUG
            ++_miss_count;
#endif
            return false;
        }

        Node *const p = n->second;
        assert(nullptr != p);
        *pdata = p->data;
        *psize = p->size;
        remove_from_list(p);
        push_list_head(p);

#ifndef NDEBUG
        ++_hit_count;
        _hit_size += p->size;
#endif
        return true;
    }

    void clear()
    {
        Guard<SpinLock> g(&_lock);

        Node *p = _list_head;
        while (nullptr != p)
        {
            Node *const n = p->next;
            delete_node(p);
            p = n;
        }
        _list_head = nullptr;
        _list_end = nullptr;
        _map.clear();
        _bytes_size = 0;

#ifndef NDEBUG
        _hit_count = 0;
        _hit_size = 0;
        _miss_count = 0;
#endif
    }
};


}

#endif
