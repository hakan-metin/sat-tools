// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_STREAMBUFFER_H_
#define SATTOOLS_STREAMBUFFER_H_

#include <zlib.h>

#include <cctype>
#include <string>

#include "sattools/Logging.h"

static const int kBufferSize = 4096;

namespace sat {

class StreamBuffer {
 public:
    explicit StreamBuffer(const std::string& filename);
    explicit StreamBuffer(const char * filename);
    ~StreamBuffer();

    int readInt();
    void skipWhiteSpaces();
    void skipLine();

    int operator*();
    void operator++();

 private:
    const std::string _filename;
    gzFile _in;
    unsigned char _buffer[kBufferSize];
    unsigned int _index;
    unsigned int _size;

    unsigned char read();
};

}  // namespace sat

#endif  // SATTOOLS_STREAMBUFFER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
