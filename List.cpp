#include <iostream>
#include "List.h"

void* List::ListIterator::getElement(size_t &size)
{
    if (curElement == nullptr)
        return nullptr;
    size = curElement->size;
    return curElement->value;
}

bool List::ListIterator::hasNext()
{
    if (curElement == nullptr)
        return false;
    else
    {
        if (curElement->next == nullptr)
            return false;
        else
            return true;
    }
}

void List::ListIterator::goToNext()
{
    if (curElement == nullptr)
        return;
    curElement = curElement->next;
}

bool List::ListIterator::equals(Iterator *right)
{
    auto* other = dynamic_cast<ListIterator*>(right);
    return (curElement == other->curElement);
}

int List::size()
{
    size_t overall_size = 0;
    //    auto* iterator = dynamic_cast<ListIterator*>(begin());
//    auto* beginElement = (List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement));
    if (List::empty())
        return 0;
    auto* iterator = dynamic_cast<ListIterator*>((List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement)));
    while (iterator->curElement != nullptr)
    {
        overall_size++;
        iterator->goToNext();
    }
    return overall_size;
}

size_t List::max_bytes()
{
    return _memory.maxBytes();
}

Container::Iterator* List::find(void *elem, size_t size)
{
//    auto* iterator = dynamic_cast<ListIterator*>(begin());
//    auto* beginElement = (List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement));
    if (List::empty())
        return nullptr;
    auto* iterator = dynamic_cast<ListIterator*>((List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement)));
    while (iterator->curElement != nullptr)
    {
        if (size != iterator->curElement->size)
        {
            iterator->goToNext();
            continue;
        }
        if (memcmp(elem, iterator->curElement->value, size) == 0)
            break;
        else
            iterator->goToNext();
    }
    if (iterator->curElement == nullptr)
        return nullptr;
    return iterator;
}

Container::Iterator* List::newIterator()
{
    return new ListIterator(List::startElement);
}

//Container::Iterator* List::begin()
//{
//    if (List::startElement == nullptr)
//        return nullptr;
//    return new List::ListIterator(List::startElement);
//}
//
//Container::Iterator* List::end()
//{
//    if (List::startElement == nullptr)
//        return nullptr;
//    return new List::ListIterator(nullptr);
//}

void List::remove(Iterator *iter)
{
    auto* newIter = dynamic_cast<ListIterator*>(iter);
//    auto* findIter = dynamic_cast<ListIterator*>(begin());
    auto* findIter = dynamic_cast<ListIterator*>((List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement)));

    if (iter->equals(new List::ListIterator(List::startElement)))
        pop_front();
    else
    {
        while (findIter->curElement->next != newIter->curElement)
            findIter->goToNext();
    }
    findIter->curElement->next = newIter->curElement->next;
    auto* tempIter = dynamic_cast<ListStruct*>(newIter->curElement);
    newIter->curElement = findIter->curElement->next;
    delete[]static_cast<char*>(tempIter->value);
    delete tempIter;
//    delete findIter;
}

void List::clear()
{
    auto* elementToDelete = List::startElement;
    while (elementToDelete != nullptr)
    {
        delete[]static_cast<char*>(elementToDelete->value);
        auto* next = elementToDelete->next;
        delete elementToDelete;
        elementToDelete = next;
    }
    List::startElement = nullptr;
}

bool List::empty()
{
    if (List::startElement == nullptr)
        return true;
    return false;
}

int List::push_front(void *elem, size_t elemSize)
{
//    auto* newElement = new List::ListStruct;
//    newElement->value = static_cast<void*>(new char[elemSize]);
    auto* newElement = (List::ListStruct*)_memory.allocMem(sizeof(List::ListStruct));
    if (newElement == nullptr)
//        throw; // или
        return 1;

    newElement->value = _memory.allocMem(elemSize);
    if (newElement->value == nullptr)
    {
        _memory.freeMem(newElement);
//        delete newElement;
//        throw;
        return 1;
    }

    memcpy(newElement->value, elem, elemSize);
    newElement->size = elemSize;
    newElement->next = List::startElement;
    if (empty())
        newElement->next = nullptr;
    List::startElement = newElement;
    return 0;
}

void List::pop_front()
{
    if (List::startElement == nullptr)
    {
        List::startElement = nullptr;
        List::startElement->next = nullptr;
        _memory.freeMem(List::startElement->value);
        _memory.freeMem(List::startElement);
        return;
    }
    auto* nextElement = List::startElement->next;
//    delete[]static_cast<char*>(List::startElement->value);
//    delete List::startElement;
    _memory.freeMem(List::startElement->value);
    _memory.freeMem(List::startElement);

    List::startElement = nextElement;
}

void* List::front(size_t &size) //*
{
    if (List::startElement == nullptr)
    {
        size = 0;
        return nullptr;
    }
    size = List::startElement->size;
    return List::startElement->value;
}

int List::insert(Container::Iterator *iter, void *elem, size_t elemSize)
{
//    if (iter == begin())
    if (iter == (List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement)))
        return push_front(elem, elemSize);

    auto* newIter = dynamic_cast<ListIterator*>(iter);
//    auto* findIter = dynamic_cast<ListIterator*>(begin());
    auto* findIter = dynamic_cast<ListIterator*>((List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement)));

    while (findIter->curElement->next != newIter->curElement)
        findIter->goToNext();

    auto* newElement = (List::ListStruct*)_memory.allocMem(sizeof(List::ListStruct));
    if (newElement == nullptr)
//        throw;
        return 1;

    newElement->value = _memory.allocMem(sizeof(elemSize));
    if (newElement->value == nullptr)
    {
        _memory.freeMem(newElement);
//        throw;
        return 1;
    }

    memcpy(newElement->value, elem, elemSize);
    newElement->size = elemSize;
    newElement->next = findIter->curElement->next;
    findIter->curElement->next = newElement;
    delete findIter;

    return 0;
}

