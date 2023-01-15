#include <iostream>
#include "Table.h"
#include "TestClass.h"


int main() {
    Mem mem(10000000);
    Table table(mem);
    int num = 1000000;
    TableTest::BaseTest(table, num);

    return 0;
}