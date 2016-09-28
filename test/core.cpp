// Copyright (c) 2016 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/pfr.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/type_index.hpp>

#include <iostream>
#include <typeinfo>
#include <tuple>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cstring>

using namespace boost::pfr;

template <std::size_t I, class T>
void print(T& f) {
    std::cout << flat_get<I>(f) << "\t\t"
        << typeid(flat_tuple_element_t<I, T>).name()
        << std::endl;
}
/*

struct make_my_life_harder { int a0; short a1; };
struct make_my_life_even_more_harder { unsigned int b0; unsigned short b1; make_my_life_harder cr;};
struct foo {
    unsigned char v0;
    unsigned int v1;
    unsigned short v2;
    unsigned long long v3;
    unsigned char v4and5[2];
    int v6;
    std::size_t v7;
    int* v8;
    const void* v9;
    int const**const volatile**volatile** v10;
    const double v11;
    make_my_life_harder v12and13;
    make_my_life_even_more_harder v14and15andv16and17;
};

void test_print() {
    foo f {
        'A', 11, 12, 13, {'B', 'C'}, 16, 17, 0, 0, 0, 30.1
        , {18, 19}
        , {20, 21, {22, 23}}
    };
    print<0>(f);  print<1>(f);  print<2>(f);
    print<3>(f);  print<4>(f);  print<5>(f);
    print<6>(f);  print<7>(f);  print<8>(f);
    print<9>(f);  print<10>(f); print<11>(f);
    print<12>(f); print<13>(f); print<14>(f);
    print<15>(f); print<16>(f); print<17>(f);
    static_assert(flat_tuple_size_v<foo> == 18, "failed tuple size check");

    int a[] = {0, 1, 2, 3};
    std::cout << '\n' << flat_get<1>(a) << std::endl;

    int b[2][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}};
    std::cout << flat_get<4>(b) << std::endl;

    int i = 777;
    std::cout << flat_get<0>(i) << std::endl;
}

void test_runtime(const foo& f) {
    BOOST_TEST_EQ( flat_get<0>(f), f.v0);
    BOOST_TEST_EQ( flat_get<1>(f), f.v1);
    BOOST_TEST_EQ( flat_get<2>(f), f.v2);
    BOOST_TEST_EQ( flat_get<3>(f), f.v3);
    BOOST_TEST_EQ( flat_get<4>(f), f.v4and5[0]);
    BOOST_TEST_EQ( flat_get<5>(f), f.v4and5[1]);
    BOOST_TEST_EQ( flat_get<6>(f), f.v6);
    BOOST_TEST_EQ( flat_get<7>(f), f.v7);
    BOOST_TEST_EQ( flat_get<8>(f), f.v8);
    BOOST_TEST_EQ( flat_get<9>(f), f.v9);
    BOOST_TEST_EQ( flat_get<10>(f), f.v10);
    BOOST_TEST( flat_get<11>(f) < f.v11 + 0.001); BOOST_TEST( flat_get<11>(f) > f.v11 - 0.001);
    BOOST_TEST_EQ( flat_get<12>(f), f.v12and13.a0);
    BOOST_TEST_EQ( flat_get<13>(f), f.v12and13.a1);
    BOOST_TEST_EQ( flat_get<14>(f), f.v14and15andv16and17.b0);
    BOOST_TEST_EQ( flat_get<15>(f), f.v14and15andv16and17.b1);
    BOOST_TEST_EQ( flat_get<16>(f), f.v14and15andv16and17.cr.a0);
    BOOST_TEST_EQ( flat_get<17>(f), f.v14and15andv16and17.cr.a1);
}

template <class T>
void test_compiletime() {
    constexpr T f{};
    static_assert(flat_tuple_size_v<foo> == 18, "failed tuple size check");
    static_assert( std::is_same< decltype(flat_get<0>(f)), decltype((f.v0))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<1>(f)), decltype((f.v1))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<2>(f)), decltype((f.v2))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<3>(f)), decltype((f.v3))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<4>(f)), decltype((f.v4and5[0]))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<5>(f)), decltype((f.v4and5[1]))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<6>(f)), decltype((f.v6))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<7>(f)), decltype((f.v7))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<8>(f)), decltype((f.v8))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<9>(f)), decltype((f.v9))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<10>(f)), decltype((f.v10))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<11>(f)), decltype((f.v11))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<12>(f)), decltype((f.v12and13.a0))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<13>(f)), decltype((f.v12and13.a1))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<14>(f)), decltype((f.v14and15andv16and17.b0))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<15>(f)), decltype((f.v14and15andv16and17.b1))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<16>(f)), decltype((f.v14and15andv16and17.cr.a0))>::value, "types missmatch");
    static_assert( std::is_same< decltype(flat_get<17>(f)), decltype((f.v14and15andv16and17.cr.a1))>::value, "types missmatch");
}

template <class T>
constexpr void test_compiletime_array() {
    {
        constexpr T f[20] = {0};
        static_assert(flat_tuple_size_v<decltype(f)> == 20, "failed tuple size check for array");
        static_assert( std::is_same< decltype(flat_get<0>(f)), T const&>::value, "types missmatch");
        static_assert( std::is_same< decltype(flat_get<19>(f)), T const&>::value, "types missmatch");
    }
    {
        constexpr T f[2][10] = {{0}};
        static_assert(flat_tuple_size_v<decltype(f)> == 20, "failed tuple size check for array");
        static_assert( std::is_same< decltype(flat_get<0>(f)), T const&>::value, "types missmatch");
        static_assert( std::is_same< decltype(flat_get<19>(f)), T const&>::value, "types missmatch");
    }
    {
        constexpr T f[2][5][2] = {{{0}}};
        static_assert(flat_tuple_size_v<decltype(f)> == 20, "failed tuple size check for array");
        static_assert( std::is_same< decltype(flat_get<0>(f)), T const&>::value, "types missmatch");
        static_assert( std::is_same< decltype(flat_get<19>(f)), T const&>::value, "types missmatch");
    }
}

void test_with_enums() {
    enum class my_enum: unsigned {
        VALUE1 = 17, VALUE2, VALUE3
    };
    struct my_struct { my_enum e; int i; short s; };
    my_struct s {my_enum::VALUE1, 10, 11};
    std::tuple<unsigned, int, short> t = flat_structure_to_tuple(s);
    BOOST_TEST_EQ(std::get<0>(t), 17u);
    BOOST_TEST_EQ(std::get<1>(t), 10);
    BOOST_TEST_EQ(std::get<2>(t), 11);

    flat_get<1>(s) = 101;
    BOOST_TEST_EQ(flat_get<1>(s), 101);
    flat_get<2>(s) = 111;
    BOOST_TEST_EQ(flat_get<2>(s), 111);

    BOOST_TEST(flat_structure_tie(s) == flat_structure_tie(s));
    BOOST_TEST(flat_structure_tie(s) == flat_structure_to_tuple(s));
    BOOST_TEST(flat_structure_tie(s) != t);
    flat_structure_tie(s) = t;
    BOOST_TEST_EQ(flat_get<0>(s), 17u);
    BOOST_TEST_EQ(flat_get<1>(s), 10);
    BOOST_TEST_EQ(flat_get<2>(s), 11);

    static_assert(std::is_same<
        int, flat_tuple_element_t<1, my_struct>
    >::value, "");

    static_assert(std::is_same<
        short, flat_tuple_element_t<2, my_struct>
    >::value, "");


    static_assert(std::is_same<
        const int, flat_tuple_element_t<1, const my_struct>
    >::value, "");

    static_assert(std::is_same<
        volatile short, flat_tuple_element_t<2, volatile my_struct>
    >::value, "");

    static_assert(std::is_same<
        const volatile short, flat_tuple_element_t<2, const volatile my_struct>
    >::value, "");

    static_assert(
        3 == flat_tuple_size_v<const volatile my_struct>,
        ""
    );
}*/

void test_comparable_struct() {
    struct comparable_struct {
        int i; short s; char data[50]; bool bl; int a,b,c,d,e,f;
    };
    using namespace flat_ops;

    comparable_struct s1 {0, 1, "Hello", false, 6,7,8,9,10,11};
    comparable_struct s2 = s1;
    comparable_struct s3 {0, 1, "Hello", false, 6,7,8,9,10,11111};
    BOOST_TEST(s1 == s2);
    BOOST_TEST(s1 <= s2);
    BOOST_TEST(s1 >= s2);
    BOOST_TEST(!(s1 != s2));
    BOOST_TEST(!(s1 == s3));
    BOOST_TEST(s1 != s3);
    BOOST_TEST(s1 < s3);
    BOOST_TEST(s3 > s2);
    BOOST_TEST(s1 <= s3);
    BOOST_TEST(s3 >= s2);

    std::cout << s1 << std::endl;

    comparable_struct s4;
    std::stringstream ss;
    ss.exceptions ( std::ios::failbit);
    ss << s1;
    ss >> s4;
    std::cout << s4 << std::endl;
    BOOST_TEST(s1 == s4);
    int i = 1, j = 2;
    BOOST_TEST_NE(i, j);
}

void test_empty_struct() {
    struct empty {};
    using namespace flat_ops;

    std::cout << empty{} << std::endl;

    BOOST_TEST(empty{} == empty{});
}


void test_pods_with_int_operators() {
    using namespace flat_ops;

    std::stringstream ss;
    ss << std::is_pod<int>{};
    int i = 0;
    ss >> i;
    BOOST_TEST_EQ(i, 1);
    std::cout << i << std::endl;
}


void test_struct_with_single_field() {
    struct f1 { int i; };
    using namespace flat_ops;

    std::stringstream ss;
    ss << f1{ 777 };
    f1 var{};
    ss >> var;
    BOOST_TEST_EQ(var.i, 777);
    BOOST_TEST(var == f1{ 777 });
    BOOST_TEST(var != f1{ 778 });

    BOOST_TEST(var <= f1{ 777 });
    BOOST_TEST(var <= f1{ 778 });
    BOOST_TEST(var < f1{ 778 });

    BOOST_TEST(var >= f1{ 777 });
    BOOST_TEST(var >= f1{ 776 });
    BOOST_TEST(var > f1{ 776 });
}

template <class Comparator>
void test_with_contatiners() {
    struct testing { bool b1, b2; int i; };
    struct testing2 { bool b1, b2; int i; };
    std::set<testing, Comparator > t{
        {true, true, 100},
        {false, true, 100},
        {true, false, 100},
        {true, true, 101}
    };

    BOOST_TEST(t.find({true, true, 100}) != t.end());
    BOOST_TEST_EQ(t.count({true, true, 100}), 1u);
    BOOST_TEST(t.find(testing2{true, true, 100}) != t.end());

    std::set<testing2, Comparator > t2{
        {true, true, 101},
        {true, true, 100},
        {true, false, 100},
        {false, true, 100}
    };

    BOOST_TEST(std::equal(t.begin(), t.end(), t2.begin(), t2.end(), flat_equal_to<>{}));
    BOOST_TEST(!std::equal(t.begin(), t.end(), t2.begin(), t2.end(), flat_not_equal<>{}));

    std::vector<testing> res;
    std::set_intersection(t.begin(), t.end(), t2.begin(), t2.end(),
    std::back_inserter(res), Comparator{});

    BOOST_TEST_EQ(res.size(), 4u);
}

void test_with_user_defined_constructor() {
    struct pr {
        int i;
        short s;

        pr() = default;
        pr(const pr&) = default;
        pr(pr&&) = default;
        pr(int ii, short is) noexcept
            : i(ii), s(is)
        {}
    };

    pr p{1, 2};

    //assert(flat_get<1>(p) == 2); // Compilation error
}

void test_hash() {
    struct almost_pair { int i; short s; };
    std::unordered_set<almost_pair, flat_hash<almost_pair>, flat_equal_to<> > s;
    s.insert({0, 1});
    s.insert({1, 0});
    s.insert({1, 1});

    BOOST_TEST_EQ(s.size(), 3u);
    flat_hash<almost_pair> hs;
    BOOST_TEST_NE(hs({0, 1}), hs({1, 0}));
    BOOST_TEST_EQ(hs({0, 1}), hs({0, 1}));
    BOOST_TEST_EQ(hs({1, 1}), hs({1, 1}));
    BOOST_TEST_NE(hs({0, 0}), hs({1, 1}));

    struct single_field { int i; };
    BOOST_TEST_NE(flat_hash<single_field>()({1}), std::hash<int>()(1));
    BOOST_TEST_NE(flat_hash<single_field>()({199}), std::hash<int>()(199));
}

// Test case by Lisa Lippincott
void test_alignment_with_neted_structure() {
    struct A0 {
        short s;
        char c;
    };

    struct B0 {
        A0 a;
        char c1;
        char c2;
    };

    B0 test_struct;
    std::memset(&test_struct, 0, sizeof(test_struct));
    test_struct.a.s = 0;
    test_struct.a.c = '1';
    test_struct.c1 = '2';
    test_struct.c2 = '3';
    BOOST_TEST_EQ(flat_get<0>(test_struct), 0);
    BOOST_TEST_EQ(flat_get<1>(test_struct), '1');
    BOOST_TEST_EQ(flat_get<2>(test_struct), '2');
    BOOST_TEST_EQ(flat_get<3>(test_struct), '3');

}


template <std::size_t... I>
void print(std::index_sequence<I...>) {
    struct A0 {
        short s;
        char c;
    };

    A0 a0 { 3, '4' };
    BOOST_TEST_EQ(boost::pfr::flat_get<0>(a0), 3);
    BOOST_TEST_EQ(boost::pfr::flat_get<1>(a0), '4');

    struct A1 {
        int i;
    };

    struct B1 {
        A1 a;
        int j;
    };

    B1 b1 { 5, 6 };
    BOOST_TEST_EQ(boost::pfr::flat_get<0>(b1), 5);
    BOOST_TEST_EQ(boost::pfr::flat_get<1>(b1), 6);

    struct B0 {
        A0 a;
        char c1;
        char c2;
    };

    typedef B0 type;
    typedef B0 T;

    using namespace boost::pfr::detail;
    constexpr auto a = flat_array_of_type_ids<T>();
    (void)a; // `a` is unused if T is an empty type

    constexpr auto skips = make_array(
        empty_or_sequence(
            size_t_<a.count_from_opening_till_matching_parenthis_seq(I, typeid_conversions::tuple_begin_tag, typeid_conversions::tuple_end_tag) >{}, size_t_<I>{}
        )...
    );

    constexpr auto indexes_uncleanuped = make_array(std::index_sequence<1 + I...>{});
    constexpr auto indexes_plus_1_and_zeros_as_skips = remove_skips(indexes_uncleanuped, skips);
    constexpr auto new_size = size_t_<indexes_plus_1_and_zeros_as_skips.count_nonzeros()>{};
    constexpr auto indexes = resize_dropping_zeros_and_decrementing(new_size, indexes_plus_1_and_zeros_as_skips);
    static_assert(indexes.data[0] == 0, "");
    static_assert(indexes.data[1] == 4, "");
    static_assert(indexes.data[2] == 5, "");

    static_assert(
        std::is_same<
            decltype(boost::pfr::detail::as_flat_tuple_impl<type>(
                std::make_index_sequence< decltype(boost::pfr::detail::flat_array_of_type_ids<type>())::size() >()
            )),
            boost::pfr::detail::sequence_tuple::tuple<boost::pfr::detail::sequence_tuple::tuple<short, char>, char, char>
        >::value,
        ""
    );
}

int main() {
 /*   test_compiletime<foo>();
    test_compiletime_array<int>();
    test_compiletime_array<void*>();
    test_compiletime_array<const void*>();
    test_compiletime_array<char>();
    test_compiletime_array<char const volatile*>();
    {
        foo f {
            'A', 11, 12, 13, {'B', 'C'}, 16, 17, 0, 0, 0, 30.1
            , {18, 19}
            , {20, 21, {22, 23}}
        };
        test_runtime(f);
    }
    {
        foo f {
            '\0', 12437, 1212, 13, {'1', '7'}, 163, 1723, 0, 0, 0, 3000.1
            , {-18, -19}
            , {656565, 65535, {-22, -23}}
        };
        test_runtime(f);
    }

    test_with_enums();
    test_comparable_struct();
    test_empty_struct();
    test_pods_with_int_operators();
    test_struct_with_single_field();

    test_with_contatiners<flat_less<>>();
    test_with_contatiners<flat_greater<>>();

    test_print();

    test_with_user_defined_constructor();
    test_hash();

    struct non_pod1 {
        std::string s;
        std::vector<int> v;
        int i;

        struct foo {
            std::string s2;
        } f;
    };
    static_assert(tuple_size<non_pod1>::value == 4, "Must not be a compile error");


    struct non_pod2 {
        unsigned ui1: 1;
        unsigned ui2: 2;

        std::string s;
        std::vector<int> v;
        int i;

        struct foo {
            std::string s2;
        } f;
    };
    static_assert(tuple_size<non_pod2>::value == 6, "Must not be a compile error even with bitfields");

    int i_2dimens[2][2] = {{10, 11}, {12, 13} };
    static_assert(tuple_size<decltype(i_2dimens)>::value == 4, "");
    static_assert(flat_tuple_size<decltype(i_2dimens)>::value == 4, "");
*/

    print(std::make_index_sequence<6>{});
    test_alignment_with_neted_structure();

    return boost::report_errors();
}

