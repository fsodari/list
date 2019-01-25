#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <iostream>

extern "C"
{
#include "list/list.h"
}

typedef struct
{
    int dat1;
    int dat2;
}test_data;

TEST_GROUP(LIST)
{
    list dut;

    void setup()
    {
        dut = list_create(sizeof(test_data));
    }

    void teardown()
    {
        list_destroy(dut);
    }
};

TEST(LIST, append)
{
    int error;
    size_t size;
    test_data td;
    td.dat1 = 42;
    td.dat2 = 43;

    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        error = list_append(dut, &td);
        size = list_size(dut);
        CHECK_FALSE(error);
        CHECK_EQUAL(i+1, size);
    }
}

TEST(LIST, pop)
{
    int error;
    size_t size;
    test_data td = {42, 43};

    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_append(dut, &td);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }

    for (int i = 0; i < 10; i++) {
        error = list_pop(dut, &td);
        CHECK_FALSE(error);
        CHECK_EQUAL(i, td.dat1);
        size = list_size(dut);
        CHECK_EQUAL(9-i, size);
    }
}

TEST(LIST, get)
{
    int error;
    size_t size;
    test_data td = {42, 43};

    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_append(dut, &td);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }

    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(10, size);
        td.dat1 = -1;
        error = list_get(dut, &td, i);
        CHECK_FALSE(error);
        CHECK_EQUAL(i, td.dat1);
    }
}

TEST(LIST, set)
{
    int error;
    size_t size;
    test_data td = {42, 43};

    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_append(dut, &td);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }

    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(10, size);
        td.dat1 = 42;
        error = list_set(dut, &td, i);
        CHECK_FALSE(error);
        error = list_get(dut, &td, i);
        CHECK_FALSE(error);
        CHECK_EQUAL(42, td.dat1);
    }
}

TEST(LIST, empty)
{
    int error;
    size_t size;
    test_data td = {42, 43};

    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_append(dut, &td);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }

    error = list_empty(dut);
    CHECK_FALSE(error);
    size = list_size(dut);
    CHECK_EQUAL(0, size);

    // Do it again to make sure destroy works
    for (int i = 0; i < 20; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_append(dut, &td);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }
}

TEST(LIST, insert)
{
    int error;
    size_t size;
    test_data td = {42, 43};

    // Insert at head
    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_insert(dut, &td, 0);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }
    for (int i = 0; i < 10; i++) {
        error = list_get(dut, &td, i);
        CHECK_FALSE(error);
        CHECK_EQUAL(9-i, td.dat1);
    }

    error = list_empty(dut);
    CHECK_FALSE(error);

    // Insert at tail
    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_insert(dut, &td, i);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }
    for (int i = 0; i < 10; i++) {
        error = list_get(dut, &td, i);
        CHECK_FALSE(error);
        CHECK_EQUAL(i, td.dat1);
    }

    error = list_empty(dut);
    CHECK_FALSE(error);

    // Insert at tail
    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_insert(dut, &td, i);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }

    td.dat1 = 42;
    error = list_insert(dut, &td, 4);
    CHECK_FALSE(error);
    size = list_size(dut);
    CHECK_EQUAL(11, size);

    error = list_get(dut, &td, 3);
    CHECK_FALSE(error);
    CHECK_EQUAL(3, td.dat1);
    error = list_get(dut, &td, 4);
    CHECK_FALSE(error);
    CHECK_EQUAL(42, td.dat1);
    error = list_get(dut, &td, 5);
    CHECK_FALSE(error);
    CHECK_EQUAL(4, td.dat1);
}

TEST(LIST, remove)
{
    int error;
    size_t size;
    test_data td = {42, 43};

    // Insert at tail
    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_insert(dut, &td, i);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }

    // Remove from head
    for (int i = 0; i < 10; i++) {
        error = list_remove(dut, 0);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(9-i, size);
        for (int j = 0; j < size; j++) {
            error = list_get(dut, &td, j);
            CHECK_FALSE(error);
            CHECK_EQUAL(i+1+j, td.dat1);
        }
    }
    size = list_size(dut);
    CHECK_EQUAL(0, size);
    // Insert at tail
    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_insert(dut, &td, i);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }
    // Remove from head
    for (int i = 0; i < 10; i++) {
        error = list_remove(dut, 9-i);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(9-i, size);
        for (int j = 0; j < size; j++) {
            error = list_get(dut, &td, j);
            CHECK_FALSE(error);
            CHECK_EQUAL(j, td.dat1);
        }
    }

    // Insert at tail
    for (int i = 0; i < 10; i++) {
        size = list_size(dut);
        CHECK_EQUAL(i, size);
        td.dat1 = i;
        error = list_insert(dut, &td, i);
        CHECK_FALSE(error);
        size = list_size(dut);
        CHECK_EQUAL(i+1, size);
    }

    error = list_remove(dut, 4);
    CHECK_FALSE(error);
    size = list_size(dut);
    CHECK_EQUAL(9, size);

    error = list_get(dut, &td, 3);
    CHECK_FALSE(error);
    CHECK_EQUAL(3, td.dat1);
    error = list_get(dut, &td, 4);
    CHECK_FALSE(error);
    CHECK_EQUAL(5, td.dat1);
}

TEST(LIST, isempty)
{
    int error;
    size_t isempty;
    test_data td = {42, 43};

    isempty = list_isempty(dut);
    CHECK_EQUAL(1, isempty);

    // Insert at tail
    for (int i = 0; i < 10; i++) {
        td.dat1 = i;
        error = list_insert(dut, &td, i);
        CHECK_FALSE(error);
        isempty = list_isempty(dut);
        CHECK_EQUAL(0, isempty);
    }

    error = list_empty(dut);
    CHECK_FALSE(error);
    isempty = list_isempty(dut);
    CHECK_EQUAL(1, isempty);
}