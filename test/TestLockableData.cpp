#include "yarrar/LockableData.h"

#include <catch.hpp>
#include <atomic>
#include <chrono>
#include <thread>

#include <iostream>

namespace
{

struct TestData
{
    int i;
    std::string s;
};
}

namespace yarrar_test
{

TEST_CASE("LockableData basic functionality", "[util]")
{
    using namespace yarrar;
    LockableData<TestData> data({ 1, "value1" });

    SECTION("data can be fetched")
    {
        auto handle = data.lockRead();
        REQUIRE(handle.get().i == 1);
        REQUIRE(handle.get().s == "value1");
    }

    SECTION("ref to data is usable")
    {
        auto handle = data.lockRead();
        auto innerData = handle.get();
        REQUIRE(innerData.i == 1);
        REQUIRE(innerData.s == "value1");
    }

    SECTION("data can be changed")
    {
        auto handle = data.lockReadWrite();
        TestData data2{ 2, "value2" };
        handle.set(std::move(data2));
        REQUIRE(handle.get().i == 2);
        REQUIRE(handle.get().s == "value2");

        SECTION("data can be changed twice")
        {
            handle.set({ 3, "value3" });
            REQUIRE(handle.get().i == 3);
            REQUIRE(handle.get().s == "value3");
        }
    }

    SECTION("data is locked properly")
    {
        using namespace std::chrono;

        std::atomic<bool> flag(false);
        std::thread t(
            [&data, &flag]()
            {
                flag = true;
                auto handle = data.lockReadWrite();
                TestData data3{3, "value3"};
                handle.set(std::move(data3));
                std::this_thread::sleep_for(milliseconds(300));
            });
        // Busy-wait for thread to spawn.
        while(!flag.load())
        {
        }

        auto startTime = high_resolution_clock::now();
        auto handle = data.lockRead();
        auto endTime = high_resolution_clock::now();

        auto waitedMilliseconds = duration_cast<milliseconds>(endTime - startTime).count();
        auto innerData = handle.get();
        REQUIRE(innerData.i == 3);
        REQUIRE(innerData.s == "value3");
        REQUIRE(waitedMilliseconds > 250);

        t.join();
    }
}
}
