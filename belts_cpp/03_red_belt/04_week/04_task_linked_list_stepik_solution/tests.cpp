#include "tests.h"

#include "linked_list.h"
#include "test_runner.h"

void TestPushFront() {
    LinkedList<int> list;

    list.PushFront(1);
    ASSERT_EQUAL(list.GetHead()->value, 1);
    list.PushFront(2);
    ASSERT_EQUAL(list.GetHead()->value, 2);
    list.PushFront(3);
    ASSERT_EQUAL(list.GetHead()->value, 3);

    const std::vector<int> expected = {3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);
}

void TestInsertAfter() {
    {
        LinkedList<std::string> list;

        list.PushFront("a");
        auto head = list.GetHead();
        ASSERT(head);
        ASSERT_EQUAL(head->value, "a");

        list.InsertAfter(head, "b");
        const std::vector<std::string> expected1 = {"a", "b"};
        ASSERT_EQUAL(ToVector(list), expected1);

        list.InsertAfter(head, "c");
        const std::vector<std::string> expected2 = {"a", "c", "b"};
        ASSERT_EQUAL(ToVector(list), expected2);
    }

    {
        LinkedList<std::string> list;
        list.PushFront("a");
        auto head = list.GetHead();
        list.InsertAfter(head, "b");
        ASSERT_EQUAL(head->value, "a");

        auto head2 = list.GetHead();
        list.InsertAfter(head2, "c");
        const std::vector<std::string> expected2 = {"a", "c", "b"};
        ASSERT_EQUAL(ToVector(list), expected2);

        auto last = list.GetHead()->next->next;
        list.InsertAfter(last, "d");
        const std::vector<std::string> expected3 = {"a", "c", "b", "d"};
        ASSERT_EQUAL(ToVector(list), expected3);

        list.PushFront("k");
        const std::vector<std::string> expected4 = {"k", "a", "c", "b", "d"};
        ASSERT_EQUAL(ToVector(list), expected4);

        list.InsertAfter(nullptr, "M");
        const std::vector<std::string> expected5 = {"M", "k", "a", "c", "b", "d"};
        ASSERT_EQUAL(ToVector(list), expected5);

        list.InsertAfter(list.GetHead()->next->next->next->next->next, "J");
        const std::vector<std::string> expected6 = {"M", "k", "a", "c", "b", "d", "J"};
        ASSERT_EQUAL(ToVector(list), expected6);

        list.RemoveAfter(list.GetHead());
        const std::vector<std::string> expected7 = {"M", "a", "c", "b", "d", "J"};
        ASSERT_EQUAL(ToVector(list), expected7);

        list.RemoveAfter(list.GetHead()->next);
        const std::vector<std::string> expected8 = {"M", "a", "b", "d", "J"};
        ASSERT_EQUAL(ToVector(list), expected8);

        list.PopFront();
        const std::vector<std::string> expected9 = {"a", "b", "d", "J"};
        ASSERT_EQUAL(ToVector(list), expected9);

        list.RemoveAfter(list.GetHead()->next->next->next);
        const std::vector<std::string> expected10 = {"a", "b", "d", "J"};
        ASSERT_EQUAL(ToVector(list), expected10);

        list.RemoveAfter(list.GetHead()->next->next);
        const std::vector<std::string> expected11 = {"a", "b", "d"};
        ASSERT_EQUAL(ToVector(list), expected11);

        list.RemoveAfter(nullptr);
        const std::vector<std::string> expected12 = {"b", "d"};
        ASSERT_EQUAL(ToVector(list), expected12);
    }
}

void TestRemoveAfter() {
    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.PushFront(i);
    }

    const std::vector<int> expected = {5, 4, 3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);

    auto next_to_head = list.GetHead()->next;
    list.RemoveAfter(next_to_head);  // удаляем 3
    list.RemoveAfter(next_to_head);  // удаляем 2

    const std::vector<int> expected1 = {5, 4, 1};
    ASSERT_EQUAL(ToVector(list), expected1);

    while (list.GetHead()->next) {
        list.RemoveAfter(list.GetHead());
    }
    ASSERT_EQUAL(list.GetHead()->value, 5);
}

void TestPopFront() {
    {
        LinkedList<int> list;

        for (int i = 1; i <= 5; ++i) {
            list.PushFront(i);
        }
        for (int i = 1; i <= 5; ++i) {
            list.PopFront();
        }
        ASSERT(list.GetHead() == nullptr);
    }

    {
        LinkedList<int> list;
        list.PushFront(22);
        list.PopFront();
        list.PopFront();

        ASSERT(list.GetHead() == nullptr);
    }

    {
        LinkedList<int> list;
        list.PopFront();
        list.PopFront();

        ASSERT(list.GetHead() == nullptr);
    }
}