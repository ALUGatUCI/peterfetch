#include "artwork.hpp"

#include <sstream>

#include <gtest/gtest.h>
#include <stdexcept>

std::string str(const Artwork& art) {
    std::stringstream str;
    Artwork::iterator iter_art {art.begin()};
    for(int i {}; i < art.artwork_lines(); ++i, ++iter_art)
        str << *iter_art << '\n';
    return str.str();
}

TEST(Artwork, AllDefault)
{
    Artwork art { "foo\nbar\nbaz\n", 3};
    EXPECT_EQ(str(art), "foo\nbar\nbaz\n");
    art.color( "   \n   \n   \n", "   \n   \n   \n");
    EXPECT_EQ(str(art), "foo\x1b[0m\nbar\x1b[0m\nbaz\x1b[0m\n");
}

TEST(Artwork, Checkers)
{
    Artwork art { "xox\noxo\nxox\n", 3};
    art.color( "141\n414\n141\n", "C9C\n9C9\nC9C\n");
    std::string expected {
        "\x1b[31;104mx\x1b[34;101mo\x1b[31;104mx\x1b[0m\n"
        "\x1b[34;101mo\x1b[31;104mx\x1b[34;101mo\x1b[0m\n"
        "\x1b[31;104mx\x1b[34;101mo\x1b[31;104mx\x1b[0m\n"
    };
    std::string result {str(art)};
    EXPECT_EQ(result.size(), expected.size());
    EXPECT_EQ(result, expected);
}

TEST(Artwork, AllForegrounds) {
    Artwork art { "01234567\n89ABCDEF\n", 8};
    art.color("01234567\n89ABCDEF\n", "        \n        \n");
    std::string expected {
        "\x1b[30;49m0\x1b[31;49m1\x1b[32;49m2\x1b[33;49m3"
        "\x1b[34;49m4\x1b[35;49m5\x1b[36;49m6\x1b[37;49m7"
        "\x1b[0m\n\x1b[90;49m8\x1b[91;49m9\x1b[92;49mA"
        "\x1b[93;49mB\x1b[94;49mC\x1b[95;49mD\x1b[96;49mE"
        "\x1b[97;49mF\x1b[0m\n"
    };
    std::string result {str(art)};
    EXPECT_EQ(result.size(), expected.size());
    EXPECT_EQ(result, expected);
}

TEST(Artwork, AllBackgrounds) {
    Artwork art { "01234567\n89ABCDEF\n", 8};
    art.color("        \n        \n", "01234567\n89ABCDEF\n");
    std::string expected {
        "\x1b[39;40m0\x1b[39;41m1\x1b[39;42m2\x1b[39;43m3"
        "\x1b[39;44m4\x1b[39;45m5\x1b[39;46m6\x1b[39;47m7"
        "\x1b[0m\n\x1b[39;100m8\x1b[39;101m9\x1b[39;102mA"
        "\x1b[39;103mB\x1b[39;104mC\x1b[39;105mD"
        "\x1b[39;106mE\x1b[39;107mF\x1b[0m\n"
    };
    std::string result {str(art)};
    EXPECT_EQ(result.size(), expected.size());
    EXPECT_EQ(result, expected);
}

TEST(Artwork, ErrorHandling)
{
    Artwork art { "foo\nbar\nbaz\n", 3};
    EXPECT_EQ(str(art), "foo\nbar\nbaz\n");
    EXPECT_THROW({
            try {
                art.color( "  \n        \n   \n", "   \n   \n   \n");
            } catch ( const std::runtime_error& err ) {
                EXPECT_STREQ( "Delimiters are not of equal size", err.what() );
                throw;
            }
        },
        std::runtime_error
    );

    EXPECT_THROW({
            try {
                art.color( " \n   \n   \n", " \n   \n   \n");
            } catch ( const std::runtime_error& err ) {
                EXPECT_STREQ( "Delimiters are not equal in size to the artwork", err.what() );
                throw;
            }
        },
        std::runtime_error
    );

    EXPECT_THROW({
            try {
                art.color( "foo\nbar\nbaz\n", "foo\nbar\nbaz\n");
            } catch (const std::runtime_error& err ) {
                EXPECT_STREQ( "Invalid color delimiter", err.what() );
                throw;
            }
        },
        std::runtime_error
    );
}
