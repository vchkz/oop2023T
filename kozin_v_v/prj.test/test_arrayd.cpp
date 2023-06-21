#include <arrayd/arrayd.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

TEST_CASE("Testing ArrayD class") {
    SUBCASE("Testing constructor and destructor") {
        ArrayD arr(3);
        CHECK(arr.ssize() == 3);
    }

    SUBCASE("Testing copy constructor and assignment operator") {
        ArrayD arr1(3);
        arr1[0] = 1.0;
        arr1[1] = 2.0;
        arr1[2] = 3.0;



//        std::cout << "-------" << std::endl;
//        for (int i = 0; i < arr1.ssize(); i++) {
//            std::cout << arr1[i] << std::endl;
//        }
//        std::cout << "-------" << std::endl;
//        arr1.resize(6);
//        for (int i = 0; i < arr1.ssize(); i++) {
//            std::cout << arr1[i] << std::endl;
//        }
//        std::cout << "-------" << std::endl;
//
//
//
//        arr1.resize(3);
//        arr1[0] = 1.0;
//        arr1[1] = 2.0;
//        arr1[2] = 3.0;




        ArrayD arr2(arr1);
        CHECK(arr2.ssize() == 3);
        CHECK(arr2[0] == 1.0);
        CHECK(arr2[1] == 2.0);
        CHECK(arr2[2] == 3.0);

        ArrayD arr3(2);
        arr3 = arr1;
        CHECK(arr3.ssize() == 3);
        CHECK(arr3[0] == 1.0);
        CHECK(arr3[1] == 2.0);
        CHECK(arr3[2] == 3.0);
    }

    SUBCASE("Testing subscript operator") {
        ArrayD arr(3);
        arr[0] = 1.0;
        arr[1] = 2.0;
        arr[2] = 3.0;

        CHECK(arr[0] == 1.0);
        CHECK(arr[1] == 2.0);
        CHECK(arr[2] == 3.0);

        ArrayD carr(3);
        carr[0] = 1.0;
        carr[1] = 2.0;
        carr[2] = 3.0;

        CHECK(carr[0] == 1.0);
        CHECK(carr[1] == 2.0);
        CHECK(carr[2] == 3.0);
    }
    SUBCASE("Testing ssize() method") {
        ArrayD arr(3);
        CHECK(arr.ssize() == 3);

        arr.resize(5);
        CHECK(arr.ssize() == 5);

        arr.insert(1, 4.0);
        CHECK(arr.ssize() == 6);

        arr.remove(1);
        CHECK(arr.ssize() == 5);
    }
    SUBCASE("Testing resize() method") {
        ArrayD arr(3);
        arr[0] = 1.0;
        arr[1] = 2.0;
        arr[2] = 3.0;

        arr.resize(5);
        CHECK(arr.ssize() == 5);
        CHECK(arr[0] == 1.0);
        CHECK(arr[1] == 2.0);
        CHECK(arr[2] == 3.0);

        arr.resize(2);
        CHECK(arr.ssize() == 2);
        CHECK(arr[0] == 1.0);
        CHECK(arr[1] == 2.0);
    }

    SUBCASE("Testing insert() method") {
        ArrayD arr(3);
        arr[0] = 1.0;
        arr[1] = 2.0;
        arr[2] = 3.0;

        arr.insert(1, 4.0);
        CHECK(arr.ssize() == 4);
        CHECK(arr[0] == 1.0);
        CHECK(arr[1] == 4.0);
        CHECK(arr[2] == 2.0);
        CHECK(arr[3] == 3.0);
    }

    SUBCASE("Testing remove() method") {
        ArrayD arr(3);
        arr[0] = 1.0;
        arr[1] = 2.0;
        arr[2] = 3.0;

        arr.remove(1);
        CHECK(arr.ssize() == 2);
        CHECK(arr[0] == 1.0);
        CHECK(arr[1] == 3.0);
    }
}


TEST_CASE("Test construct and copy") {
    SUBCASE("Test with normal size") {
        CHECK_NOTHROW(ArrayD(3));
    }
    SUBCASE("Test with size = 0") {
        CHECK_NOTHROW(ArrayD(0));
    }
    SUBCASE("Test with negative size") {
        CHECK_THROWS(ArrayD(-1));
    }
    SUBCASE("Test copy") {
        ArrayD x(1);
        x[0] = 1;
        ArrayD y = x;
        y[0] = 2;
        CHECK((x[0] != y[0]));
    }
}
TEST_CASE("Test with operator[]") {
    SUBCASE("Test with middle index") {
        ArrayD x(3);
        CHECK_NOTHROW(x[1] = 404);
        CHECK((x[1] == 404));
    }
    SUBCASE("Test with begin index") {
        ArrayD x(3);
        CHECK_NOTHROW(x[0] = 404);
        CHECK((x[0] == 404));
    }
    SUBCASE("Test with last index") {
        ArrayD x(3);
        CHECK_NOTHROW(x[2] = 404);
        CHECK((x[2] == 404));
    }
    SUBCASE("Test with out of range index > size") {
        ArrayD x(3);
        CHECK_THROWS(x[4]);
    }
    SUBCASE("Test with out of range index == size") {
        ArrayD x(3);
        CHECK_THROWS(x[3]);
    }
    SUBCASE("Test with out of range index < 0") {
        ArrayD x(3);
        CHECK_THROWS(x[-1]);
    }
}

TEST_CASE("Test of ssize, resize, insert, remove function") {
    SUBCASE("size == size") {
        CHECK((ArrayD(5).ssize() == 5));
    }
    SUBCASE("Resize to size+1") {
        ArrayD x(1);
        x.resize(x.ssize() + 1);
        CHECK_NOTHROW(x[1]);
    }
    SUBCASE("Resize to 0") {
        ArrayD x(1);
        CHECK_THROWS(x.resize(0));
    }
    SUBCASE("Resize to size < 0") {
        ArrayD x(1);
        CHECK_THROWS(x.resize(-1));
    }
    ArrayD test(3);
    ArrayD test_i(0);
    int size = test.ssize();
    for (int i = 0; i < size; i++) {
        test[i] = i + 1;
    }
    SUBCASE("insert to middle") {
        test_i = test;
        test_i.insert(1,4);
        CHECK((test_i[0] == test[0]));
        CHECK((test_i[1] == 4));
        CHECK((test_i[2] == test[1]));
        CHECK((test_i[3] == test[2]));
        CHECK((test_i.ssize() == size + 1));
    }
    SUBCASE("insert to begin") {
        test_i = test;
        test_i.insert(0,4);
        CHECK((test_i[0] == 4));
        CHECK((test_i[1] == test[0]));
        CHECK((test_i[2] == test[1]));
        CHECK((test_i[3] == test[2]));
        CHECK((test_i.ssize() == size + 1));
    }
    SUBCASE("insert to end") {
        test_i = test;
        test_i.insert(3,4);
        CHECK((test_i[0] == test[0]));
        CHECK((test_i[1] == test[1]));
        CHECK((test_i[2] == test[2]));
        CHECK((test_i[3] == 4));
        CHECK((test_i.ssize() == size + 1));
    }
    SUBCASE("insert to invalid position < 0") {
        test_i = test;
        CHECK_THROWS(test_i.insert(-1,4));
    }
    SUBCASE("insert to invalid position > size") {
        test_i = test;
        CHECK_THROWS(test_i.insert(4,4));
    }

    SUBCASE("remove from middle") {
        test_i = test;
        test_i.remove(1);
        CHECK((test_i[0] == test[0]));
        CHECK((test_i[1] == test[2]));
        CHECK((test_i.ssize() == size - 1));
    }
    SUBCASE("remove from begin") {
        test_i = test;
        test_i.remove(0);
        CHECK((test_i[0] == test[1]));
        CHECK((test_i[1] == test[2]));
        CHECK((test_i.ssize() == size - 1));
    }
    SUBCASE("remove from end") {
        test_i = test;
        test_i.remove(2);
        CHECK((test_i[0] == test[0]));
        CHECK((test_i[1] == test[1]));
        CHECK((test_i.ssize() == size - 1));
    }
    SUBCASE("remove from invalid position < 0") {
        test_i = test;
        CHECK_THROWS(test_i.remove(-1));
    }
    SUBCASE("remove from invalid position == size") {
        test_i = test;
        CHECK_THROWS(test_i.remove(3));
    }
    SUBCASE("remove from invalid position > size") {
        test_i = test;
        CHECK_THROWS(test_i.remove(4));
    }
}



