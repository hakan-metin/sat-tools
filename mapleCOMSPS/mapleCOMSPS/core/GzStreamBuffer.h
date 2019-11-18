#pragma once
#include <zlib.h>
#include <iostream>

#include <cctype>
#include <string>

static const int kBufferSize = 4096;

namespace MapleCOMSPS {

static const char kRead[] = "rb";
static const char kCompressedWrite[] = "w";
static const char kUncompressedWrite[] = "wT";

class GzStreamBuffer {
 public:
    explicit GzStreamBuffer(const std::string& filename, const std::string& mode);
    explicit GzStreamBuffer(const char *filename, const char* mode);
    ~GzStreamBuffer();

    // Write
    void write(const std::string& s);
    void write(const char *s);
    void writeInt(int n);
    void flush();

    // Read
    int readInt();
    void skipWhiteSpaces();
    void skipLine();

    int operator*();
    void operator++();

 private:
    const std::string _filename;
    const std::string mode;
    gzFile _in;
    unsigned char _buffer[kBufferSize];
    unsigned int _index;
    unsigned int _size;

    unsigned char read();
    void write(bool force);
};

} // namespace MapleCOMSPS