// Copyright 2017 Hakan Metin

#include "sattools/io/StreamBuffer.h"

namespace sattools {

StreamBuffer::StreamBuffer(const std::string& filename, const std::string& mode)
    : StreamBuffer(filename.c_str(), mode.c_str()) {
}

StreamBuffer::StreamBuffer(const char* filename, const char* mode) :
        _filename(filename),
        _in(nullptr),
        _index(0),
        _size(0) {
    _in = gzopen(filename, mode);
    if (_in == nullptr) {
        LOG(FATAL) << "Cannot open file '" << filename << "'";
        abort();
    }
}

StreamBuffer::~StreamBuffer() {
    if (_in != nullptr)
        gzclose(_in);
}


int StreamBuffer::readInt() {
    bool negative = false;
    int value = 0;

    skipWhiteSpaces();

    unsigned char c = read();
    if (c == '-') {
        negative = true;
        ++(*this);
    } else if (c == '+') {
        negative = false;
        ++(*this);
    }

    while ((c = read()) != '\0' &&  c >= '0' && c <= '9') {
        value = (value * 10) + (_buffer[_index] - '0');
        ++(*this);
    }

    if (!std::isspace(c) && c != '\0' && c != ',' && c != ')')
        LOG(FATAL) << "Cannot read literal " << value << c;

    return negative ? -value : value;
}

void StreamBuffer::skipWhiteSpaces() {
    unsigned char c;

    while ((c = read()) != '\0' && std::isspace(c)) {
        _index++;
    }
}

void StreamBuffer::skipLine() {
    unsigned char c;

    while ((c = read()) != '\0' && c != '\n') {
        _index++;
    }
    ++(*this);
}


int StreamBuffer::operator*() {
    return static_cast<int>(read());
}

void StreamBuffer::operator++() {
    _index++;
    read();
}

unsigned char StreamBuffer::read() {
    if (_index >= _size) {
        _size = gzread(_in, _buffer, sizeof(_buffer));
        _index = 0;
    }
    return (_index >= _size) ? '\0' : _buffer[_index];
}

void StreamBuffer::write(bool force) {
    if (_index >= kBufferSize || force) {
        gzwrite(_in, _buffer, _index);
        _index = 0;
    }
}

void StreamBuffer::write(const std::string& s) {
    for (const char& c : s) {
        _buffer[_index++] = c;
        write(false);
    }
}

void StreamBuffer::write(const char *s) {
    std::string ss(s);
    write(ss);
}

void StreamBuffer::writeInt(int n) {
    std::string s = std::to_string(n);
    write(s);
}

void StreamBuffer::flush() {
    write(true);
}

}  // namespace sattools

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
