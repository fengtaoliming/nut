/**
 * @file -
 * @author jingqi
 * @date 2012-03-02
 * @last-edit 2012-03-02 21:21:56 jingqi
 */

#ifndef ___HEADFILE_CFEF4EB8_082C_417F_A58C_9D65C9F3DAD1_
#define ___HEADFILE_CFEF4EB8_082C_417F_A58C_9D65C9F3DAD1_

#include "btree.hpp"

namespace nut
{

/**
 * ���������
 */
template <typename K, typename NODE>
class BSTree
{
public:

    /**
     * �����������ڵĽڵ�
     *
     * @return
     *      û���ҵ��򷵻� NULL
     */
    static NODE* search(NODE *sub_root, const K& key)
    {
        while (NULL != sub_root)
        {
            if (key < sub_root->getKey())
                sub_root = sub_root->getLeftChild();
            else if (sub_root->getKey() < key)
                sub_root = sub_root->getRightChild();
            else
                return sub_root;
        }
        return NULL;
    }

    /**
     * �����½ڵ㵽���������
     *
     * @return �µĸ�
     */
    static NODE* insert(NODE *root, NODE *new_node)
    {
        assert(NULL != new_node);
        NODE *parent = NULL;
        bool insertToLeft = true;
        for (NODE *x = root; NULL != x; )
        {
            parent = x;
            if (new_node->getKey() < x->getKey())
            {
                x = x->getLeftChild();
                insertToLeft = true;
            }
            else
            {
                x = x->getRightChild();
                insertToLeft = false;
            }
        }

        new_node->setParent(parent);
        if (NULL == parent)
            root = new_node;
        else if (insertToLeft)
            parent->setLeftChild(new_node);
        else
            parent->setRightChild(new_node);
        return root;
    }

    /**
     * �Ӷ����������ɾ�����нڵ�
     */
    static NODE* remove(NODE *root, NODE *to_be_del)
    {
        assert(NULL != to_be_del);
        NODE *escaper = NULL;
        if (NULL == to_be_del->getLeftChild() || NULL == to_be_del->getRightChild())
            escaper = to_be_del;
        else
            escaper = successor(to_be_del);

        NODE *sublink = NULL;
        if (NULL != escaper->getLeftChild())
            sublink = escaper->getLeftChild();
        else
            sublink = escaper->getRightChild();
        assert(NULL != sublink);

        NODE *sublink_parent = escaper->getParent();
        if (NULL != sublink)
            sublink->setParent(sublink_parent);

        if (NULL == sublink_parent)
            root = sublink;
        else if (escaper == sublink_parent->getLeftChild())
            sublink_parent->setLeftChild(sublink);
        else
            sublink_parent->setRightChild(sublink);

        if (escaper != to_be_del)
        {
            escaper->setParent(to_be_del->getParent());
            escaper->setLeftChild(to_be_del->getLeftChild());
            escaper->setRightChild(to_be_del->getRightChild());
            if (NULL == to_be_del->getParent())
                root = escaper;
            else if (to_be_del == to_be_del->getParent()->getLeftChild())
                x->getParent()->setLeftChild(escaper);
            else
                x->getParent()->setRightChild(escaper);
        }
        return root;
    }

    /**
     * �ҵ���С�������ڵĽڵ�
     */
    static NODE* minimum(NODE *sub_root)
    {
        assert(NULL != sub_root);
        while (NULL != sub_root->getLeftChild())
            sub_root = sub_root->getLeftChild();
        return sub_root;
    }
    
    /**
     * �ҵ�����������ڵĽڵ�
     */
    static NODE* maximum(NODE *sub_root)
    {
        assert(NULL != sub_root);
        while (NULL != sub_root->getRightChild())
            sub_root = sub_root->getRightChild();
        return sub_root;
    }

    /**
     * �������ݴ�С˳���ҵ���һ�����
     */
    static NODE* successor(NODE *x)
    {
        assert(NULL != x);
        if (NULL != x->getRightChild())
            return minimum(x->getRightChild());
        NODE *parent = x->getParent();
        while (NULL != parent && x == parent->getRightChild())
        {
            x = parent;
            parent = x->getParent();
        }
        return parent;
    }
    
    /**
     * �������ݴ�С˳���ҵ���һ�����
     */
    static NODE* predecessor(NODE *x)
    {
        assert(NULL != x);
        if (NULL != x->getLeftChild())
            return maximum(x->getLeftChild());
        NODE *parent = x->getParent();
        while (NULL != parent && x == parent->getLeftChild())
        {
            x = parent;
            parent = x->getParent();
        }
        return parent;
    }
};

}

#endif /* head file guarder */

