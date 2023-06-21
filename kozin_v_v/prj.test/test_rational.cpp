#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <rational/rational.hpp>

#include <sstream>
TEST_CASE("[rational] - Rational ctor") {
    CHECK(Rational() == Rational(0, 1));
    CHECK(Rational(3) == Rational(3, 1));
    CHECK(Rational(-3) == Rational(-3, 1));
    CHECK(Rational(10, 6) == Rational(5, 3));
    CHECK(Rational(-10, 6) == Rational(-5, 3));
    CHECK(Rational(10, -6) == Rational(-5, 3));
    CHECK(Rational(-10, -6) == Rational(5, 3));
    CHECK_THROWS(Rational(1, 0));
}


//TEST_CASE("[rational] - Rational ctor") {
////    CHECK(Rational() == Rational(0, 1));
////    CHECK(Rational(3) == Rational(3, 1));
////    CHECK(Rational(-3) == Rational(-3, 1));
////    CHECK(Rational(10, 6) == Rational(5, 3));
////    CHECK(Rational(-10, 6) == Rational(-5, 3));
////    CHECK(Rational(10, -6) == Rational(-5, 3));
////    CHECK(Rational(-10, -6) == Rational(5, 3));
////    CHECK_THROWS(Rational(1, 0));
//    std::cout<< Rational(-10, -6) << "-----------";
//}

TEST_CASE("[rational] - Rational add") {
    CHECK(Rational(3, 2) + Rational(5, 7) == Rational(31, 14));
    CHECK(Rational(5, 7) + Rational(3, 2) == Rational(31, 14));
    CHECK(Rational() + Rational() == Rational());
    CHECK(Rational() + Rational(29, 41) == Rational(29, 41));
    CHECK(Rational(5, 1) + Rational(3, 2) == Rational(13, 2));
}

TEST_CASE("[rational] - Rational subtract") {
    CHECK(Rational(31, 14) - Rational(5, 7) == Rational(3, 2));
    CHECK(Rational(25, 7) - Rational(2, 9) == Rational(211, 63));
    CHECK(Rational(2, 3) - Rational(7, 4) == Rational(-13, 12));
}

TEST_CASE("[rational] - Rational division") {
    CHECK(Rational(5, 2) / Rational(5, 2) == Rational(1, 1));
    CHECK(Rational(3, 2) / Rational(9, 8) == Rational(4, 3));
    CHECK_THROWS(Rational(3, 2) / Rational(5, 0));
}

TEST_CASE("[rational] - Rational minus") {
    CHECK(-Rational() == Rational());
    CHECK(Rational(-5, 3) == -Rational(5, 3));
    CHECK(Rational(25, 7) == -(-(Rational(25, 7))));
    CHECK(Rational(29, 41) == -Rational(-29, 41));
}

TEST_CASE("[rational] - I/O") {

    SUBCASE("Output") {

        std::stringstream sstrm;

        SUBCASE("Simple") {
            sstrm << Rational(23, 7);
            CHECK(sstrm.str() == "23/7");
        }

        sstrm.clear();

        SUBCASE("Default constructor") {
            sstrm << Rational();
            CHECK(sstrm.str() == "0/1");
        }

        sstrm.clear();


        SUBCASE("Negative") {
            sstrm << -Rational(3, 8);
            CHECK(sstrm.str() == "-3/8");
        }

        sstrm.clear();

        SUBCASE("Negative with reduce") {
            sstrm << Rational(-8, 2);
            CHECK(sstrm.str() == "-4/1");
        }

    }

    SUBCASE("Correct inputs") {

        Rational r;
        std::stringstream sstrm;

        SUBCASE("Simple") {
            sstrm.str("2/3");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::eofbit);
            CHECK(r == Rational(2, 3));
        }

        sstrm.clear();

        SUBCASE("Input with reducing") {
            sstrm.str("36/48");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::eofbit);
            CHECK(r == Rational(3, 4));
        }

        sstrm.clear();

        SUBCASE("Negative input") {
            sstrm.str("-8/7");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::eofbit);
            CHECK(r == Rational(-8, 7));
        }
    }

    SUBCASE("Correct inputs with extra spaces on begin and trash on end") {

        Rational r;
        std::stringstream sstrm;

        SUBCASE("Extra spaces on begin") {
            sstrm.str(" 1/3");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::eofbit);
            CHECK(r == Rational(1, 3));
        }

        sstrm.clear();

        SUBCASE("Extra extra spaces on begin") {
            sstrm.str("   -3/4");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::eofbit);
            CHECK(r == Rational(-3, 4));
        }

        sstrm.clear();

        SUBCASE("Extra spaces on both ends") {
            sstrm.str(" 5/9  ");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::goodbit);
            CHECK(r == Rational(5, 9));
        }

        sstrm.clear();

        SUBCASE("Extra spaces on both ends and negative number") {
            sstrm.str(" -3/9  ");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::goodbit);
            CHECK(r == Rational(-1, 3));
        }

        sstrm.clear();

        SUBCASE("Extra spaces on begin and trash on end") {
            sstrm.str(" 23/9abc");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::goodbit);
            CHECK(r == Rational(23, 9));
        }

        sstrm.clear();

        SUBCASE("Extra spaces on end") {
            sstrm.str("3/7 ");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::goodbit);
            CHECK(r == Rational(3, 7));
        }

        sstrm.clear();

        SUBCASE("Trash on end") {
            sstrm.str("9/2&");
            sstrm >> r;
            CHECK(sstrm.rdstate() == std::ios_base::goodbit);
            CHECK(r == Rational(9, 2));
        }
    }

    SUBCASE("Uncorrect inputs") {
        Rational r;
        std::stringstream sstrm;

        SUBCASE("Trash") {
            sstrm.str("kek");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

        sstrm.clear();

        SUBCASE("Bad delimiter") {
            sstrm.str("5\\3");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

        sstrm.clear();

        SUBCASE("Numerator is trash") {
            sstrm.str("a/3");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

        sstrm.clear();

        SUBCASE("Denominator is trash") {
            sstrm.str("3/*");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

        sstrm.clear();

        SUBCASE("Spaces between 1") {
            sstrm.str("3 /2");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

        sstrm.clear();

        SUBCASE("Spaces between 2") {
            sstrm.str("3/ 2");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

        sstrm.clear();

        SUBCASE("Spaces between 3") {
            sstrm.str("3 / 2");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

        sstrm.clear();

        SUBCASE("Denominator is negative") {
            sstrm.str("3/-2");
            sstrm >> r;
            CHECK(sstrm.fail());
            CHECK(!sstrm.bad());
        }

    }
}







TEST_CASE("Rational") {
    Rational num1{3, 4};
    Rational num2{1, 2};

    SUBCASE("Addition") {
        Rational sum = num1 + num2;
        Rational expected_sum{5, 4};
        CHECK(sum == expected_sum);


        num1 += num2;
        CHECK(num1 == expected_sum);
        Rational num1{3, 4};

        num1 += 2;
//        CHECK(num1 == expected_sum_with_int);
    }

//    SUBCASE("Subtraction") {
//        Rational diff = num1 - num2;
//        Rational expected_diff{1, 4};
//        CHECK(diff == expected_diff);
//
//        Rational diff_with_int = num1 - 2;
//        Rational expected_diff_with_int{-5, 4};
//        CHECK(diff_with_int == expected_diff_with_int);
//
//        num1 -= num2;
//        CHECK(num1 == expected_diff);
//        Rational num1{3, 4};
//
//        num1 -= 2;
//        CHECK(num1 == expected_diff_with_int);
//    }

//    SUBCASE("Multiplication") {
//        Rational prod = num1 * num2;
//        Rational expected_prod{3, 8};
//        CHECK(prod == expected_prod);
//
////        Rational prod_with_int = num1 * 2;
////        Rational expected_prod_with_int{3, 2};
////        CHECK(prod_with_int == expected_prod_with_int);
//
//        num1 *= num2;
//        CHECK(num1 == expected_prod);
//        Rational num1{3, 4};
//
//        num1 *= 2;
//        CHECK(num1 == expected_prod_with_int);
//    }

//    SUBCASE("Division") {
//        Rational quotient = num1 / num2;
//        Rational expected_quotient{3, 2};
//        CHECK(quotient == expected_quotient);
//
//        Rational quotient_with_int = num1 / 2;
//        Rational expected_quotient_with_int{3, 8};
//        CHECK(quotient_with_int == expected_quotient_with_int);
//
//        num1 /= num2;
//        CHECK(num1 == expected_quotient);
//        Rational num1{3, 4};
//
//        num1 /= 2;
//        CHECK(num1 == expected_quotient_with_int);
//    }

    SUBCASE("Unary Plus and Minus") {
        Rational num{3, 4};
        Rational expected_num{3, 4};
        std::cout << "~~~" << ' ' << num << std::endl;

        Rational pos = num;
        CHECK(pos == expected_num);
        std::cout << "~~~" << ' ' << num << std::endl;

        Rational neg = -num;
        Rational expected_neg{-3, 4};
        CHECK(neg == expected_neg);
        std::cout << "~~~" << ' ' << num << std::endl;

        num--;
        Rational expected_num_after_dec{-1, 4};
        CHECK(num == expected_num_after_dec);
        std::cout << "~~~" << ' ' << num << std::endl;

        num++;
        CHECK(num == expected_num);
    }

    SUBCASE("I/O Stream") {
        Rational num{3, 4};

        std::stringstream ss;
        ss << num;

        Rational num_from_stream;
        ss >> num_from_stream;

        CHECK(num_from_stream == num);
    }

    SUBCASE("Equality Operators") {
        Rational num1{3, 4};
        Rational num2{3, 4};
        Rational num3{1, 2};

        CHECK(num1 == num2);
        CHECK(num1 != num3);
    }
}
TEST_CASE("Rational operator+(const Rational& lhs, const Rational& rhs)"){
    Rational a(1,2);
    Rational b(1,3);
    Rational expected(5,6);
    CHECK(a + b == expected);
    Rational c(-2,5);
    Rational d(1,5);
    expected = Rational(-1,5);
    CHECK(c + d == expected);
}

TEST_CASE("Rational operator-(const Rational& lhs, const Rational& rhs)"){
    Rational a(1,2);
    Rational b(1,3);
    Rational expected(1,6);
    CHECK(a - b == expected);
    Rational c(-2,5);
    Rational d(1,5);
    expected = Rational(-3,5);
    CHECK(c - d == expected);
}

TEST_CASE("Rational operator*(const Rational& lhs, const Rational& rhs)"){
    Rational a(2,3);
    Rational b(3,4);
    Rational expected(1,2);
    CHECK(a * b == expected);
    Rational c(-2,5);
    Rational d(1,5);
    expected = Rational(-2,25);
    CHECK(c * d == expected);
}

TEST_CASE("Rational operator/(const Rational& lhs, const Rational& rhs)"){
    Rational a(2,3);
    Rational b(3,4);
    Rational expected(8,9);
    CHECK(a / b == expected);
    Rational c(-2,5);
    Rational d(1,5);
    expected = Rational(-2);
    CHECK(c / d == expected);
}

TEST_CASE("Rational operator+(const Rational& lhs, const int32_t& rhs)"){
    Rational a(2,3);
    int32_t b = 1;
    Rational expected(5,3);
//    CHECK(a + b == expected);
    Rational c(-2,5);
    int32_t d = 3;
    expected = Rational(13,5);
//    CHECK(c + d == expected);
}

TEST_CASE("Rational operator-(const Rational& lhs, const int32_t& rhs)"){
    Rational a(2,3);
    int32_t b = 1;
    Rational expected(-1,3);
//    CHECK(a - b == expected);
    Rational c(-2,5);
    int32_t d = 3;
    expected = Rational(-17,5);
//    CHECK(c - d == expected);
}

TEST_CASE("Rational operator-(const Rational& lhs)"){
    Rational a(2,3);
    Rational expected(-2,3);
    CHECK(-a == expected);
    Rational b(0,5);
    expected = Rational(0,1);
    CHECK(-b == expected);
}

TEST_CASE("Rational operator*(const Rational& lhs, const int32_t& rhs)"){
    Rational a(2,3);
    int32_t b = 3;
    Rational expected(2);
//    CHECK(a * b == expected);
    Rational c(-2,5);
    int32_t d = 4;
    expected = Rational(-8,5);
//    CHECK(c * d == expected);
}

TEST_CASE("Rational operator/(const Rational& lhs, const int32_t& rhs)"){
    Rational a(2,3);
    int32_t b = 4;
    Rational expected(1,6);
//    CHECK(a / b == expected);
    Rational c(-2,5);
    int32_t d = -2;
    expected = Rational(1,5);
//    CHECK(c / d == expected);
}
TEST_CASE("Rational operator+="){
    Rational a(2,4);
    int32_t b = 4;
    Rational expected(9, 2);
    a+=4;
    CHECK(a == expected);
    a-=4;
    CHECK(a == Rational(2,4));
    a+=b;
    CHECK(a == expected);
}
TEST_CASE("Rational comparison operators") {
    Rational r1{2, 4};
    Rational r2{4, 8};
    Rational r3{-1, 2};
    Rational r4{3, 4};

    SUBCASE("Equality operator") {
        CHECK(r1 == r2);
        CHECK_FALSE(r1 == r3);
        CHECK_FALSE(r1 == r4);
        CHECK_FALSE(r3 == r4);
    }

    SUBCASE("Inequality operator") {
        CHECK_FALSE(r1 != r2);
        CHECK(r1 != r3);
        CHECK(r1 != r4);
        CHECK(r3 != r4);
    }

    SUBCASE("Less than operator") {
        CHECK_FALSE(r1 < r2);
        CHECK(r3 < r1);
        std::cout << r3 << "-----" << r2 << std::endl;
        CHECK(r3 < r4);
        CHECK_FALSE(r2 < r1);
        CHECK_FALSE(r4 < r3);
    }

    SUBCASE("Greater than operator") {
        CHECK_FALSE(r1 > r2);
        CHECK(r1 > r3);
        CHECK_FALSE(r1 > r4);
        CHECK_FALSE(r3 > r4);
        CHECK(r4 > r3);
    }

    SUBCASE("Less than or equal to operator") {
        CHECK(r1 <= r2);
        CHECK(r3 <= r1);
        CHECK(r3 <= r4);
        CHECK_FALSE(r4 <= r3);
    }

    SUBCASE("Greater than or equal to operator") {
        CHECK(r1 >= r2);
        CHECK(r1 < r4);
        CHECK_FALSE(r3 >= r4);
        CHECK(r4 >= r3);
    }
}
TEST_CASE("Rational unary operators") {
    Rational r1{2, 4};
    Rational r2{-3, 5};

    SUBCASE("Unary minus operator") {
        CHECK((-r1) == Rational{-1, 2});
        CHECK((-r2) == Rational{3, 5});
    }

    SUBCASE("Prefix increment operator") {
        CHECK((++r1) == Rational{3, 2});
        CHECK((++r2) == Rational{2, 5});
    }

    SUBCASE("Prefix decrement operator") {
        CHECK((--r1) == Rational{-1, 2});
        CHECK((--r2) == Rational{-8, 5});
    }
}
TEST_CASE("Rational operators *=") {
    Rational r1{2, 4};
    Rational r2{-3, 5};

    SUBCASE("*=") {
        CHECK((-r1) == Rational{-1, 2});
        CHECK((-r2) == Rational{3, 5});
    }
    std::stringstream sstrm;

    SUBCASE("Simple") {
        Rational r1{-8, 3};
//        r1 = r1 * 3;

        sstrm << r1;

//        CHECK(sstrm.str() == "5/1");
//        CHECK(r1 == -8);
        if (r1 == Rational(-8)){std::cout << r1;}
//        std::cout << r1 << std::endl;

    }


}

TEST_CASE("Arithmetic tests for Rational class") {

    SUBCASE("Addition") {
        Rational r1{1, 2};
        Rational r2{3, 4};
        Rational r3{5, 4};
        Rational r4 = r1 + r2;
        REQUIRE(r4 == r3);

        Rational r5{2, 3};
        Rational r6{1, 6};
        Rational r7{5, 6};
        Rational r8 = r5 + r6;
        REQUIRE(r8 == r7);
    }

    SUBCASE("Subtraction") {
        Rational r1{5, 4};
        Rational r2{3, 4};
        Rational r3{1, 2};
        Rational r4 = r1 - r2;
        REQUIRE(r4 == r3);

        Rational r5{7, 6};
        Rational r6{1, 6};
        Rational r7{1};
        Rational r8 = r5 - r6;
        REQUIRE(r8 == r7);
    }

    SUBCASE("Multiplication") {
        Rational r1{3, 5};
        Rational r2{7, 9};
        Rational r3{21, 45};
        Rational r4 = r1 * r2;
        REQUIRE(r4 == r3);

        Rational r5{2, 3};
        Rational r6{5, 6};
        Rational r7{5, 9};
        Rational r8 = r5 * r6;
        REQUIRE(r8 == r7);
    }

    SUBCASE("Division") {
        Rational r1{5, 6};
        Rational r2{3, 4};
        Rational r3{10, 9};
        Rational r4 = r1 / r2;
        REQUIRE(r4 == r3);

        Rational r5{1, 2};
        Rational r6{3, 5};
        Rational r7{5, 6};
        Rational r8 = r5 / r6;
        REQUIRE(r8 == r7);
    }

    SUBCASE("Unary operators") {
        Rational r1{3, 4};
        Rational r2{-3, 4};
        REQUIRE(-r1 == r2);

        Rational r3{-1, 3};
        Rational r4{2, 3};
        REQUIRE(++r3 == r4);

        Rational r5{1, 3};
        Rational r6{-1, 3};
        REQUIRE(-r5 == r6);
    }

    SUBCASE("Compound assignment operators") {
        Rational r1{1, 2};
        Rational r2{3, 4};
        Rational r3{5, 4};
        r1 += r2;
        REQUIRE(r1 == r3);

        Rational r4{2, 3};
        Rational r5{1, 6};
        Rational r6{5, 6};
        r4 += r5;
        REQUIRE(r4 == r6);

        Rational r7{7, 6};
        Rational r8{1, 6};
        Rational r9{1};
        r7 -= r8;
        REQUIRE(r7 == r9);

        Rational r10{3, 5};
        Rational r11{7, 9};
        Rational r12{21, 45};
        r10 *= r11;
        REQUIRE(r10 == r12);
    }
}