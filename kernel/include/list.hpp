#pragma once
#include <stddef.h>

template <class T>
class List
{
  private:
    struct Node
    {
        T data;
        Node* prev;
        Node* next;

        Node(const T& d, Node* p = NULL, Node* n = NULL) : data(d), prev(p), next(n) {}
    };

    Node* frontNode = NULL;
    Node* backNode = NULL;
    size_t nodes = 0;

  public:
    class Iterator
    {
      private:
        Node* pos;

      public:
        Iterator(Node* n = NULL) : pos(n) {}
        Iterator(const Iterator& it) : pos(it.pos) {}

        Iterator& operator++()    { pos = pos->next; return *this; }
        Iterator& operator--()    { pos = pos->prev; return *this; }
        Iterator  operator++(int) { Iterator tmp(*this); operator++(); return tmp; }
        Iterator  operator--(int) { Iterator tmp(*this); operator--(); return tmp; }

        bool operator==(const Iterator& it) { return pos == it.pos; }
        bool operator!=(const Iterator& it) { return pos != it.pos; }

        T& operator*() { return pos->data; }

        friend class List;
    };

    List() {}
    List(const List& list)
    {
        for (auto el: list)
            push_back(el);
    }
    ~List() { clear(); }

    List& operator=(const List& list)
    {
        List tmp(list);
        swap(tmp);
        return *this;
    }

    void swap(List& list)
    {
        auto tFront = frontNode, tBack = backNode, tNodes = nodes;

        frontNode = list.frontNode; backNode = list.backNode; nodes = list.nodes ;
        list.frontNode = tFront   ; list.backNode = tBack   ; list.nodes = tNodes;
    }

    Iterator begin() { return Iterator(frontNode); }
    Iterator end()   { return Iterator(); }

    T& front() { return *begin(); }
    T& back()  { return *Iterator(backNode); }

    void push_front(const T& data)
    {
        if (frontNode == NULL)
        {
            frontNode = backNode = new Node(data);
            nodes++;
        }
        else
            insert(begin(), data);
    }

    void push_back(const T& data)
    {
        if (backNode == NULL)
            push_front(data);
        else
        {
            backNode->next = new Node(data, backNode);
            backNode = backNode->next;
            nodes++;
        }
    }

    void pop_front() { erase(begin()); }
    void pop_back()  { erase(Iterator(backNode)); }

    void insert(const Iterator& it, const T& data)
    {
        auto newNode = new Node(data, it.pos->prev, it.pos);

        if (it.pos->prev == NULL)
            frontNode = newNode;
        else
            it.pos->prev->next = newNode;

        it.pos->prev = newNode;
        nodes++;
    }

    void erase(const Iterator& it)
    {
        if (it.pos->prev == NULL)
            frontNode = it.pos->next;
        else
            it.pos->prev->next = it.pos->next;

        if (it.pos->next == NULL)
            backNode = it.pos->prev;
        else
            it.pos->next->prev = it.pos->prev;

        delete it.pos;
        nodes--;
    }

    size_t size() { return nodes; }
    bool empty()  { return nodes == 0; }
    void clear()  { while (not empty()) pop_front(); }
};
