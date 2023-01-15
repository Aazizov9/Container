#ifndef TABLE_TEST_TESTCLASS_H
#define TABLE_TEST_TESTCLASS_H

#include "Table.h"

class TableTest {
public:
    static void insertByKeyTest(Table &table, int num);
    static void findByKeyTest(Table &table, int num);
    static void removeByKeyTest(Table &table, int num);
    static void removeTest(Table &table, int num);
    static void sizeTest(Table &table, int num);
    static void iteratorTest(Table &table, int num);
    static void timeTest(Table &table, int num);

    static void BaseTest(Table &table, int num);
};



#endif //TABLE_TEST_TESTCLASS_H
