#include "test_framework.h"
#include "canvas/tools/PickAndPlaceComponentTool.h"

void test_pickPickAndPlaceComponentTool()
{
    test_section("test_pickPickAndPlaceComponentTool");
    Canvas c;
    //c.
    auto tool = PickAndPlaceComponentTool(&c);
}

void run_igenius_tools_PickAndPlaceComponentTool_tests()
{
    test_pickPickAndPlaceComponentTool();
}