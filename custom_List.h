#ifndef TABLE_TEST_CUSTOM_LIST_H
#define TABLE_TEST_CUSTOM_LIST_H

#include "List.h"
#include "Table.h"
#include <iostream>

struct Cell {
    void* key;
    size_t key_size;
    void* elem;
    size_t elem_size;
};

class custom_List: public List {
public:
    custom_List(MemoryManager &mem): List(mem) {}

    Iterator* find(void* elem, size_t size) override {
        custom_List::Iterator* current = newIterator();
        size_t buf;

        while (current != nullptr) {
            Cell* cell = (Cell*)(current->getElement(buf));
            if (cell->key_size == size)
                if (memcmp(cell->key, elem, size) == 0) break;
            if (current->hasNext()) current->goToNext();
            else return nullptr;
        }
        return current;
    }

    void remove(Iterator *iter) override {

        auto* temp = dynamic_cast<ListIterator*>(iter);
        iter = temp;

        if (iter->equals(new List::ListIterator(List::startElement))) {
            pop_front();
            temp->curElement = startElement;
            iter = temp;
        }
        else {
            auto* newIter = dynamic_cast<ListIterator*>(iter);
            //auto* findIter = dynamic_cast<ListIterator*>(begin());
            auto* findIter = dynamic_cast<ListIterator*>((List::startElement == nullptr ? nullptr : new List::ListIterator(List::startElement)));

            while (findIter->curElement->next != newIter->curElement)
                findIter->goToNext();

            findIter->curElement->next = newIter->curElement->next;
            auto* tempIter = dynamic_cast<ListStruct*>(newIter->curElement);
            newIter->curElement = findIter->curElement->next;
            delete[]static_cast<char*>(tempIter->value);
            delete tempIter;
            //delete findIter;
        }
    }
};

#endif //TABLE_TEST_CUSTOM_LIST_H