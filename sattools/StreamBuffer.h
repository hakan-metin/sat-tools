// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_STREAMBUFFER_H_
#define SATTOOLS_STREAMBUFFER_H_

#include <zlib.h>

#include <cctype>
#include <string>

#include "sattools/Logging.h"

static const int kBufferSize = 4096;

namespace sat {

static const std::string kRead = "rb";
static const std::string kCompressedWrite = "w";
static const std::string kUncompressedWrite = "wT";

class StreamBuffer {
 public:
    explicit StreamBuffer(const std::string& filename, const std::string& mode);
    explicit StreamBuffer(const char *filename, const char* mode);
    ~StreamBuffer();

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

}  // namespace sat

#endif  // SATTOOLS_STREAMBUFFER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
