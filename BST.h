#ifndef BST_H
#define BST_H

#include <iostream>

#include "BSTNode.h"

enum TraversalPlan
{
    preorder,
    inorder,
    postorder
};

template <class T>
class BST
{
public: // DO NOT CHANGE THIS PART.
    BST();
    BST(const BST<T> &obj);

    ~BST();

    BSTNode<T> *getRoot() const;
    bool isEmpty() const;
    bool contains(BSTNode<T> *node) const;

    void insert(const T &data);

    void remove(const T &data);
    void removeAllNodes();

    BSTNode<T> *search(const T &data) const;
    BSTNode<T> *getSuccessor(BSTNode<T> *node, TraversalPlan tp) const;

    void print(TraversalPlan tp = inorder) const;

    BST<T> &operator=(const BST<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(BSTNode<T> *node, TraversalPlan tp) const;
    void copyRecursively(BSTNode<T> *node);
    void HELPERremoveAllNodes(BSTNode<T> *node);
    bool HELPERcontains(BSTNode<T> *node, BSTNode<T> *root) const;
    BSTNode<T> *getSuccessorInorder(BSTNode<T> *root, int &counter, BSTNode<T> *aranan) const;
    BSTNode<T> *getSuccessorPreorder(BSTNode<T> *root, int &counter, BSTNode<T> *aranan) const;
    BSTNode<T> *getSuccessorPostorder(BSTNode<T> *root, int &counter, BSTNode<T> *aranan) const;

private: // DO NOT CHANGE THIS PART.
    BSTNode<T> *root;
};

#endif // BST_H

template <class T>
BST<T>::BST()
{
    root = NULL;
}

template <class T>
void BST<T>::copyRecursively(BSTNode<T> *node)
{
    if (node)
        insert(node->data);
    if (node->left)
        copyRecursively(node->left);
    if (node->right)
        copyRecursively(node->right);
}

template <class T>
BST<T>::BST(const BST<T> &obj)
{
    root = NULL;
    copyRecursively(obj.getRoot());
}

template <class T>
BST<T>::~BST()
{
    if (root)
        removeAllNodes();
    root = NULL;
}

template <class T>
BSTNode<T> *BST<T>::getRoot() const
{
    return root;
}

template <class T>
bool BST<T>::isEmpty() const
{
    return root == NULL;
}

template <class T>
bool BST<T>::contains(BSTNode<T> *node) const
{
    return HELPERcontains(node, root);
}

template <class T>
bool BST<T>::HELPERcontains(BSTNode<T> *node, BSTNode<T> *root) const
{
    if (root == node)
        return true;
    if (root && root->left != NULL)
    {
        if (HELPERcontains(node, root->left))
            return true;
    };
    if (root && root->right != NULL)
    {
        if (HELPERcontains(node, root->right))
            return true;
    };
    return false;
}

template <class T>
void BST<T>::insert(const T &data)
{
    BSTNode<T> *temp = root;
    BSTNode<T> *parent = NULL;
    while (temp != NULL)
    {
        parent = temp;
        if (data > temp->data)
            temp = temp->right;
        else if (data < temp->data)
            temp = temp->left;
        else if (data == temp->data)
            return;
    }
    if (parent == NULL)
    {
        root = new BSTNode<T>(data, NULL, NULL);
    }
    else
    {
        temp = new BSTNode<T>(data, NULL, NULL);
        if (parent->data > temp->data)
            parent->left = temp;
        else
            parent->right = temp;
    }
}

template <class T>
void BST<T>::remove(const T &data)
{
    if (search(data) == NULL)
        return;

    BSTNode<T> *delete_this = root;
    BSTNode<T> *parent = NULL;

    while (data != delete_this->data)
    {
        parent = delete_this;
        if (data > delete_this->data)
            delete_this = delete_this->right;
        else if (data < delete_this->data)
            delete_this = delete_this->left;
    }

    if (delete_this->left == NULL && delete_this->right == NULL)
    {
        if (parent != NULL)
        {
            if (parent->right == delete_this)
                parent->right = NULL;
            else if (parent->left == delete_this)
                parent->left = NULL;
        }

        delete delete_this;

        if (parent == NULL)
            root = NULL;
    }

    else if (delete_this->left != NULL && delete_this->right == NULL)
    {
        if (parent == NULL)
        {
            root = delete_this->left;
            delete delete_this;
        }
        else
        {
            if (parent->left == delete_this)
            {
                parent->left = delete_this->left;
                delete delete_this;
            }
            if (parent->right == delete_this)
            {
                parent->right = delete_this->left;
                delete delete_this;
            }
        }
    }
    else if (delete_this->left == NULL && delete_this->right != NULL)
    {
        if (parent == NULL)
        {
            root = delete_this->right;
            delete delete_this;
        }
        else
        {
            if (parent->left == delete_this)
            {
                parent->left = delete_this->right;
                delete delete_this;
            }
            if (parent->right == delete_this)
            {
                parent->right = delete_this->right;
                delete delete_this;
            }
        }
    }

    else if (delete_this->left != NULL && delete_this->right != NULL)
    {
        BSTNode<T> *p_successor = delete_this->right;
        BSTNode<T> *parent_successor = NULL;
        while (p_successor->left != NULL)
        {
            parent_successor = p_successor;
            p_successor = p_successor->left;
        }

        if (parent == NULL)
        {
            root = p_successor;
            p_successor->left = delete_this->left;
            if (delete_this->right != p_successor)
            {
                if (p_successor->right && parent_successor)
                    parent_successor->left = p_successor->right;
                p_successor->right = delete_this->right;
            }
            if (parent_successor && parent_successor->left == p_successor)
                parent_successor->left = NULL;
            delete delete_this;
        }
        else
        {
            if (parent->left == delete_this)
                parent->left = p_successor;
            if (parent->right == delete_this)
                parent->right = p_successor;
            p_successor->left = delete_this->left;
            if (delete_this->right != p_successor)
            {
                if (p_successor->right && parent_successor)
                    parent_successor->left = p_successor->right;
                p_successor->right = delete_this->right;
            }
            if (parent_successor && parent_successor->left == p_successor)
                parent_successor->left = NULL;
            delete delete_this;
        }
    }
}

template <class T>
void BST<T>::removeAllNodes()
{
    HELPERremoveAllNodes(root);
    root = NULL;
}

template <class T>
void BST<T>::HELPERremoveAllNodes(BSTNode<T> *node)
{
    if (node == NULL)
        return;
    BSTNode<T> *left = node->left;
    BSTNode<T> *right = node->right;
    node->left = NULL;
    node->right = NULL;
    delete node;
    HELPERremoveAllNodes(left);
    HELPERremoveAllNodes(right);
}

template <class T>
BSTNode<T> *BST<T>::search(const T &data) const
{
    BSTNode<T> *traverser = root;

    while (traverser != NULL)
    {
        if (data > traverser->data)
            traverser = traverser->right;
        else if (data < traverser->data)
            traverser = traverser->left;
        else if (data == traverser->data)
            break;
    }

    return traverser;
}

template <class T>
BSTNode<T> *BST<T>::getSuccessor(BSTNode<T> *node, TraversalPlan tp) const
{
    int counter = 0;
    if (tp == inorder)
        return getSuccessorInorder(root, counter, node);
    else if (tp == preorder)
        return getSuccessorPreorder(root, counter, node);
    else if (tp == postorder)
        return getSuccessorPostorder(root, counter, node);
}

template <class T>
BSTNode<T> *BST<T>::getSuccessorInorder(BSTNode<T> *root, int &counter, BSTNode<T> *aranan) const
{
    BSTNode<T> *a;
    if (root == NULL)
        return NULL;
    if (a = getSuccessorInorder(root->left, counter, aranan))
        return a;
    if (counter == 1)
        return root;
    if (root == aranan)
        counter = 1;
    if (a = getSuccessorInorder(root->right, counter, aranan))
        return a;

    return NULL;
}

template <class T>
BSTNode<T> *BST<T>::getSuccessorPreorder(BSTNode<T> *root, int &counter, BSTNode<T> *aranan) const
{
    BSTNode<T> *a;
    if (root == NULL)
        return NULL;
    if (counter == 1)
        return root;
    if (root == aranan)
        counter = 1;
    if (a = getSuccessorPreorder(root->left, counter, aranan))
        return a;
    if (a = getSuccessorPreorder(root->right, counter, aranan))
        return a;

    return NULL;
}

template <class T>
BSTNode<T> *BST<T>::getSuccessorPostorder(BSTNode<T> *root, int &counter, BSTNode<T> *aranan) const
{
    BSTNode<T> *a;
    if (root == NULL)
        return NULL;
    if (a = getSuccessorPostorder(root->left, counter, aranan))
        return a;
    if (a = getSuccessorPostorder(root->right, counter, aranan))
        return a;
    if (counter == 1)
        return root;
    if (root == aranan)
        counter = 1;

    return NULL;
}

template <class T>
void BST<T>::print(TraversalPlan tp) const
{
    if (tp == inorder)
    {
        if (isEmpty())
        {
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }

        std::cout << "BST_inorder{" << std::endl;
        print(root, inorder);
        std::cout << std::endl
                  << "}" << std::endl;
    }
    else if (tp == preorder)
    {
        if (isEmpty())
        {
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }

        std::cout << "BST_preorder{" << std::endl;
        print(root, preorder);
        std::cout << std::endl
                  << "}" << std::endl;
    }
    else if (tp == postorder)
    {
        if (isEmpty())
        {
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }

        std::cout << "BST_postorder{" << std::endl;
        print(root, postorder);
        std::cout << std::endl
                  << "}" << std::endl;
    }
}

template <class T>
BST<T> &BST<T>::operator=(const BST<T> &rhs)
{
    if (this != &rhs)
    {
        removeAllNodes();
        copyRecursively(rhs.getRoot());
    }
    return *this;
}

template <class T>
void BST<T>::print(BSTNode<T> *node, TraversalPlan tp) const
{
    if (node == NULL)
        return;

    if (tp == inorder)
    {
        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left)
            std::cout << "," << std::endl;

        // then, output the node.
        std::cout << "\t" << node->data;

        if (node->right)
            std::cout << "," << std::endl;

        print(node->right, inorder);
    }
    else if (tp == preorder)
    {

        std::cout << "\t" << node->data;

        if (node->left)
            std::cout << "," << std::endl;
        print(node->left, preorder);

        if (node->right)
            std::cout << "," << std::endl;
        print(node->right, preorder);
    }
    else if (tp == postorder)
    {
        print(node->left, postorder);
        if (node->left)
            std::cout << "," << std::endl;

        print(node->right, postorder);
        if (node->right)
            std::cout << "," << std::endl;

        std::cout << "\t" << node->data;
    }
}