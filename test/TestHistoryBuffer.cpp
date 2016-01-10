#include "yarrar/HistoryBuffer.h"

#include <catch.hpp>
#include <iostream>

#define REQUIRE_RANGE_ITEM_COUNT(range, itemCount) \
    {                                              \
        size_t items = 0;                          \
        for(const auto& item : range)              \
        {                                          \
            items++;                               \
        };                                         \
        REQUIRE(itemCount == items);               \
    }


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

TEST_CASE("HistoryBuffer basic functionality", "[util]")
{
    using namespace yarrar;
    HistoryBuffer<TestData> buffer(2);

    SECTION("empty buffer returns no data")
    {
        REQUIRE_RANGE_ITEM_COUNT(buffer.get(2), 0);
    }

    SECTION("adding element works")
    {
        buffer.add({ 1, "value1" });
        auto range = buffer.get(2);
        REQUIRE_RANGE_ITEM_COUNT(range, 1);
    }

    SECTION("adding too many elements prunes the oldest one")
    {
        buffer.add({ 1, "value" });
        buffer.add({ 2, "value2" });
        buffer.add({ 3, "value3" });
        auto range = buffer.get(2);
        REQUIRE_RANGE_ITEM_COUNT(range, 2);

        bool value1Found = false;
        bool value2Found = false;
        bool value3Found = false;
        for(const auto& item : range)
        {
            if(item.i == 1 && item.s == "value1")
            {
                value1Found = true;
            }
            else if(item.i == 2 && item.s == "value2")
            {
                value2Found = true;
            }
            else if(item.i == 3 && item.s == "value3")
            {
                value3Found = true;
            }
        }

        REQUIRE_FALSE(value1Found);
        REQUIRE(value2Found);
        REQUIRE(value3Found);
    }

    SECTION("requesting too many elements (bufferSize) returns all available data")
    {
        buffer.add({ 1, "value" });
        buffer.add({ 2, "value2" });
        auto range = buffer.get(5);
        REQUIRE_RANGE_ITEM_COUNT(range, 2);

        auto range2 = buffer.get(3);
        REQUIRE_RANGE_ITEM_COUNT(range2, 2);
    }

    SECTION("requesting too many elements (currently available data) returns all available data")
    {
        buffer.add({ 1, "value" });
        auto range = buffer.get(5);
        REQUIRE_RANGE_ITEM_COUNT(range, 1);

        auto range2 = buffer.get(3);
        REQUIRE_RANGE_ITEM_COUNT(range2, 1);
    }

    SECTION("requesting zero elements returns no data")
    {
        buffer.add({ 1, "value" });
        auto range = buffer.get(0);
        REQUIRE_RANGE_ITEM_COUNT(range, 0);
    }


    SECTION("clearing works")
    {
        buffer.add({ 1, "value" });
        buffer.add({ 2, "value2" });
        REQUIRE_RANGE_ITEM_COUNT(buffer.get(2), 2);

        buffer.clear();
        REQUIRE_RANGE_ITEM_COUNT(buffer.get(0), 0);
    }
}
}
