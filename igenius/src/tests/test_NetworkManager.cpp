#include "test_framework.h"
#include "NetworkManager.h"

void test_network_manager_createNetworkSimple()
{
    test_section("Network Manager CreateNetworkSimple tests");

    // create network path along horizontal lines
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0, 0});
        m.TryAddAnchorPoint({0, 10});
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path along horizontal lines");
    }

    // create network path along vertical lines
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0, 0});
        m.TryAddAnchorPoint({10, 0});
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path along vertical lines");
    }

    // create network path along diagonal lines
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0, 0});
        m.TryAddAnchorPoint({10, 10});
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path along diagonal lines");
    }

    // create network path not on straight line, fails
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0, 0});
        test_assert(m.TryAddAnchorPoint({10, 15}) == false, "TryAdd... Create network path not on straight line", "TryAddAnchorPoint should return false");
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(0), "Test Create 2 point network path along not following straight line");
    }

    // create network path as a square
    // { 0, 0} -> {0, 10}
    //             \/
    // {10, 0} <-{10, 10}
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0, 0});
        m.TryAddAnchorPoint({0, 10});
        m.TryAddAnchorPoint({10, 10});
        m.TryAddAnchorPoint({10, 0});
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(4), "Test Create 4 point network path along diagonal and horizontal lines");
    }
}

void test_network_manager_createNetworkPathFinding()
{
    test_section("Network Manager CreateNetwork using path finding");
    {
        NetworkManager n;
        n.StartDrawing();
        n.TryAddAnchorPoint({0, 0});
        n.TryCreatePathToAnchorPoint({10, 10});
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path following straight line");
    }

    { // Sout South East (SSE)
        NetworkManager n;
        n.StartDrawing();
        CellPosition start = {0, 0};
        CellPosition end = {15, 10};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should create 3 point path, example (0,0)->(15,10)->(15,10)
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(3), "Test Create 3 point network path following straight line (SSE)");
        test_equals(result.front(), start, "Test Create 3 point network path starts as expected (SSE)");
        test_equals(result.back(), end, "Test Create 3 point network path ends as expected (SSE)");
    }

    { // Sout East East (SEE)
        NetworkManager n;
        n.StartDrawing();
        CellPosition start = {0, 0};
        CellPosition end = {10, 15};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should create 3 point path, example (0,0)->(10,15)->(10,15)
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(3), "Test Create 3 point network path following straight line (SEE)");
        test_equals(result.front(), start, "Test Create 3 point network path starts as expected (SEE)");
        test_equals(result.back(), end, "Test Create 3 point network path ends as expected (SEE)");
    }

    { // Sout East East (NWW)
        NetworkManager n;
        n.StartDrawing();
        CellPosition start = {10, 15};
        CellPosition end = {0, 0};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should create 3 point path, example (10,15)->(10,10)->(0,0)
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(3), "Test Create 3 point network path following straight line (NWW)");
        test_equals(result.front(), start, "Test Create 3 point network path starts as expected (NWW)");
        test_equals(result.back(), end, "Test Create 3 point network path ends as expected (NWW)");
    }

    { // Sout East East (NNE)
        NetworkManager n;
        CellPosition start = {0, 10};
        CellPosition end = {15, 0};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should create 3 point path, example (0,10)->(10,10)->(15,0)
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(3), "Test Create 3 point network path following straight line (NNE)");
        test_equals(result.front(), start, "Test Create 3 point network path starts as expected (NNE)");
        test_equals(result.back(), end, "Test Create 3 point network path ends as expected (NNE)");
    }
}

void test_network_manager_atBoundStable()
{
    test_section("Network Manager CreateNetwork path finder at bounds stability");
    {
        NetworkManager n;
        CellPosition start = {0, 0};
        CellPosition end = {255, 255};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should not create anything as we try to go out of bounds
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create network, end is at bounds");
    }
}
void test_network_manager_outOfBoundStable()
{
    test_section("Network Manager CreateNetwork path finder out of bounds stability");
    {
        NetworkManager n;
        CellPosition start = {10, 10};
        CellPosition end = {-1, 0};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should not create anything as we try to go out of bounds
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(0), "Test Create network, end is out of bounds and does nothing");
    }

    {
        CellPosition start = {-1, 0}, end = {10, 10};
        NetworkManager n;
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should not create anything as we try to go out of bounds
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(0), "Test Create network, start is out of bounds and does nothing");
    }
}

void test_network_manager_createPathAvoidBlocked()
{
    test_section("Network Manager CreateNetwork path finder avoid blocked");
    {
        CellPosition start = {1, 1},end = {1, 3};
        NetworkManager n;
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // Creating a straigt pipe, with no blocking / contraints, should give two anchors
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Creating a straigt pipe, with no blocking / contraints, should give two anchors");
    }

    {
        CellPosition start = {1, 1},end = {1, 4};
        NetworkManager n;
        n.SetConstraintFunction([](const CellPosition& anchor) 
        {
            return (NetworkConstraintFlags)((anchor.x == 1 && anchor.y == 2) 
                ? FLAG_NCONSTRAINT_N | FLAG_NCONSTRAINT_NE | FLAG_NCONSTRAINT_E | FLAG_NCONSTRAINT_SE | FLAG_NCONSTRAINT_S | FLAG_NCONSTRAINT_SW | FLAG_NCONSTRAINT_W | FLAG_NCONSTRAINT_NW
                : FLAG_NCONSTRAINT_NO_CONSTRAINTS);
        });

        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // Creating a straigt pipe, with no blocking / contraints, should give two anchors
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(4), "Creating a straigt pipe, with a blocking / contraints, should give four anchors");
    }

    { // Test where entry only allows horizonatal, should create an additional bend
        CellPosition start = {1, 1},end = {3, 3};
        NetworkManager n;
        n.SetConstraintFunction([](const CellPosition& anchor) 
        {
            return (NetworkConstraintFlags)((anchor.x == 3 && anchor.y == 3) 
                ? FLAG_NCONSTRAINT_ALLDIR & ~(FLAG_NCONSTRAINT_E | FLAG_NCONSTRAINT_W)
                : FLAG_NCONSTRAINT_NO_CONSTRAINTS);
        });

        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // Test where entry only allows horizonatal, should create an additional bend
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(4), "Test diagonal where target only allows horizonatal, should create an additional bend");
    }

       { // Test where entry only allows horizonatal, should create an additional bend
        CellPosition start = {3, 3},end = {1, 1};
        NetworkManager n;
        n.SetConstraintFunction([](const CellPosition& anchor) 
        {
            return (NetworkConstraintFlags)((anchor.x == 3 && anchor.y == 3) 
                ? FLAG_NCONSTRAINT_ALLDIR & ~(FLAG_NCONSTRAINT_E | FLAG_NCONSTRAINT_W)
                : FLAG_NCONSTRAINT_NO_CONSTRAINTS);
        });

        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); //Test where start only allows horizonatal, should create an additional bend
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(4), "Test diagonal where start only allows horizonatal, should create an additional bend");
    }




}

// Function to run all engine tests
void run_networkm_manager_tests()
{
    test_network_manager_createNetworkSimple();
    test_network_manager_createNetworkPathFinding();
    test_network_manager_atBoundStable();
    test_network_manager_outOfBoundStable();
    test_network_manager_createPathAvoidBlocked();
}
