#include "doctest.h"

#include <iostream>
#include "reader.h"
#include <climits>
#include <writer.h>
#include <sstream>
#include <vector>

namespace DataProcessing::Test {
    TEST_SUITE("data_reader") {
        TEST_CASE("simple_a") {
            std::stringstream in;
            in << "a";
            Reader reader(in);
            CHECK_EQ(reader.readBits(CHAR_BIT), 'a');
        }

        TEST_CASE("amount of extracted bytes") {
            std::stringstream in;
            in << "aaa";

            Reader reader(in);
            int count = 0;
            while (reader.canRead()) {
                CHECK_EQ(count++, reader.bytesExtracted());
                reader.readBits(CHAR_BIT);
            }

            CHECK_EQ(3, reader.bytesExtracted());
        }

        TEST_CASE("empty file") {
            std::stringstream in;
            Reader reader(in);

            CHECK_EQ(false, reader.canRead());
            CHECK_EQ(0, reader.bytesExtracted());
        }
    }
    TEST_SUITE("data_writer") {
        TEST_CASE("simple_aaa") {
            std::stringstream s;
            Writer writer(s);

            writer.writeBits('a');
            writer.writeBits('a');
            writer.writeBits('a');

            std::string res;
            s >> res;
            CHECK_EQ(res, "aaa");
        }

        TEST_CASE("amount of inserted bytes") {
            std::stringstream s;
            Writer writer(s);

            writer.writeBits(12, 6);
            writer.writeBits(2, 4);
            writer.writeBits(63, 3);

            writer.close();

            CHECK_EQ(writer.bytesInserted(), 2);
        }
    }

    TEST_SUITE("combine") {
        TEST_CASE("small values") {
            std::stringstream s;

            Writer writer(s);
            writer.writeBits(27, 6);
            writer.close();

            Reader reader(s);
            CHECK_EQ(27, reader.readBits(6));
        }

        TEST_CASE("many small values") {
            std::stringstream s;
            std::vector<std::pair<uint8_t, uint8_t>> characters =
                    {{0, 1}, {15, 5}, {1, 1}, {255, 8}, {30, 5}};

            Writer writer(s);
            for (auto p : characters) {
                writer.writeBits(p.first, p.second);
            }
            writer.close();

            Reader reader(s);
            for (auto p : characters) {
                CHECK_EQ(p.first, reader.readBits(p.second));
            }
        }

        TEST_CASE("big values") {
            std::stringstream s;

            Writer writer(s);
            writer.writeBits(446, 9);
            writer.close();

            Reader reader(s);
            std::vector<bool> result = { 1, 1, 0, 1, 1, 1, 1, 1, 0 };
            for (auto bit: result) {
                CHECK_EQ(bit, reader.readBit());
            }
        }

        TEST_CASE("combine int") {
            std::stringstream s;
            Writer writer(s);
            Reader reader(s);

            std::vector<uint32_t> ints = {4, 12, 43, 91, 4324232, 432, 123, 2, 0, 234, UINT32_MAX, 20};

            for (auto i : ints) {
                writer.writeInt(i);
            }
            writer.close();
            for (auto i : ints) {
                uint32_t j = reader.readInt();
                CHECK_EQ(i, j);
            }
        }

        TEST_CASE("combine int char") {
            std::stringstream s;
            Writer writer(s);
            Reader reader(s);

            writer.writeInt(10);
            writer.writeBits(0);
            writer.writeInt(1);

            CHECK_EQ(10, reader.readInt());
            CHECK_EQ(0, reader.readBits(CHAR_BIT));
            CHECK_EQ(1, reader.readInt());
        }
    }
}