/*
 * test_string.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#include <doctest/doctest.h>

#include "st_primitives.h"
#include "type/string.h"

TEST_SUITE("type::string")
{
    TEST_CASE("ST_STRING macro")
    {
        SUBCASE("char array")
        {
            st_string_t _;
            st_char str[] = "Hello, world!";
            _ = ST_STRING(str);
            _ = ST_STRING(str, 13);
            CHECK_EQ(str, _.data);
            CHECK_EQ(13,  _.len);
        }
        SUBCASE("char pointer")
        {
            st_string_t _;
            st_char str[] = "Hello, world!";
            st_char *ptr = str;
            _ = ST_STRING(ptr, ST_SIZEOF(str) - 1);
            CHECK_EQ(ptr, _.data);
            CHECK_EQ(str, _.data);
            CHECK_EQ(13,  _.len);
            _ = ST_STRING(ptr, 13);
            CHECK_EQ(ptr, _.data);
            CHECK_EQ(str, _.data);
            CHECK_EQ(13,  _.len);
        }
    }
    TEST_CASE("string length")
    {
        SUBCASE("empty string with implicit length")
        {
            st_string_t _;
            _ = ST_STRING_EMPTY;
            CHECK_EQ(0, _.len);
            CHECK(   st_string_empty(_));
            _ = ST_STRING("");
            CHECK_EQ(0, _.len);
            CHECK(   st_string_empty(_));
        }
        SUBCASE("empty string with explicit length")
        {
            st_string_t _;
            st_char null_terminator[1] = { 0x0 };
            st_char *ptr = null_terminator;
            _ = ST_STRING(null_terminator);
            CHECK_EQ(0, _.len);
            CHECK(   st_string_empty(_));
            _ = ST_STRING(null_terminator, 1);
            CHECK_EQ( 1, _.len);
            CHECK(   !st_string_empty(_));
            _ = ST_STRING(ptr, 1);
            CHECK_EQ( 1, _.len);
            CHECK(   !st_string_empty(_));
        }
        SUBCASE("non-empty string with implicit length")
        {
            st_string_t _;
            _ = ST_STRING("C strings!");
            CHECK_EQ( 10, _.len);
            CHECK(   !st_string_empty(_));
            _ = ST_STRING("a");
            CHECK_EQ( 1, _.len);
            CHECK(   !st_string_empty(_));
        }
        SUBCASE("non-empty string with explicit length")
        {
            st_string_t _;
            _ = ST_STRING("C strings!", 11);
            CHECK_EQ( 11, _.len);
            CHECK(   !st_string_empty(_));
            _ = ST_STRING("a", 2);
            CHECK_EQ( 2, _.len);
            CHECK(   !st_string_empty(_));
        }
    }
    TEST_CASE("string equality")
    {
        SUBCASE("equal empty strings")
        {
            st_string_t a = ST_STRING_EMPTY;
            st_string_t b = ST_STRING_EMPTY;
            CHECK(   st_string_equal(a, b));
            CHECK(   st_string_equal_ignore_case(a, b));
            CHECK_EQ(0, st_string_compare(a, b));
            CHECK_EQ(0, st_string_compare(b, a));
        }
        SUBCASE("non-empty string not equal to empty string")
        {
            st_string_t a = ST_STRING("C string!");
            st_string_t b = ST_STRING_EMPTY;
            CHECK(   !st_string_equal(a, b));
            CHECK(   !st_string_equal_ignore_case(a, b));
            CHECK_LT( 0, st_string_compare(a, b));
            CHECK_GT( 0, st_string_compare(b, a));
        }
        SUBCASE("equal non-empty strings")
        {
            st_string_t a = ST_STRING("C string!");
            st_string_t b = ST_STRING("C string!");
            CHECK(   st_string_equal(a, b));
            CHECK(   st_string_equal_ignore_case(a, b));
            CHECK_EQ(0, st_string_compare(a, b));
            CHECK_EQ(0, st_string_compare(b, a));
        }
        SUBCASE("equal strings one character short")
        {
            st_string_t a = ST_STRING("abcdefg");
            st_string_t b = ST_STRING("abcdef");
            CHECK(   !st_string_equal(a, b));
            CHECK(   !st_string_equal_ignore_case(a, b));
            CHECK_NE( a.len,            b.len);
            CHECK_LT( 0,                st_string_compare(a, b));
            CHECK_GT( 0,                st_string_compare(b, a));
        }
        SUBCASE("equal strings one character different")
        {
            st_string_t a = ST_STRING("abcdefg");
            st_string_t b = ST_STRING("abc#efg");
            CHECK(   !st_string_equal(a, b));
            CHECK(   !st_string_equal_ignore_case(a, b));
            CHECK_EQ( a.len, b.len);
            CHECK_LT( 0,                st_string_compare(a, b));
            CHECK_GT( 0,                st_string_compare(b, a));
        }
        SUBCASE("equal strings ignoring case")
        {
            st_string_t a = ST_STRING("aBcDeFg UPPERCASE lowercase");
            st_string_t b = ST_STRING("AbCdEfG uppercase LOWERCASE");
            CHECK(   !st_string_equal(a, b));
            CHECK(    st_string_equal_ignore_case(a, b));
            CHECK_EQ( a.len,            b.len);
            CHECK_LT( 0,                st_string_compare(a, b));
            CHECK_GT( 0,                st_string_compare(b, a));
        }
    }
    TEST_CASE("string search")
    {
        SUBCASE("starts with")
        {
            CHECK( st_string_starts_with(ST_STRING_EMPTY,      ST_STRING_EMPTY));
            CHECK( st_string_starts_with(ST_STRING("a"),       ST_STRING_EMPTY));
            CHECK(!st_string_starts_with(ST_STRING_EMPTY,      ST_STRING("a")));
            CHECK( st_string_starts_with(ST_STRING("a"),       ST_STRING("a")));
            CHECK( st_string_starts_with(ST_STRING("aa"),      ST_STRING("a")));
            CHECK( st_string_starts_with(ST_STRING("abcdefg"), ST_STRING("a")));
            CHECK( st_string_starts_with(ST_STRING("abcdefg"), ST_STRING("abc")));
            CHECK( st_string_starts_with(ST_STRING("abcdefg"), ST_STRING("abcdef")));
            CHECK( st_string_starts_with(ST_STRING("abcdefg"), ST_STRING("abcdefg")));
            CHECK(!st_string_starts_with(ST_STRING("abcdefg"), ST_STRING("abcdeg")));
            CHECK(!st_string_starts_with(ST_STRING("abcdefg"), ST_STRING("bcd")));
            CHECK(!st_string_starts_with(ST_STRING("abcdefg"), ST_STRING("bcdefg")));
            CHECK(!st_string_starts_with(ST_STRING("a"),       ST_STRING("abc")));
            CHECK(!st_string_starts_with(ST_STRING("a"),       ST_STRING("abcdefg")));
            CHECK(!st_string_starts_with(ST_STRING("a"),       ST_STRING("aa")));
        }
        SUBCASE("ends with")
        {
            CHECK( st_string_ends_with(ST_STRING_EMPTY,      ST_STRING_EMPTY));
            CHECK( st_string_ends_with(ST_STRING("a"),       ST_STRING_EMPTY));
            CHECK(!st_string_ends_with(ST_STRING_EMPTY,      ST_STRING("a")));
            CHECK( st_string_ends_with(ST_STRING("a"),       ST_STRING("a")));
            CHECK( st_string_ends_with(ST_STRING("aa"),      ST_STRING("a")));
            CHECK( st_string_ends_with(ST_STRING("abcdefg"), ST_STRING("g")));
            CHECK( st_string_ends_with(ST_STRING("abcdefg"), ST_STRING("efg")));
            CHECK( st_string_ends_with(ST_STRING("abcdefg"), ST_STRING("bcdefg")));
            CHECK( st_string_ends_with(ST_STRING("abcdefg"), ST_STRING("abcdefg")));
            CHECK(!st_string_ends_with(ST_STRING("abcdefg"), ST_STRING("bcdefa")));
            CHECK(!st_string_ends_with(ST_STRING("abcdefg"), ST_STRING("def")));
            CHECK(!st_string_ends_with(ST_STRING("abcdefg"), ST_STRING("abcdef")));
            CHECK(!st_string_ends_with(ST_STRING("a"),       ST_STRING("abc")));
            CHECK(!st_string_ends_with(ST_STRING("a"),       ST_STRING("abcdefg")));
            CHECK(!st_string_ends_with(ST_STRING("a"),       ST_STRING("aa")));
        }
        SUBCASE("contains")
        {
            CHECK( st_string_contains(ST_STRING_EMPTY,     ST_STRING_EMPTY));
            CHECK(!st_string_contains(ST_STRING_EMPTY,     ST_STRING("abc")));
            CHECK( st_string_contains(ST_STRING("abc"),    ST_STRING_EMPTY));
            CHECK( st_string_contains(ST_STRING("abc"),    ST_STRING("abc")));
            CHECK( st_string_contains(ST_STRING("abc"),    ST_STRING("a")));
            CHECK( st_string_contains(ST_STRING("abc"),    ST_STRING("ab")));
            CHECK( st_string_contains(ST_STRING("abc"),    ST_STRING("bc")));
            CHECK( st_string_contains(ST_STRING("abc"),    ST_STRING("c")));
            CHECK(!st_string_contains(ST_STRING("abc"),    ST_STRING("d")));
            CHECK(!st_string_contains(ST_STRING("a"),      ST_STRING("aa")));
            CHECK(!st_string_contains(ST_STRING("a"),      ST_STRING("abc")));
            CHECK(!st_string_contains(ST_STRING("needle"), ST_STRING("haystack")));

            CHECK( st_string_contains_char(ST_STRING("abc"),    'a'));
            CHECK( st_string_contains_char(ST_STRING("abc"),    'b'));
            CHECK( st_string_contains_char(ST_STRING("abc"),    'c'));
            CHECK(!st_string_contains_char(ST_STRING("abc"),    'd'));
            CHECK(!st_string_contains_char(ST_STRING_EMPTY,     'a'));
            CHECK( st_string_contains_char(ST_STRING("abc", 4), 0x0));
        }
        SUBCASE("find")
        {
            CHECK_EQ(0            , st_string_find(ST_STRING_EMPTY,             ST_STRING_EMPTY));
            CHECK_EQ(ST_INDEX_NONE, st_string_find(ST_STRING_EMPTY,             ST_STRING("abc")));
            CHECK_EQ(0,             st_string_find(ST_STRING("abcdefg"),        ST_STRING_EMPTY));
            CHECK_EQ(0,             st_string_find(ST_STRING("abcdefg"),        ST_STRING("")));
            CHECK_EQ(ST_INDEX_NONE, st_string_find(ST_STRING("abcdefg"),        ST_STRING("abcdefghijk")));
            CHECK_EQ(0,             st_string_find(ST_STRING("abcdefg"),        ST_STRING("a")));
            CHECK_EQ(3,             st_string_find(ST_STRING("hayneedlestack"), ST_STRING("needle")));
            CHECK_EQ(ST_INDEX_NONE, st_string_find(ST_STRING("haystack"),       ST_STRING("needle")));
            CHECK_EQ(2,             st_string_find(ST_STRING("abc"),            ST_STRING("c")));
            CHECK_EQ(2,             st_string_find(ST_STRING("abcdef"),         ST_STRING("cdef")));
            CHECK_EQ(0,             st_string_find(ST_STRING("abcabcabc"),      ST_STRING("abc")));

            CHECK_EQ(ST_INDEX_NONE, st_string_find_char(ST_STRING_EMPTY,  'a'));
            CHECK_EQ(ST_INDEX_NONE, st_string_find_char(ST_STRING("abc"), 'd'));
            CHECK_EQ(0,             st_string_find_char(ST_STRING("a"),   'a'));
            CHECK_EQ(0,             st_string_find_char(ST_STRING("abc"), 'a'));
            CHECK_EQ(2,             st_string_find_char(ST_STRING("abc"), 'c'));
            CHECK_EQ(0,             st_string_find_char(ST_STRING("cac"), 'c'));
        }
        SUBCASE("find last")
        {
            CHECK_EQ(0            , st_string_find_last(ST_STRING_EMPTY,             ST_STRING_EMPTY));
            CHECK_EQ(ST_INDEX_NONE, st_string_find_last(ST_STRING_EMPTY,             ST_STRING("abc")));
            CHECK_EQ(0,             st_string_find_last(ST_STRING("a"),              ST_STRING("a")));
            CHECK_EQ(1,             st_string_find_last(ST_STRING("a"),              ST_STRING_EMPTY));
            CHECK_EQ(1,             st_string_find_last(ST_STRING("a"),              ST_STRING("")));
            CHECK_EQ(2,             st_string_find_last(ST_STRING("aaa"),            ST_STRING("a")));
            CHECK_EQ(3,             st_string_find_last(ST_STRING("hayneedlestack"), ST_STRING("needle")));
            CHECK_EQ(ST_INDEX_NONE, st_string_find_last(ST_STRING("haystack"),       ST_STRING("needle")));
            CHECK_EQ(2,             st_string_find_last(ST_STRING("abcdef"),         ST_STRING("cdef")));
            CHECK_EQ(6,             st_string_find_last(ST_STRING("abcabcabc"),      ST_STRING("abc")));

            CHECK_EQ(ST_INDEX_NONE, st_string_find_last_char(ST_STRING_EMPTY,  'a'));
            CHECK_EQ(ST_INDEX_NONE, st_string_find_last_char(ST_STRING("abc"), 'd'));
            CHECK_EQ(0,             st_string_find_last_char(ST_STRING("a"),   'a'));
            CHECK_EQ(0,             st_string_find_last_char(ST_STRING("abc"), 'a'));
            CHECK_EQ(2,             st_string_find_last_char(ST_STRING("abc"), 'c'));
            CHECK_EQ(2,             st_string_find_last_char(ST_STRING("cac"), 'c'));
        }
    }
    TEST_CASE("string sub-views")
    {
        SUBCASE("substring")
        {
            st_string_t _ = ST_STRING("abcdefABCDEF1234567890");
            CHECK(   st_string_equal(ST_STRING_EMPTY,     st_string_substring(_, 0, 0)));
            CHECK(   st_string_equal(ST_STRING_EMPTY,     st_string_substring(_, 6, 0)));
            CHECK(   st_string_equal(ST_STRING("abcdef"), st_string_substring(_, 0, 6)));
            CHECK(   st_string_equal(ST_STRING("ABCDEF"), st_string_substring(_, 6, 6)));
            CHECK(   st_string_equal(ST_STRING("123456"), st_string_substring(_, 12, 6)));
            CHECK(   st_string_equal(ST_STRING("7890"),   st_string_substring(_, 18, 4)));
            CHECK(   st_string_equal(_,                st_string_substring(_, 0, 9999)));
            CHECK_EQ(_.len,                                   st_string_substring(_, 0, 9999).len);
        }
        SUBCASE("left")
        {
            st_string_t _ = ST_STRING("abcdefABCDEF1234567890");
            CHECK(   st_string_equal(ST_STRING_EMPTY,                 st_string_left(_, 0)));
            CHECK(   st_string_equal(_,                            st_string_left(_, 9999)));
            CHECK(   st_string_equal(ST_STRING("abcdefABCDEF123456"), st_string_left(_, 18)));
            CHECK(   st_string_equal(ST_STRING("abcdef"),             st_string_left(_, 6)));
            CHECK(   st_string_equal(_,                            st_string_left(_, 22)));
            CHECK_EQ(_.len,                                               st_string_left(_, 22).len);
        }
        SUBCASE("right")
        {
            st_string_t _ = ST_STRING("abcdefABCDEF1234567890");
            CHECK(   st_string_equal(ST_STRING_EMPTY,               st_string_right(_, 0)));
            CHECK(   st_string_equal(_,                          st_string_right(_, 9999)));
            CHECK(   st_string_equal(ST_STRING("7890"),             st_string_right(_, 4)));
            CHECK(   st_string_equal(ST_STRING("ABCDEF1234567890"), st_string_right(_, 16)));
            CHECK(   st_string_equal(ST_STRING("0"),                st_string_right(_, 1)));
            CHECK_EQ(_.len,                                             st_string_right(_, 22).len);
        }
        SUBCASE("trim")
        {
            st_string_t _ = ST_STRING("\n\r\t  lots of \n\r\t spaces \n\r\t");
            CHECK(st_string_equal(ST_STRING_EMPTY,                    st_string_trim(ST_STRING_EMPTY)));
            CHECK(st_string_equal(ST_STRING_EMPTY,                    st_string_trim(ST_STRING("   "))));
            CHECK(st_string_equal(ST_STRING("lots of \n\r\t spaces"), st_string_trim(_)));
            CHECK(st_string_equal(ST_STRING("null \0 terminators"),   st_string_trim(ST_STRING("\0 null \0 terminators \0"))));
        }
        SUBCASE("trim left")
        {
            st_string_t _ = ST_STRING("\n\r\t  lots of \n\r\t spaces \n\r\t");
            CHECK(st_string_equal(ST_STRING_EMPTY,                           st_string_trim_left(ST_STRING_EMPTY)));
            CHECK(st_string_equal(ST_STRING_EMPTY,                           st_string_trim_left(ST_STRING("   "))));
            CHECK(st_string_equal(ST_STRING("lots of \n\r\t spaces \n\r\t"), st_string_trim_left(_)));
            CHECK(st_string_equal(ST_STRING("null \0 terminators \0"),       st_string_trim_left(ST_STRING("\0 null \0 terminators \0"))));
        }
        SUBCASE("trim right")
        {
            st_string_t _ = ST_STRING("\n\r\t  lots of \n\r\t spaces \n\r\t");
            CHECK(st_string_equal(ST_STRING_EMPTY,                            st_string_trim_right(ST_STRING_EMPTY)));
            CHECK(st_string_equal(ST_STRING_EMPTY,                            st_string_trim_right(ST_STRING("   "))));
            CHECK(st_string_equal(ST_STRING("\n\r\t  lots of \n\r\t spaces"), st_string_trim_right(_)));
            CHECK(st_string_equal(ST_STRING("\0 null \0 terminators"),        st_string_trim_right(ST_STRING("\0 null \0 terminators \0"))));
        }
    }
}
