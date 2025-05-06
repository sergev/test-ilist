#include <stdexcept>
#include <sstream>
#include "ilist.h"

#include <gtest/gtest.h>

struct TestNode {
    int value;
    ilist_head list;
};

class IListTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ILIST_INIT(&head);
        nodes = new TestNode[10];
        for (int i = 0; i < 10; i++) {
            nodes[i].value = i;
            ILIST_INIT(&nodes[i].list);
        }
    }

    void TearDown() override { delete[] nodes; }

    ilist_head head;
    TestNode *nodes;
};

TEST_F(IListTest, InitAndEmpty)
{
    EXPECT_TRUE(ilist_empty(&head));
    EXPECT_FALSE(ilist_is_singular(&head));
}

TEST_F(IListTest, AddSingleNode)
{
    ilist_add(&nodes[0].list, &head);
    EXPECT_FALSE(ilist_empty(&head));
    EXPECT_TRUE(ilist_is_singular(&head));
    EXPECT_TRUE(ilist_is_first(&nodes[0].list, &head));
    EXPECT_TRUE(ilist_is_last(&nodes[0].list, &head));

    TestNode *entry = ilist_first_entry(&head, TestNode, list);
    EXPECT_EQ(entry, &nodes[0]);
    EXPECT_EQ(entry->value, 0);
}

TEST_F(IListTest, AddMultipleNodes)
{
    ilist_add(&nodes[0].list, &head);
    ilist_add(&nodes[1].list, &head);
    ilist_add(&nodes[2].list, &head);

    EXPECT_FALSE(ilist_empty(&head));
    EXPECT_FALSE(ilist_is_singular(&head));

    TestNode *entry = ilist_first_entry(&head, TestNode, list);
    EXPECT_EQ(entry->value, 2);
    entry = ilist_next_entry(entry, list);
    EXPECT_EQ(entry->value, 1);
    entry = ilist_next_entry(entry, list);
    EXPECT_EQ(entry->value, 0);
}

TEST_F(IListTest, AddTail)
{
    ilist_add_tail(&nodes[0].list, &head);
    ilist_add_tail(&nodes[1].list, &head);

    TestNode *entry = ilist_first_entry(&head, TestNode, list);
    EXPECT_EQ(entry->value, 0);
    entry = ilist_next_entry(entry, list);
    EXPECT_EQ(entry->value, 1);
}

TEST_F(IListTest, AddBeforeTail)
{
    ilist_add_tail(&nodes[0].list, &head);
    ilist_add_tail(&nodes[1].list, &head);
    ilist_add_before_tail(&nodes[2].list, &head);

    TestNode *entry = ilist_first_entry(&head, TestNode, list);
    EXPECT_EQ(entry->value, 0);
    entry = ilist_next_entry(entry, list);
    EXPECT_EQ(entry->value, 2);
    entry = ilist_next_entry(entry, list);
    EXPECT_EQ(entry->value, 1);
}

TEST_F(IListTest, DeleteNode)
{
    ilist_add(&nodes[0].list, &head);
    ilist_add(&nodes[1].list, &head);
    ilist_add(&nodes[2].list, &head);

    ilist_del(&nodes[1].list);
    EXPECT_EQ(nodes[1].list.next, 0);
    EXPECT_EQ(nodes[1].list.prev, 0);

    TestNode *entry = ilist_first_entry(&head, TestNode, list);
    EXPECT_EQ(entry->value, 2);
    entry = ilist_next_entry(entry, list);
    EXPECT_EQ(entry->value, 0);
}

TEST_F(IListTest, ForEachEntry)
{
    ilist_add(&nodes[0].list, &head);
    ilist_add(&nodes[1].list, &head);
    ilist_add(&nodes[2].list, &head);

    int values[] = { 2, 1, 0 };
    int i        = 0;
    TestNode *node;
    ilist_for_each_entry(node, &head, list)
    {
        EXPECT_EQ(node->value, values[i++]);
    }
    EXPECT_EQ(i, 3);
}

TEST_F(IListTest, DeleteAllNodes)
{
    ilist_add(&nodes[0].list, &head);
    ilist_add(&nodes[1].list, &head);
    ilist_del(&nodes[0].list);
    ilist_del(&nodes[1].list);
    EXPECT_TRUE(ilist_empty(&head));
}

TEST_F(IListTest, SingularList)
{
    ilist_add(&nodes[0].list, &head);
    EXPECT_TRUE(ilist_is_singular(&head));
    ilist_add(&nodes[1].list, &head);
    EXPECT_FALSE(ilist_is_singular(&head));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
