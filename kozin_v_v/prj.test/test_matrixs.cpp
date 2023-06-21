#include <iostream>
#include <matrixs/matrixs.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

TEST_CASE("MatrixS constructors") {
    SUBCASE("Default constructor") {
        MatrixS m;
        CHECK(m.nRows() == 0);
        CHECK(m.nCols() == 0);
    }
    SUBCASE("Constructor with size tuple") {
        MatrixS m(MatrixS::SizeType{2, 3});
//        MatrixS m({2, 3});
        CHECK(m.nRows() == 2);
        CHECK(m.nCols() == 3);
    }
    SUBCASE("Constructor with two size arguments") {
        MatrixS m(2, 3);
        CHECK(m.nRows() == 2);
        CHECK(m.nCols() == 3);
    }
}

TEST_CASE("MatrixS copy constructor and assignment operator") {
    MatrixS m1(2, 3);
    m1.at(0, 0) = 1;
    m1.at(0, 1) = 2;
    m1.at(0, 2) = 3;
    m1.at(1, 0) = 4;
    m1.at(1, 1) = 5;
    m1.at(1, 2) = 6;


    SUBCASE("Copy constructor") {
        MatrixS m2(m1);
        CHECK(m2.nRows() == 2);
        CHECK(m2.nCols() == 3);
        CHECK(m2.at(0, 0) == 1);
        CHECK(m2.at(0, 1) == 2);
        CHECK(m2.at(0, 2) == 3);
        CHECK(m2.at(1, 0) == 4);
        CHECK(m2.at(1, 1) == 5);
        CHECK(m2.at(1, 2) == 6);
    }
    SUBCASE("Assignment operator") {
        MatrixS m2;
        m2 = m1;
        CHECK(m2.nRows() == 2);
        CHECK(m2.nCols() == 3);
        CHECK(m2.at(0, 0) == 1);
        CHECK(m2.at(0, 1) == 2);
        CHECK(m2.at(0, 2) == 3);
        CHECK(m2.at(1, 0) == 4);
        CHECK(m2.at(1, 1) == 5);
        CHECK(m2.at(1, 2) == 6);
    }
}

TEST_CASE("MatrixS at method") {
    MatrixS m(3, 4);

    SUBCASE("Non-const at with tuple argument") {
        m.at({0, 0}) = -5;
        CHECK(m.at({0, 0}) == -5);

        const MatrixS &cm = m;
        CHECK(cm.at({0, 0}) == -5);

    }
    SUBCASE("Non-const at with two arguments") {
        m.at(0, 0) = -5;
        CHECK(m.at(0, 0) == -5);
        const MatrixS &cm = m;
        CHECK(cm.at(0, 0) == -5);

    }
}
//
TEST_CASE("MatrixS resize method") {
    MatrixS m;

    SUBCASE("Resize with tuple argument") {
        m.resize({4, 4});
        CHECK(m.nRows() == 4);
        CHECK(m.nCols() == 4);

    }

    SUBCASE("Resize with two arguments") {
        m.resize(4, 4);
        CHECK(m.nRows() == 4);
        CHECK(m.nCols() == 4);

    }
}


    TEST_CASE("MatrixS initialization and resizing") {
        SUBCASE("Default initialization") {
            MatrixS matrix;
            CHECK(matrix.ssize() == std::make_tuple(0, 0));
        }

//        SUBCASE("Initialization with size") {
//            MatrixS matrix(MatrixS::SizeType{3, 4});
//            CHECK(matrix.ssize() == std::make_tuple(3, 4));
//            CHECK(matrix.nRows() == 3);
//            CHECK(matrix.nCols() == 4);
//        }
//
//        SUBCASE("Resizing") {
//            MatrixS matrix(MatrixS::SizeType{2, 2});
//            matrix.at(0, 0) = 1;
//            matrix.at(0, 1) = 2;
//            matrix.at(1, 0) = 3;
//            matrix.at(1, 1) = 4;
//
//            matrix.resize({3, 3});
//            CHECK(matrix.ssize() == std::make_tuple(3, 3));
//            CHECK(matrix.nRows() == 3);
//            CHECK(matrix.nCols() == 3);
//            CHECK(matrix.at(2, 2) == 0);
//            CHECK(matrix.at(1, 1) == 4);
//
//            matrix.resize(4, 4);
//            CHECK(matrix.ssize() == std::make_tuple(4, 4));
//            CHECK(matrix.nRows() == 4);
//            CHECK(matrix.nCols() == 4);
//            CHECK(matrix.at(3, 3) == 0);
//            CHECK(matrix.at(1, 1) == 4);
//        }
    }


//
//TEST_CASE("MatrixS element access") {
//    MatrixS matrix(MatrixS::SizeType{2, 3});
//
//    SUBCASE("Reading and writing elements with at()") {
//        matrix.at(0, 0) = 1;
//        matrix.at(0, 1) = 2;
//        matrix.at(0, 2) = 3;
//        matrix.at(1, 0) = 4;
//        matrix.at(1, 1) = 5;
//        matrix.at(1, 2) = 6;
//
//        CHECK(matrix.at(0, 0) == 1);
//        CHECK(matrix.at(0, 1) == 2);
//        CHECK(matrix.at(0, 2) == 3);
//        CHECK(matrix.at(1, 0) == 4);
//        CHECK(matrix.at(1, 1) == 5);
//        CHECK(matrix.at(1, 2) == 6);
//    }





























//    SUBCASE("Reading and writing elements with tuple") {
//        auto [m, n] = matrix.ssize();
//
//        for (std::ptrdiff_t i = 0; i < m; ++i) {
//            for (std::ptrdiff_t j = 0; j < n; ++j) {
//                matrix.at({i, j}) = i * n + j + 1;
//            }
//        }
//
//        CHECK(matrix.at({0, 0}) == 1);
//        CHECK(matrix.at({0, 1}) == 2);
//        CHECK(matrix.at({0, 2}) == 3);
//        CHECK(matrix.at({1, 0}) == 4);
//        CHECK(matrix.at({1, 1}) == 5);
//        CHECK(matrix.at({1, 2}) == 6);
//    }
//}



//
//TEST_CASE("MatrixS Tests") {
//    SUBCASE("Construction and Size") {
//        MatrixS matrix({3, 4});
//
//        CHECK(matrix.nRows() == 3);
//        CHECK(matrix.nCols() == 4);
//        CHECK(matrix.ssize() == std::make_tuple(3, 4));
//    }
//
//    SUBCASE("Element Access") {
//        MatrixS matrix({3, 3});
//        matrix.at(0, 0) = 1;
//        matrix.at(1, 2) = 2;
//        matrix.at(2, 1) = 3;
//
//        CHECK(matrix.at(0, 0) == 1);
//        CHECK(matrix.at(1, 2) == 2);
//        CHECK(matrix.at(2, 1) == 3);
//        CHECK_THROWS(matrix.at(-1, 2));
//        CHECK_THROWS(matrix.at(2, 3));
//    }
//
//    SUBCASE("Resize") {
//        MatrixS matrix({3, 3});
//        matrix.at(0, 0) = 1;
//        matrix.at(1, 2) = 2;
//        matrix.at(2, 1) = 3;
//        matrix.resize({4, 4});
//
//        CHECK(matrix.nRows() == 4);
//        CHECK(matrix.nCols() == 4);
//        CHECK(matrix.at(0, 0) == 1);
//        CHECK(matrix.at(1, 2) == 2);
//        CHECK(matrix.at(2, 1) == 3);
//
//        matrix.resize(2, 2);
//
//        CHECK(matrix.nRows() == 2);
//        CHECK(matrix.nCols() == 2);
//        CHECK_THROWS(matrix.at(2, 1));
//    }
//
//    SUBCASE("Copy Constructor and Assignment Operator") {
//        MatrixS matrix({3, 3});
//        matrix.at(0, 0) = 1;
//        matrix.at(1, 2) = 2;
//        matrix.at(2, 1) = 3;
//        MatrixS copy(matrix);
//        MatrixS assigned(2, 2);
//        assigned = matrix;
//
//        CHECK(copy.nRows() == 3);
//        CHECK(copy.nCols() == 3);
//        CHECK(copy.at(0, 0) == 1);
//        CHECK(copy.at(1, 2) == 2);
//        CHECK(copy.at(2, 1) == 3);
//
//        CHECK(assigned.nRows() == 3);
//        CHECK(assigned.nCols() == 3);
//        CHECK(assigned.at(0, 0) == 1);
//        CHECK(assigned.at(1, 2) == 2);
//        CHECK(assigned.at(2, 1) == 3);
//    }
//}


//
//TEST_CASE("MatrixS ssize method") {
//    MatrixS m{3, 4};
//    auto [rows, cols] = m.ssize();
//    CHECK(rows == 3);
//    CHECK(cols == 4);
//}


//#include <iostream>
//
//int main() {
//    std::ptrdiff_t a = 3;
//    std::ptrdiff_t b = 4;
//    MatrixS matrix(a, b);
//
//    std::cout << "Initial matrix:\n";
//    for (std::ptrdiff_t i = 0; i < matrix.nRows(); ++i) {
//        for (std::ptrdiff_t j = 0; j < matrix.nCols(); ++j) {
//            std::cout << matrix.at({i, j}) << ' ';
//        }
//        std::cout << '\n';
//    }
//
//
//
//    matrix.resize({2, 2});
//
//    std::cout << "Resized matrix:\n";
//    for (std::ptrdiff_t i = 0; i < matrix.nRows(); ++i) {
//        for (std::ptrdiff_t j = 0; j < matrix.nCols(); ++j) {
//            std::cout << matrix.at({i, j}) << ' ';
//        }
//        std::cout << '\n';
//    }
//
//    return 0;
//}