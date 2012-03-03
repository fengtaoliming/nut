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
            if (key < sub_root->key())
                sub_root = sub_root->left();
            else if (sub_root->key() < key)
                sub_root = sub_root->right();
            else
                return sub_root;
        }
        return NULL;
    }

    static NODE* insert(NODE *root, NODE *new_node)
    {
        assert(NULL != new_node);
        NODE *parent = NULL;
        for (NODE *x = root; NULL != x; )
        {
            parent = x;
            if (new_node->key() < x->key())
                x = x->left();
            else
                x = x->right();
        }

        new_node->parent(parent);
        if (NULL == parent)
            return new_node;
        else if (new_node->key() < parent->key())
            parent->left(new_node);
        else
            parent->right(new_node);
        return root;
    }

    static NODE* remove(NODE *root, const K& key)
    {
        // TODO
    }

    /**
     * �ҵ���С�������ڵĽڵ�
     */
    static NODE* minimum(NODE *sub_root)
    {
        assert(NULL != sub_root);
        while (NULL != sub_root->left())
            sub_root = sub_root->left();
        return sub_root;
    }
    
    /**
     * �ҵ�����������ڵĽڵ�
     */
    static NODE* maximum(NODE *sub_root)
    {
        assert(NULL != sub_root);
        while (NULL != sub_root->right())
            sub_root = sub_root->right();
        return sub_root;
    }

    /**
     * �������ݴ�С˳���ҵ���һ�����
     */
    static NODE* successor(NODE *x)
    {
        assert(NULL != x);
        if (NULL != x->right())
            return minimum(x->right());
        NODE *parent = x->parent();
        while (NULL != parent && x == parent->right())
        {
            x = parent;
            parent = x->parent();
        }
        return parent;
    }
    
    /**
     * �������ݴ�С˳���ҵ���һ�����
     */
    static NODE* predecessor(NODE *x)
    {
        assert(NULL != x);
        if (NULL != x->left())
            return maximum(x->left());
        NODE *parent = x->parent();
        while (NULL != parent && x == parent->left())
        {
            x = parent;
            parent = x->parent();
        }
        return parent;
    }
};

}

#endif /* head file guarder */

