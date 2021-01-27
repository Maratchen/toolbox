#include "gtest/gtest.h"
#include "segment_tree.hpp"

#include <array>
#include <memory_resource>

TEST(segment_tree_test, build)
{
    using tree_type = segment_tree<
        int,
        monoid::sum<int>,
        std::pmr::polymorphic_allocator<int>
    >;

    std::array<int, 16> layout = 
        {
            15,
            3, 12,
            1, 2, 3, 9,
            0, 0, 0, 0, 0, 0, 4, 5, 0
        };

    std::array<int, 16> buffer;

    auto resource = std::pmr::monotonic_buffer_resource(
        buffer.data(),
        buffer.size() * sizeof(int),
        std::pmr::null_memory_resource()
    );
    
    auto tree = tree_type(
        { 1, 2, 3, 4, 5 },
        std::pmr::polymorphic_allocator<int>(&resource)
    );

    EXPECT_EQ(tree.size(), 5);
    EXPECT_EQ(buffer, layout);
}

TEST(segment_tree_test, search)
{
    segment_tree<int> tree =  { 1, 2, 3, 4, 5 };

    EXPECT_EQ(tree.search(0, 1), 1);
    EXPECT_EQ(tree.search(1, 2), 2);
    EXPECT_EQ(tree.search(2, 3), 3);
    EXPECT_EQ(tree.search(3, 4), 4);
    EXPECT_EQ(tree.search(4, 5), 5);

    
    EXPECT_EQ(tree.search(0, 2), 3);
    EXPECT_EQ(tree.search(0, 3), 6);
    EXPECT_EQ(tree.search(0, 4), 10);
    EXPECT_EQ(tree.search(0, 5), 15);

    EXPECT_EQ(tree.search(1, 5), 14);
    EXPECT_EQ(tree.search(2, 5), 12);
    EXPECT_EQ(tree.search(3, 5), 9);
    EXPECT_EQ(tree.search(2, 4), 7);

    EXPECT_THROW(tree.search(0, 6), std::invalid_argument);
    EXPECT_THROW(tree.search(3, 3), std::invalid_argument);
    EXPECT_THROW(tree.search(4, 2), std::invalid_argument);
}

TEST(segment_tree_test, update)
{
    segment_tree<int> tree =  { 1, 2, 3, 4, 5 };

    {
        auto temp = tree;
        temp.update(0, 0);

        EXPECT_EQ(temp.search(0 ,1), 0);
        EXPECT_EQ(temp.search(0, 5), 14);
    }

    {
        auto temp = tree;
        temp.update(1, 0);

        EXPECT_EQ(temp.search(1, 2), 0);
        EXPECT_EQ(temp.search(0, 5), 13);
    }

    {
        auto temp = tree;
        temp.update(2, 0);

        EXPECT_EQ(temp.search(2, 3), 0);
        EXPECT_EQ(temp.search(0, 5), 12);
    }

    {
        auto temp = tree;
        temp.update(3, 0);

        EXPECT_EQ(temp.search(3, 4), 0);
        EXPECT_EQ(temp.search(0, 5), 11);
    }

    {
        auto temp = tree;
        temp.update(4, 0);

        EXPECT_EQ(temp.search(4, 5), 0);
        EXPECT_EQ(temp.search(0, 5), 10);
    }

    EXPECT_THROW(tree.update(5, 0), std::out_of_range);
}