/**
 * @file -
 * @author jingqi
 * @date 2011-11-11 18:49
 */

#ifndef ___HEADFILE___B77B6DC2_6C30_4732_943B_117AF7E35B83_
#define ___HEADFILE___B77B6DC2_6C30_4732_943B_117AF7E35B83_

#include <stdint.h>
#include <stdlib.h>
#include <new.h>

#include <nut/debugging/destroychecker.hpp>

#include "refcounter.hpp"
#include "ref.hpp"

namespace nut
{

/**
 * �����ü��������װ��
 *
 * @note
 * ʹ�ð�װ������װ�ĺô���
 * 1. ���������Ĳ��ּ�gc_new��note
 * 2. ��������Ƕ�̳еģ���������Ȼֻ��һ�ݣ���Ȼ����������
 */
template <typename T, typename COUNTER = RefCounter>
class GCWrapper : public T
{
protected:
	/** ���������� */
	typedef void (*destroyer)(GCWrapper<T,COUNTER>*);

	/** ���ü��� */
	COUNTER m_counter;
	/** ������ */
	destroyer m_destroyer;

	/** ���������ٵļ���� */
#ifndef NDEBUG
	DestroyChecker m_checker;
#endif

public:
	GCWrapper(int c, destroyer d) : T(), m_counter(c), m_destroyer(d) {}

	template <typename Arg1>
	GCWrapper(int c, destroyer d, Arg1 arg1) : T(arg1), m_counter(c), m_destroyer(d) {}

	template <typename Arg1, typename Arg2>
	GCWrapper(int c, destroyer d, Arg1 arg1, Arg2 arg2) : T(arg1, arg2), m_counter(c), m_destroyer(d) {}

	template <typename Arg1, typename Arg2, typename Arg3>
	GCWrapper(int c, destroyer d, Arg1 arg1, Arg2 arg2, Arg3 arg3) : T(arg1, arg2, arg3), m_counter(c), m_destroyer(d) {}

	virtual void add_ref()
	{
#ifndef NDEBUG
		m_checker.checkDestroy();
#endif
		++m_counter;
	}

	virtual void rls_ref()
	{
#ifndef NDEBUG
		m_checker.checkDestroy();
#endif
		if (--m_counter == 0)
			m_destroyer(this);
	}
};

/**
 * �����ü�������������
 */
template <typename T, typename COUNTER = RefCounter>
class gc_new : public ref<typename RefTraits<T>::plain_type>
{
	typedef typename RefTraits<T>::plain_type plain_type;
	typedef GCWrapper<plain_type, COUNTER> wrapper_type;

	using weak_ref<plain_type>::m_ptr;

	static inline wrapper_type* alloc_wrapper()
	{
		return (wrapper_type*)::malloc(sizeof(wrapper_type));
	}

    /**
     * @note
     * �ô����ʵ�ֵĺô���
     * 1. ��֤�ڴ��������ٵĴ�����ͬһģ��
     * 2. ��ʹ����û�����������������Ҳ������������
     */
	static void destroy_wrapper(wrapper_type *p)
	{
		p->~wrapper_type();
		::free(p);
	}

public:
	gc_new()
	{
		m_ptr = alloc_wrapper();
		new (m_ptr) wrapper_type(1, destroy_wrapper);
	}

	template <typename Arg1>
	gc_new(Arg1 arg1)
	{
		m_ptr = alloc_wrapper();
		new (m_ptr) wrapper_type(1, destroy_wrapper, arg1);
	}

	template <typename Arg1, typename Arg2>
	gc_new(Arg1 arg1, Arg2 arg2)
	{
		m_ptr = alloc_wrapper();
		new (m_ptr) wrapper_type(1, destroy_wrapper, arg1, arg2);
	}

	template <typename Arg1, typename Arg2, typename Arg3>
	gc_new(Arg1 arg1, Arg2 arg2, Arg3 arg3)
	{
		m_ptr = alloc_wrapper();
		new (m_ptr) wrapper_type(1, destroy_wrapper, arg1, arg2, arg3);
	}
};

/**
 * ���������ü���
 */
#define DECLARE_GC_ENABLE \
	virtual void add_ref() = 0; \
	virtual void rls_ref() = 0; \
	template <typename ___T> friend class nut::ref;

}

#endif /* head file guarder */
