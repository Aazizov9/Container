#pragma once
#include "Mem.h"
#include "ListAbstract.h"

class List: public AbstractList
{
//private:
//    struct ListStruct {
//        void* value;
//        size_t size;
//        ListStruct* next;
//    };
public:
    struct ListStruct {
        void* value;
        size_t size;
        ListStruct* next;
    };
    class ListIterator: public List::Iterator
//    class ListIterator: public AbstractList::Iterator
    {
    public:
        ListStruct* curElement;
        // конструктор
        ListIterator(ListStruct* curElement) : curElement(curElement) {}
        ~ListIterator() = default;
        // Iterator
        void* getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Iterator *right) override;
    };
    // конструктор
    List(MemoryManager &mem) : AbstractList(mem) {}
    // деструктор
    ~List() = default;
    // Container
    ListStruct* startElement = nullptr;
    int size() override;
    size_t max_bytes() override;
    Iterator* find(void *elem, size_t size) override; //*
    Iterator* newIterator() override;
    void remove(Iterator *iter) override;
    void clear() override;
    bool empty() override;
    int push_front(void *elem, size_t elemSize) override; //*
    void pop_front() override;
    void* front(size_t &size) override;
    int insert(Iterator *iter, void *elem, size_t elemSize) override;
//    static List::ListStruct* startElement;
};
//static List::ListStruct* startElement = nullptr;
