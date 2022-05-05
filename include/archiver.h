#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "exceptions.h"
#include "writer.h"
#include "code.h"
#include "reader.h"

namespace Huffman {
    class Archiver {
    public:
        Archiver(const std::string& input_name, const std::string& output_name);

        void archive();
        void unarchive();

    private:
        std::ifstream input_;
        std::ofstream output_;
    };

    uint32_t buildFrequencyTable(std::istream& input_, std::map<uint8_t, uint32_t>& frequency_table);
    void writeInfo(DataProcessing::Writer& writer, int32_t extracted, std::vector<Code>& codes);
    void compress(DataProcessing::Reader& reader, DataProcessing::Writer& writer, std::vector<Code>& codes);
    std::vector<Code> readInfo(DataProcessing::Reader &reader);
}