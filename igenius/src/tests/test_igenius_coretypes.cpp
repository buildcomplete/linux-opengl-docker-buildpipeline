#include "test_framework.h"
#include "IG_types.h"
#include <iostream>



void test_CellPosition_equals()
{
    test_section("Testing of cellposition compare");
    
    CellPosition p1 = {1, 1};
    CellPosition p2 = {1, 1};
    test_equals(p1, p2, "Testing cell position equals");

    CellPosition p3 = {1, 2};
    test_not_equals(p1, p3, "Testing cell position NOT equals");

    CellPosition p4 = p3;
    test_equals(p3, p4, "Testing cell position equals after assignent");

    p4.x++;
    test_not_equals(p3, p4, "Testing cell position NOT equals after increment (ie they are not having same data)");

}

void test_CellPosition_operatorplus()
{
    test_section("Testing of cellposition operator add");
    
    CellPosition p1 = {1, 1};
    CellPosition p2 = {2, 2};
    test_equals(p1+p1, p2, "Testing cell position adds up x and y 1");

    CellPosition p3 = {1, 2};
    CellPosition p4 = {2, 3};
    test_equals(p1+p3, p4, "Testing cell position adds up x and y 2");

}

void run_igenius_coretypes_tests()
{
    test_CellPosition_equals();
    test_CellPosition_operatorplus();
}