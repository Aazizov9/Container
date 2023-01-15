#include "Table.h"

/* Табличные функции */

int Table::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) {
	//std::cout << elem_counter<<endl;
    size_t hash = hash_function(key, keySize);

    void* key_buf = (void*)(_memory.allocMem(keySize));
    void* elem_buf = (void*)(_memory.allocMem(elemSize));
    memcpy(key_buf, key, keySize);
    memcpy(elem_buf, elem, elemSize);

    Cell cell = {key_buf, keySize, elem_buf, elemSize};

    if (table_array[hash] == nullptr)
        table_array[hash] = new (_memory.allocMem(sizeof (custom_List))) custom_List(_memory);//

    if (table_array[hash]->empty() || table_array[hash]->find(key, keySize) == nullptr) {
        table_array[hash]->push_front((void*)(&cell), (size_t)sizeof(cell));
        elem_counter++;
        return 0;
    }
    return 1;
}

void Table::removeByKey(void *key, size_t keySize) {
    size_t hash = hash_function(key, keySize);

    if (table_array[hash] == nullptr) return;
    custom_List::Iterator* found = table_array[hash]->find(key, keySize);

    if (found != nullptr) {
        table_array[hash]->remove(found);
        elem_counter--;
    }
    if (table_array[hash]->empty()) {
        _memory.freeMem(table_array[hash]);
        table_array[hash] = nullptr;
    }
}

Container::Iterator* Table::findByKey(void *key, size_t keySize) {
    size_t hash = hash_function(key, keySize);

    if (table_array[hash] != nullptr) {
        void* result = table_array[hash]->find(key, keySize);
        return (result != nullptr) ? new Table_Iterator((custom_List::Iterator*)result, (int)hash, this) : nullptr;//
    }
    else return nullptr;
}

void* Table::at(void *key, size_t keySize, size_t &valueSize) {
    return (this->findByKey(key, keySize) != nullptr) ? this->findByKey(key, keySize)->getElement(valueSize) : nullptr;//
}
//





size_t Table::hash_function(void *key, size_t keySize) {
    char* temp = (char*)(key);
    int p = 31;
    int m = 1e9 + 9;
    long long power_of_p = 1;
    long long hash = 0;

    for (int i = 0; i < keySize; i++) {
        hash = (hash + (temp[i] - 'a' + 1) * power_of_p) % m;
        power_of_p = (power_of_p * p) % m;
    }
    if (hash < 0) hash *= -1;

    return (size_t)(hash % table_size);
}

/* Контейнерные функции */

int Table::size() {
    return elem_counter;
}

size_t Table::max_bytes() {
    return _memory.maxBytes();
}

Container::Iterator* Table::find(void *elem, size_t size) {
    size_t buf;
    for (int i = 0; i < table_size; i++) {
        if (table_array[i] != nullptr) {
            Container::Iterator *current = table_array[i]->newIterator();
            while (true) {
                Cell* cell = (Cell*)(current->getElement(buf));
                if (memcmp(elem, cell->elem, size) == 0) return current;
                if (current->hasNext()) current->goToNext();
                else break;
            }
        }
    }
    return nullptr;
}

Container::Iterator* Table::newIterator() {
    if (table_size > 0) {
        for (int i = 0; i < table_size; i++) {
            if (table_array[i] != nullptr)
                return new Table_Iterator((custom_List::Iterator*)table_array[i]->newIterator(), i, this);
        }
    }
    return nullptr;
}

void Table::remove(Iterator *iter) {
    size_t buf;
    auto* iterator = dynamic_cast<Table_Iterator*>(iter);

    if (iterator == nullptr || iterator->current_cell == nullptr) return;

    if (table_array[iterator->index] != nullptr || !table_array[iterator->index]->empty()) {
        table_array[iterator->index]->remove(iterator->current_cell);
        elem_counter--;
    }

    if (iterator->current_cell->getElement(buf) == nullptr) {
        if (table_array[iterator->index]->empty()) {
            _memory.freeMem(table_array[iterator->index]);
            table_array[iterator->index] = nullptr;
        }
        if (iterator->hasNext()) iterator->goToNext();
        else {
            delete iterator->current_cell;
            iterator->current_cell = nullptr;
            iterator->index = -1;
        }
    }
}

void Table::clear() {
	
    for (int i = 0; i < table_size; i++) {
        _memory.freeMem(table_array[i]);
        table_array[i] = nullptr;
    }
    elem_counter = 0;
}

bool Table::empty() {
    return (elem_counter == 0);
}

/* Функции итератора */

void* Table::Table_Iterator::getElement(size_t &size) {
    size_t buf;// /
    if (current_cell != nullptr) {
        Cell* temp = (Cell* )(current_cell->getElement(buf));
        if (temp == nullptr) return nullptr;
        size = temp->elem_size;
        return temp->elem;
    }
    else return nullptr;
}

bool Table::Table_Iterator::hasNext() {
    if (current_cell == nullptr) return false;
    if (current_cell->hasNext()) return true;
    else {
        for (int i = index + 1; i < table_size; i++) {
            if (table->table_array[i] != nullptr && !table->table_array[i]->empty()) return true;
        }
    }
    return false;
}

void Table::Table_Iterator::goToNext() {
    if (current_cell->hasNext()) current_cell->goToNext();
    else {
        for (int i = index + 1; i < table_size; i++) {
            if (table->table_array[i] != nullptr && !table->table_array[i]->empty()) {
                current_cell = table->table_array[i]->newIterator();
                index = i;
                return;
            }
        }
    }
}

bool Table::Table_Iterator::equals(Iterator *right) {
    if (current_cell != nullptr && right != nullptr)
        return (current_cell->equals(((Table_Iterator *)right)->current_cell));
    else return false;
}