#include <iostream>
#include <cassert>
#include <cstring>
#include "archiver.h"

class Query {
public:
    enum class Type {
        compress, decompress
    };

    Type operation;
    std::string input, output;

    static const size_t NUMBER_OF_ARGUMENTS = 6;

    Query(int argc, char** argv) {
        if (argc != NUMBER_OF_ARGUMENTS) {
            throw Exceptions::InvalidArguments("incorrect number of arguments");
        }

        operation = getOperationType(argc, argv);
        input = getInput(argc, argv);
        output = getOutput(argc, argv);
    }

private:

    static Type getOperationType(int argc, char** argv) {
        for (size_t i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-u") == 0) {
                return Type::decompress;
            }
            if (strcmp(argv[i], "-c") == 0) {
                return Type::compress;
            }
        }
        throw Exceptions::InvalidArguments("unknown operation");
    }

    static std::string getInput(int argc, char** argv) {
        for (size_t i = 1; i < argc - 1; i++) {
            if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
                return argv[i + 1];
            }
        }
        throw Exceptions::InvalidArguments("no input file");
    }

    static std::string getOutput(int argc, char** argv) {
        for (size_t i = 1; i < argc - 1; i++) {
            if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
                return argv[i + 1];
            }
        }
        throw Exceptions::InvalidArguments("no output file");
    }
};


int main(int argc, char** argv) {
    try {
        Query query(argc, argv);
        Huffman::Archiver archiver(query.input, query.output);
        if (query.operation == Query::Type::compress) {
            archiver.archive();
        } else {
            archiver.unarchive();
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}