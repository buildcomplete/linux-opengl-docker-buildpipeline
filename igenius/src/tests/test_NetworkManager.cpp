#include "test_framework.h"
#include "NetworkManager.h"

void test_network_manager_createNetwork()
{
    test_section("Network Manager CreateNetwork tests");

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

    
    
    

}

// Function to run all engine tests
void run_networkm_manager_tests() 
{
    test_network_manager_createNetwork();
}
