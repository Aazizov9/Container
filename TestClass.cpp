#include "TestClass.h"
#include <ctime>
#include <cassert>

void TableTest::insertByKeyTest(Table &table, int num) {

    cout << "insertByKey test..." << endl;
    clock_t t1 = clock();
    size_t size = 0;

    for (int i = 0; i < num; i+=4) table.insertByKey(&i, sizeof (int), &i, sizeof (int));

    for (int i = 0; i < num; ++i) {
        Container::Iterator* tester = table.findByKey(&i, sizeof(int));
        if (i % 4 == 0 && tester == nullptr) {
            cout << "There's no inserted element in the table" << endl;
            assert(tester != nullptr);
        }
        else if (i % 4 != 0 && tester != nullptr) {
            cout << "There's an element that wasn't inserted" << endl;
            assert(tester == nullptr);
        }
        else if (tester != nullptr) {
            auto elem = *(int *)tester->getElement(size);
            if (elem != i) {
                cout << "Invalid value" << endl;
                assert(elem == i);
            }
        }
        delete tester;
    }

    for (int i = 2; i < num; i+=4) table.insertByKey(&i, sizeof (int), &i, sizeof (int));

    for (int i = 0; i < num; ++i) {
        Container::Iterator* tester = table.findByKey(&i, sizeof(int));
        if (i % 2 == 0 && tester == nullptr) {
            cout << "There's no inserted element in the table" << endl;
            assert(tester != nullptr);
        }
        else if (i % 2 != 0 && tester != nullptr) {
            cout << "There's an element that wasn't inserted" << endl;
            assert(tester == nullptr);
        }
        else if (tester != nullptr) {
            auto elem = *(int*)tester->getElement(size);
            if (elem != i) {
                cout << "Invalid value" << endl;
                assert(elem == i);
            }
        }
        delete tester;
    }
    assert(table.size() == num/2);

    char elem[] = "Hello, world!";
    for (int i = 1; i < num; i+=2) table.insertByKey(&i, sizeof(int), &elem, sizeof(elem));

    for (int i = 0; i < num; i++) {
        Container::Iterator* tester = table.findByKey(&i, sizeof(int));
        if (tester == nullptr) {
            cout << "There's no inserted element in the table" << endl;
            assert(tester != nullptr);
        }
        if ((i % 2 == 0 && *(int*)tester->getElement(size) != i) ||
            (i % 2 == 1 && (memcmp(tester->getElement(size), elem, sizeof(size))) != 0)) {
            cout << "Invalid value" << endl;
            exit(1);
        }
        delete tester;
    }

    assert(table.size() == num);

    table.clear();
    assert(table.empty());

    cout << "insertByKey test successfully finished" << endl;
    clock_t t2 = clock();
    cout << "total time: " << (t2 - t1) << " ms" << endl;
    cout << endl;
}

void TableTest::findByKeyTest(Table &table, int num) {

    cout << "findByKey test..." << endl;
    clock_t t1 = clock();
    size_t size = 0;

    for (int i = 0; i < num; i+=3) {
        table.insertByKey(&i, sizeof (int), &i, sizeof (int));
    }

    for (int i = 0; i < num; i++) {
        Container::Iterator* tester = table.findByKey(&i, sizeof(int));
        if (i % 3 != 0 && tester != nullptr) {
            cout << "There's an element that wasn't inserted" << endl;
            assert(tester == nullptr);
        }
        else if (i % 3 == 0 && tester != nullptr && *(int*)tester->getElement(size) != i) {
            cout << "Invalid value" << endl;
            assert(*(int*)tester->getElement(size) == i);
        }
        delete tester;
    }

    size = (int)(num/3) + 1;
    assert(table.size() == size);

    table.clear();
    assert(table.empty());

    cout << "findByKey test successfully finished" << endl;
    clock_t t2 = clock();
    cout << "total time: " << (t2 - t1)  << " ms" << endl;
    cout << endl;
}

void TableTest::removeByKeyTest(Table &table, int num) {

    cout << "removeByKey test..." << endl;
    clock_t t1 = clock();

    for (int i = 0; i < num; i++) table.insertByKey(&i, sizeof(int), &i, sizeof(int));
    assert(table.size() == num);

    for (int i = 0; i < num; i+=10) table.removeByKey(&i, sizeof(int));
    assert(table.size() == 9 * num / 10);

    for (int i = 0; i < num; i++) {
        Container::Iterator* tester = table.findByKey(&i, sizeof(int));
        if (i % 10 == 0 && tester != nullptr) {
            cout << "Unremoved element" << endl;
            assert(tester == nullptr);
        }
        else if (i % 10 != 0 && tester == nullptr) {
            cout << "There's no inserted element in the table" << endl;
            assert(tester != nullptr);
        }
        delete tester;
    }

    for (int i = 0; i < num; i++) table.removeByKey(&i, sizeof(int));
    assert(table.size() == 0);

    for (int i = 0; i < num; i++) {
        Container::Iterator* tester = table.findByKey(&i, sizeof(int));
        if (tester != nullptr) {
            cout << "Unremoved element" << endl;
            assert(tester == nullptr);
        }
        delete tester;
    }

    table.clear();
    assert(table.empty());

    cout << "removeByKey test successfully finished" << endl;
    clock_t t2 = clock();
    cout << "total time: " << (t2 - t1 + .0)  << " ms" << endl;
    cout << endl;
}

void TableTest::removeTest(Table &table, int num) {

    cout << "remove test..." << endl;
    size_t size;
    clock_t t1 = clock();

    for (int i = 0; i < num; i++) table.insertByKey(&i, sizeof(int), &i, sizeof(int));
    assert(table.size() == num);

    Container::Iterator* tester = table.newIterator();

    while (tester->getElement(size) != nullptr) table.remove(tester);
    assert(table.size() == 0);

    for (int i = 0; i < num; i++) table.insertByKey(&i, sizeof(int), &i, sizeof(int));
    assert(table.size() == num);

    tester = table.newIterator();

    while (true) {
        if (tester->getElement(size) == nullptr) break;
        int elem = *(int*)tester->getElement(size);
        if (elem % 2 == 0 && tester->hasNext()) tester->goToNext();
        else if (elem % 2 == 1) table.remove(tester);
        else break;
    }

    for (int i = 0; i < num; i++) {
        tester = table.findByKey(&i, sizeof(int));
        if (i % 2 == 1 && tester != nullptr) {
            cout << "Unremoved element" << endl;
            assert(tester == nullptr);
        }
        else if (i % 2 == 0 && tester == nullptr) {
            cout << "There's no inserted element in the table" << endl;
            assert(tester != nullptr);
        }
    }

    assert(table.size() == num / 2);

    tester = table.newIterator();
    while (tester->getElement(size) != nullptr) table.remove(tester);
    assert(table.size() == 0);

    delete tester;
    table.clear();
    assert(table.empty());

    cout << "remove test successfully finished" << endl;
    clock_t t2 = clock();
    cout << "total time: " << (t2 - t1) << " ms" << endl;
    cout << endl;
}

void TableTest::sizeTest(Table &table, int num) {

    cout << "size test..." << endl;
    size_t size;
    clock_t t1 = clock();

    for (int i = 0; i < num; i += 2) table.insertByKey(&i, sizeof(int), &i, sizeof(int));
    assert(table.size() == num / 2);

    for (long long int i = 1; i < num; i += 2) table.insertByKey(&i, sizeof(long long int), &i, sizeof(long long int));
    assert(table.size() == num);

    for (int i = 0; i < num; i += 2) {
        Container::Iterator *tester = table.findByKey(&i, sizeof(int));
        if (tester == nullptr) {
            cout << "The table doesn't have an element that was inserted" << endl;
            assert(tester != nullptr);
        }
        if (tester->getElement(size) == nullptr || size != sizeof(int)) {
            cout << "The found element has the wrong size" << endl;
            assert(size == sizeof(int));
        }
    }

    for (long long int i = 1; i < num; i += 2) {
        Container::Iterator *tester = table.findByKey(&i, sizeof(long long int));
        if (tester == nullptr) {
            cout << "The table doesn't have an element that was inserted" << endl;
            assert(tester != nullptr);
        }
        if (tester->getElement(size) == nullptr || size != sizeof(long long int)) {
            cout << "The found element has the wrong size" << endl;
            assert(size == sizeof(long long));
        }
    }

    table.clear();
    assert(table.empty());

    cout << "size test successfully finished" << endl;
    clock_t t2 = clock();
    cout << "total time: " << (t2 - t1) << " ms" << endl;
    cout << endl;
}

void TableTest::iteratorTest(Table &table, int num) {

    cout << "iterator test..." << endl;
    clock_t t1 = clock();

    for (int i = 0; i < num; i+= 2)  table.insertByKey(&i, sizeof(int), &i, sizeof(int));
    assert(table.size() == num / 2);

    Container::Iterator* tester = table.newIterator();
    Container::Iterator* last_iterator = table.newIterator();

    while (last_iterator->hasNext()) last_iterator->goToNext();
    while (!tester->equals(last_iterator)) table.remove(tester);
    assert(table.size() == 1);

    table.remove(last_iterator);
    assert(table.size() == 0);

    delete tester;
    delete last_iterator;

    table.clear();
    assert(table.empty());

    cout << "iterator test successfully finished" << endl;
    clock_t t2 = clock();
    cout << "total time: " << (t2 - t1) << " ms" << endl;
    cout << endl;
}

void TableTest::timeTest(Table &table, int num) {

    cout << "time test..." << endl;
    clock_t t1 = clock();
    for (int i = 0; i < num; i++) table.insertByKey(&i, sizeof(int), &i, sizeof(int));
    clock_t t2 = clock();
    cout << "insertByKey: " << (t2 - t1) << " ms" << endl;

    t1 = clock();
    for (int i = 0; i < num; i++) {
        Container::Iterator *temp = table.findByKey(&i, sizeof(int));
        delete temp;
    }
    t2 = clock();
    cout << "findByKey: " << (t2 - t1) << " ms" << endl;

    t1 = clock();
    for (int i = 1; i < num; i += 2) table.removeByKey(&i, sizeof(int));
    t2 = clock();
    cout << "removeByKey: " << (t2 - t1) << " ms" << endl;

    t1 = clock();
    for (int i = 0; i < num; i++) {
        Container::Iterator *temp = table.findByKey(&i, sizeof(int));
        delete temp;
    }
    t2 = clock();
    cout << "findByKey 2: " << (t2 - t1)  << " ms" << endl;

    table.clear();
    assert(table.empty());

    cout << "time test successfully finished" << endl;
    cout << endl;
}

void TableTest::BaseTest(Table &table, int num) {
    insertByKeyTest(table, num);
    findByKeyTest(table, num);
    removeByKeyTest(table, num);
    removeTest(table, num);
    sizeTest(table, num);
    iteratorTest(table, num);
    timeTest(table, num);
    cout << "=================" << endl;
    cout << "The program has passed all tests" << endl;
    cout << "=================" << endl;
}