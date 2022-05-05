#include "doctest.h"

#include <iostream>
#include <sstream>
#include <filesystem>
#include "archiver.h"
#include "tree.h"
#include "code.h"

namespace Huffman::Test {
    TEST_SUITE("archiver functions") {
        TEST_CASE("buildFrequencyTable") {
            std::stringstream stream;
            std::string s = "a\\bs\n#$$$\n";
            stream << s;
            std::map<uint8_t, uint32_t> table;
            uint32_t length = buildFrequencyTable(stream, table);

            CHECK_EQ(length, s.length());
            CHECK_EQ(table.size(), 7);
            CHECK_EQ(table['\n'], 2);
            CHECK_EQ(table['\\'], 1);
            CHECK_EQ(table['$'], 3);
        }


        TEST_CASE("writeInfo small") {
            //void writeInfo(DataProcessing::Writer &writer, int32_t extracted, std::vector<Code> &codes);
            std::stringstream stream;
            std::string s = "55";
            stream << s;
            std::map<uint8_t, uint32_t> table;
            uint32_t length = buildFrequencyTable(stream, table);
            Tree tree(table);
            std::vector<Code> codes = tree.getCodes();

            std::stringstream output;
            DataProcessing::Writer writer(output);
            writeInfo(writer, length, codes);
            writer.close();

            std::vector<uint32_t> result = {0, 1, 0, 53, 1, 1};
            DataProcessing::Reader reader(output);

            CHECK_EQ(2, reader.readInt());
            for (int i = 0; i < result.size(); i += 3) {
                CHECK_EQ(result[i], reader.readBits(CHAR_BIT));
                CHECK_EQ(result[i + 1], reader.readInt());
                CHECK_EQ(result[i + 2], reader.readBits(result[i + 1]));
            }
            CHECK_EQ(2, reader.readInt());
        }
    }

    TEST_SUITE("archive - unarchive") {

        void compareFile(const std::string& input, const std::string& output) {
            std::ifstream a(input, std::ios::binary);
            std::ifstream b(input, std::ios::binary);

            DataProcessing::Reader f(a), s(b);

            while (f.canRead() && s.canRead()) {
                CHECK_EQ(f.readBit(), s.readBit());
            }
            CHECK_EQ(f.canRead(), s.canRead());
        }

        void testArchiver(const std::string& input, const std::string& output) {
            const std::string archive = "../sample/test.bin";
            {
                Archiver archiver(input, archive);
                archiver.archive();
            }
            {
                Archiver unarchiver(archive, output);
                unarchiver.unarchive();
            }
            compareFile(input, output);
            std::filesystem::remove(output);
        }

        TEST_CASE("simple txt") {
            testArchiver("../sample/test.in", "../sample/test.out");
        }

        TEST_CASE("UTF_8 txt") {
            testArchiver("../sample/utf8.txt", "../sample/utf8.out");
        }

        TEST_CASE("png") {
            testArchiver("../sample/img.png", "../sample/test.png");
            testArchiver("../sample/img_1.png", "../sample/test.png");
        }

        TEST_CASE("jpg") {
            testArchiver("../sample/jpg.jpg", "../sample/test.jpg");
        }

        TEST_CASE("big data") {
            testArchiver("../sample/book.txt", "../sample/out.txt");
        }

        TEST_CASE("empty") {
            testArchiver("../sample/empty.in", "../sample/empty.out");
        }

        TEST_CASE("pdf") {
            testArchiver("../sample/Problem2.pdf", "../sample/out.pdf");
        }
    }
}