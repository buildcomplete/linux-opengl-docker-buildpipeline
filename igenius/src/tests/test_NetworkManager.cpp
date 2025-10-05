#include "test_framework.h"
#include "NetworkManager.h"

void test_network_manager_createNetworkSimple()
{
    test_section("Network Manager CreateNetworkSimple tests");

    // create network path along horizontal lines
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0,0});
        m.TryAddAnchorPoint({0,10});
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path along horizontal lines");
    }
    
    // create network path along vertical lines
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0,0});
        m.TryAddAnchorPoint({10,0});
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path along vertical lines");
    }
        
    // create network path along diagonal lines
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0,0});
        m.TryAddAnchorPoint({10,10});
        auto result = m.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path along diagonal lines");
    }

    // create network path not on straight line, fails
    {
        NetworkManager m;
        m.StartDrawing();
        m.TryAddAnchorPoint({0,0});
        test_assert(m.TryAddAnchorPoint({10,15}) == false, "TryAdd... Create network path not on straight line", "TryAddAnchorPoint should return false");
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
        m.TryAddAnchorPoint({ 0, 0});
        m.TryAddAnchorPoint({ 0,10});
        m.TryAddAnchorPoint({10,10});
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
        n.TryAddAnchorPoint({0,0});
        n.TryCreatePathToAnchorPoint({10,10});
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(2), "Test Create 2 point network path following straight line");
    }

    { // Sout South East (SSE)
        NetworkManager n;
        n.StartDrawing();
        CellPosition start = {0,0};
        CellPosition end = {15,10};
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
        CellPosition start = {0,0};
        CellPosition end = {10,15};
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
        CellPosition start = {10,15};
        CellPosition end = {0,0};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should create 3 point path, example (10,15)->(10,10)->(0,0)
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(3), "Test Create 3 point network path following straight line (NWW)");
        test_equals(result.front(), start, "Test Create 3 point network path starts as expected (NWW)");
        test_equals(result.back(), end, "Test Create 3 point network path ends as expected (NWW)");
    }

    { // Sout East East (NNW)
        NetworkManager n;
        n.StartDrawing();
        CellPosition start = {15,10};
        CellPosition end = {0,0};
        n.TryAddAnchorPoint(start);
        n.TryCreatePathToAnchorPoint(end); // should create 3 point path, example (15,10)->(10,10)->(0,0)
        auto result = n.CompleteDrawing();
        test_equals(result.size(), static_cast<size_t>(3), "Test Create 3 point network path following straight line (NNW)");
        test_equals(result.front(), start, "Test Create 3 point network path starts as expected (NNW)");
        test_equals(result.back(), end, "Test Create 3 point network path ends as expected (NNW)");
    }

}

// Function to run all engine tests
void run_networkm_manager_tests() 
{
    test_network_manager_createNetworkSimple();
    test_network_manager_createNetworkPathFinding();
}
