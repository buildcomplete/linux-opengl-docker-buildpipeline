#include "test_framework.h"
#include "IG_types.h"

#include <iostream>
std::ostream& operator<<(std::ostream& os, const CellPosition& pos) {
    os << "CellPosition: (" << pos.x << ", " << pos.y << ")";
    return os;
};


void test_CellPosition_basic()
{
    test_section("Testing of basic cellposition components");
    
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

void run_igenius_coretypes_tests()
{
    test_CellPosition_basic();
}