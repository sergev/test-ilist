Unit tests for ilist.h, created by Grok.

https://grok.com/share/bGVnYWN5_f7946f4f-61ab-4563-a7b9-2a88c19cd53c

# Prerequisites

    sudo apt install cmake

# Build

    make

# Run all tests

    $ make test
    ...
    1/9 Test #2: IListTest.AddSingleNode ..........***Failed    0.00 sec
    2/9 Test #1: IListTest.InitAndEmpty ...........   Passed    0.00 sec
    3/9 Test #9: IListTest.SingularList ...........   Passed    0.00 sec
    4/9 Test #3: IListTest.AddMultipleNodes .......***Failed    0.00 sec
    5/9 Test #4: IListTest.AddTail ................***Failed    0.00 sec
    6/9 Test #5: IListTest.AddBeforeTail ..........***Failed    0.00 sec
    7/9 Test #6: IListTest.DeleteNode .............***Failed    0.00 sec
    8/9 Test #8: IListTest.DeleteAllNodes .........***Failed    0.00 sec
    9/9 Test #7: IListTest.ForEachEntry ...........***Exception: SegFault  0.01 sec

    22% tests passed, 7 tests failed out of 9

    Total Test time (real) =   0.01 sec

    The following tests FAILED:
          2 - IListTest.AddSingleNode (Failed)
          3 - IListTest.AddMultipleNodes (Failed)
          4 - IListTest.AddTail (Failed)
          5 - IListTest.AddBeforeTail (Failed)
          6 - IListTest.DeleteNode (Failed)
          7 - IListTest.ForEachEntry (SEGFAULT)
          8 - IListTest.DeleteAllNodes (Failed)

# Show mismatches of one test

Let's take a look at issues of test AddSingleNode.

    $ cd build
    $ ctest -V -R AddSingleNode
    ...
    2: ilist_test.cc:39: Failure
    2: Value of: ((&nodes[0].list) == &(&head)[(&head)->next])
    2:   Actual: false
    2: Expected: true
    2:
    2: ilist_test.cc:40: Failure
    2: Value of: ((&nodes[0].list) == &(&head)[(&head)->prev])
    2:   Actual: false
    2: Expected: true
    2:
    2: ilist_test.cc:43: Failure
    2: Expected equality of these values:
    2:   entry
    2:     Which is: 0x600002cd4050
    2:   &nodes[0]
    2:     Which is: 0x600000fd4050
    2:
    2: ilist_test.cc:44: Failure
    2: Expected equality of these values:
    2:   entry->value
    2:     Which is: 36519936
    2:   0
    ...

There are four mismatches:

 * Line 39: EXPECT_TRUE(ilist_is_first(&nodes[0].list, &head));
 * Line 40: EXPECT_TRUE(ilist_is_last(&nodes[0].list, &head));
 * Line 43: EXPECT_EQ(entry, &nodes[0]);
 * Line 44: EXPECT_EQ(entry->value, 0);

Something is wrong in ilist implementation.
