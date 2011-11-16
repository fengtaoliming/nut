/**
 * @file -
 * @author jingqi
 * @date 2011-11-11 18:49
 */

#ifndef ___HEADFILE___9698630E_2BC3_42BB_91C2_734500687C95_
#define ___HEADFILE___9698630E_2BC3_42BB_91C2_734500687C95_

namespace nut
{

/**
 * ������
 */
template <typename T>
class weak_ref
{
protected:
    T *m_ptr;

public:
    /** Ĭ�Ϲ��캯�� */
    weak_ref() : m_ptr(NULL) {}

    weak_ref(T *ptr) : m_ptr(ptr) {}

    /** ��ʽ����ת�� */
    template <typename U>
    weak_ref(const weak_ref<U>& r) : m_ptr(r.pointer()) {}

    /** @note ���ƹ��캯�����������������ĺ��� */
    weak_ref(const weak_ref<T>& r) : m_ptr(r.m_ptr) {}

    ~weak_ref() { m_ptr = NULL; }

public:
    template <typename U>
    inline weak_ref<T>& operator= (const weak_ref<U>& r)
    {
        m_ptr = r.pointer();
        return *this;
    }

    inline weak_ref<T>& operator= (const weak_ref<T>& r)
    {
        m_ptr = r.m_ptr;
        return *this;
    }

    inline bool operator== (const weak_ref<T>& r) const { return m_ptr == r.m_ptr; }
    inline bool operator!= (const weak_ref<T>& r) const { return m_ptr != r.m_ptr; }

    T* operator-> () const { return m_ptr; }
    T& operator* () const { return *m_ptr; }

public:
    inline bool isNull() const { return m_ptr == NULL; }
    inline T* pointer() const { return m_ptr; }
};

/**
 * ǿ����
 */
template <typename T>
class ref : public weak_ref<T>
{
protected:
    using weak_ref<T>::m_ptr;

    void assign(T *p)
    {
        if (p != NULL)
            p->add_ref();
        if (m_ptr != NULL)
            m_ptr->rls_ref();
        m_ptr = p;
    }

    void clear()
    {
        if (m_ptr != NULL)
        {
            m_ptr->rls_ref();
            m_ptr = NULL;
        }
    }

public:
    ref() {}

    ref(T *p) { assign(p); }

    /** ��ʽ����ת�� */
    template <typename U>
    ref(const weak_ref<U>& r) { assign(r.pointer()); }

    /** @note ���ƹ��캯�����������������ĺ��� */
    ref(const ref<T>& r) { assign(r.m_ptr); }

    ~ref() { clear(); }

public:
    template <typename U>
    ref<T>& operator= (const weak_ref<U>& r)
    {
        assign(r.pointer());
        return *this;
    }

    ref<T>& operator= (const ref<T>& r)
    {
        assign(r.m_ptr);
        return *this;
    }
};

/* ******************************************************************/

template <typename T>
struct RefTraits
{
    typedef T plain_type;
};

template <typename T>
struct RefTraits<weak_ref<T> >
{
    typedef typename RefTraits<T>::plain_type plain_type;
};

template <typename T>
struct RefTraits<ref<T> >
{
    typedef typename RefTraits<T>::plain_type plain_type;
};

/* ******************************************************************/

template <typename T>
struct static_ref_cast : public ref<typename RefTraits<T>::plain_type>
{
    template <typename U>
    static_ref_cast(const weak_ref<U>& r)
    {
        assign(static_cast<typename RefTraits<T>::plain_type*>(r.pointer()));
    }
};

template <typename T>
struct dynamic_ref_cast : public ref<typename RefTraits<T>::plain_type>
{
    template <typename U>
    dynamic_ref_cast(const weak_ref<U>& r)
    {
        assign(dynamic_cast<typename RefTraits<T>::plain_type*>(r.pointer()));
    }
};

}

#endif /* head file guarder */
