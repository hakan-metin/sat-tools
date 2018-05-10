#include <gtest/gtest.h>

#include "sattools/StreamBuffer.h"

namespace sat {

TEST(StreamBuffer, file_not_exists) {
    ASSERT_DEATH(StreamBuffer stream("file_not_exists"),
                 ".*Cannot open file.*");
}

TEST(StreamBuffer, operator_star) {
    StreamBuffer stream("tests/resources/test.cnf");

    ASSERT_EQ(*stream, 'p');
}

TEST(StreamBuffer, operator_plusplus) {
    StreamBuffer stream("tests/resources/test.cnf");

    ASSERT_EQ(*stream, 'p'); ++stream;
    ASSERT_EQ(*stream, ' '); ++stream;
    ASSERT_EQ(*stream, 'c'); ++stream;
    ASSERT_EQ(*stream, 'n'); ++stream;
    ASSERT_EQ(*stream, 'f'); ++stream;
    ASSERT_EQ(*stream, ' '); ++stream;
    ASSERT_EQ(*stream, '4'); ++stream;
    ASSERT_EQ(*stream, ' '); ++stream;
    ASSERT_EQ(*stream, '4'); ++stream;
    ASSERT_EQ(*stream, '\n'); ++stream;
}

TEST(StreamBuffer, skip_whitespaces) {
    StreamBuffer stream("tests/resources/test.cnf");

    stream.skipWhiteSpaces();
    ASSERT_EQ(*stream, 'p'); ++stream;
    stream.skipWhiteSpaces();
    ASSERT_EQ(*stream, 'c');
}

TEST(StreamBuffer, skip_line) {
    StreamBuffer stream("tests/resources/test.cnf");

    ASSERT_EQ(*stream, 'p');
    stream.skipLine();
    ASSERT_EQ(*stream, '1');

    stream.skipLine();
    stream.skipLine();
    stream.skipLine();
    stream.skipLine();

    ASSERT_EQ(*stream, '\0');
}

TEST(StreamBuffer, read_int) {
    int value;
    StreamBuffer stream("tests/resources/test.cnf");

    stream.skipLine();

    value = stream.readInt();
    ASSERT_EQ(value, 1);
    ASSERT_EQ(*stream, ' '); ++stream;

    value = stream.readInt();
    ASSERT_EQ(value, -2);
    ASSERT_EQ(*stream, ' '); ++stream;

    value = streamone.cnf();
    ASSERT_EQ(value, 3);
    ASSERT_EQ(*stream, ' '); ++stream;
}

}  // namespace core
