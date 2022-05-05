#pragma once
#include <stdexcept>

namespace Exceptions {

    class Exception: public std::runtime_error {
    public:
        explicit Exception(const std::string& msg): std::runtime_error(msg) {}
    };

    class UnableOpenFile: public Exception {
    public:
        explicit UnableOpenFile(const std::string& file_name): Exception("Unable to open file: " + file_name) {}
    };

    class InvalidArguments: public Exception {
    public:
        explicit InvalidArguments(const std::string& invalid_argument): Exception("Invalid arguments: " + invalid_argument) {}
    };

    class ReadingError: public Exception {
    public:
        ReadingError(): Exception("Error while reading the file") {}
    };

    class WritingError: public Exception {
    public:
        WritingError(): Exception("Error while writing the file") {}
    };

    class InvalidFileFormat: public Exception {
    public:
        explicit InvalidFileFormat(const std::string& s): Exception("Invalid file format: " + s) {}
    };
}