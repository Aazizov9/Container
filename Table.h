#pragma once

#include "TableAbstract.h"
#include "Container.h"
#include "custom_List.h"

#define table_size 10'000'000

class Table: public AbstractTable {
private:
    custom_List **table_array;
    int elem_counter = 0;

public:

    Table(MemoryManager &mem): AbstractTable(mem) {
            table_array = new (mem.allocMem(table_size * sizeof(custom_List*)))custom_List*[table_size];
            for (int i = 0; i < table_size; i++) table_array[i] = nullptr;
    }

    /* Табличный итератор */

    class Table_Iterator: public Container::Iterator {
    public:

        custom_List::Iterator *current_cell;
        Table *table;
        int index;

        Table_Iterator(custom_List::Iterator *current_cell, int index, Table* table):
            current_cell(current_cell),
            index(index),
            table(table) {}

        void* getElement(size_t &size) override;

        bool hasNext() override;

        void goToNext() override;

        bool equals(Iterator *right) override;

        ~Table_Iterator() = default;

        friend Table;
    };

    /* Табличные функции */

    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) override;

    void removeByKey(void *key, size_t keySize) override;

    Iterator* findByKey(void *key, size_t keySize) override;

    void* at(void *key, size_t keySize, size_t &valueSize) override;

    size_t hash_function(void *key, size_t keySize) override;

    /* Контейнерные функции */

    int size() override;

    size_t max_bytes() override;

    Iterator* find(void *elem, size_t size) override;

    Iterator* newIterator() override;

    void remove(Iterator *iter) override;

    void clear() override;

    bool empty() override;

    ~Table() = default;

};