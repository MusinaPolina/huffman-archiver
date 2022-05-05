#include "archiver.h"
#include "reader.h"
#include "tree.h"
#include "writer.h"
#include <iostream>
#include <climits>
#include <algorithm>
#include <set>

namespace Huffman {

    Archiver::Archiver(const std::string& input_name, const std::string& output_name) {
        input_ = std::ifstream(input_name, std::ios::binary);
        if (input_.fail()) throw Exceptions::UnableOpenFile(input_name);

        output_ = std::ofstream(output_name, std::ios::binary);
        if (output_.fail()) throw Exceptions::UnableOpenFile(output_name);
    }

    uint32_t buildFrequencyTable(std::istream& input_, std::map<uint8_t, uint32_t>& frequency_table) {
        DataProcessing::Reader reader(input_);

        while (reader.canRead()) {
            uint8_t byte = reader.readBits(CHAR_BIT);
            frequency_table[byte]++;
        }

        return reader.bytesExtracted();
    }

    void writeInfo(DataProcessing::Writer& writer, int32_t extracted, std::vector<Code>& codes) {

        writer.writeInt(codes.size());
        for (const auto& code: codes) {
            writer.writeBits(code.symbol());
            writer.writeInt(code.length());
            writer.writeBits(code.code_);
        }

        writer.writeInt(extracted);
        writer.close();
    }

    void compress(DataProcessing::Reader& reader, DataProcessing::Writer& writer, std::vector<Code>& codes) {

        std::set<Code> sorted_codes(codes.begin(), codes.end());
        while (reader.canRead()) {
            uint8_t symbol = reader.readBits(CHAR_BIT);
            auto code = sorted_codes.find(Code(symbol));

            if (code == sorted_codes.end()) {
                throw Exceptions::InvalidFileFormat(" unknown symbol during the second pass");
            }

            writer.writeBits((*code).code_);
        }
        writer.close();
    }

    void Archiver::archive() {
        std::map<uint8_t, uint32_t> frequency_table;
        size_t initial_size = buildFrequencyTable(input_, frequency_table);

        Tree tree(frequency_table);
        std::vector<Code> codes = tree.getCodes();

        DataProcessing::Writer writer(output_);
        DataProcessing::Reader reader(input_);

        writeInfo(writer, initial_size, codes);
        size_t info_size = writer.bytesInserted();

        compress(reader, writer, codes);
        size_t compressed_size = writer.bytesInserted() - info_size;

        std::cout << initial_size << std::endl << compressed_size << std::endl << info_size << std::endl;
    }

    std::vector<Code> readInfo(DataProcessing::Reader &reader) {
        size_t codes_size = reader.readInt();
        std::vector<Code> codes;

        while (codes_size--) {
            uint8_t symbol = reader.readBits(CHAR_BIT);

            Code code(symbol);
            size_t length = reader.readInt();

            code.code_.resize(length);
            for (size_t i = 0; i < length; i++) {
                code.code_[i] = reader.readBit();
            }

            codes.emplace_back(code);
        }
        return codes;
    }

    void Archiver::unarchive() {
        DataProcessing::Reader reader(input_);
        std::vector<Code> codes = readInfo(reader);
        Tree tree(codes);

        size_t length = reader.readInt();
        reader.close();

        size_t info_size = reader.bytesExtracted();

        DataProcessing::Writer writer(output_);
        while (length--) {
            uint8_t symbol = tree.getSymbol(reader);
            writer.writeBits(symbol, CHAR_BIT);
        }
        writer.close();

        size_t compressed_size = reader.bytesExtracted() - info_size;
        size_t initial_size = writer.bytesInserted();

        std::cout << compressed_size << std::endl << initial_size << std::endl << info_size << std::endl;
    }
}