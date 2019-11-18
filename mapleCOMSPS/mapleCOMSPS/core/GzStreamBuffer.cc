#include "../core/GzStreamBuffer.h"

namespace MapleCOMSPS {

GzStreamBuffer::GzStreamBuffer(const std::string& filename, const std::string& mode)
    : GzStreamBuffer(filename.c_str(), mode.c_str()) {
}

GzStreamBuffer::GzStreamBuffer(const char* filename, const char* mode) :
        _filename(filename),
        _in(nullptr),
        _index(0),
        _size(0) {
    _in = gzopen(filename, mode);
    if (_in == nullptr) {
        std::cerr << "Cannot open file '" << filename << "'";
        abort();
    }
}

GzStreamBuffer::~GzStreamBuffer() {
    if (_in != nullptr) {
        gzflush(_in, Z_FINISH);
        gzclose(_in);
    }
}


int GzStreamBuffer::readInt() {
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
        std::cerr << "Cannot read literal " << value << c;

    return negative ? -value : value;
}

void GzStreamBuffer::skipWhiteSpaces() {
    unsigned char c;

    while ((c = read()) != '\0' && std::isspace(c)) {
        _index++;
    }
}

void GzStreamBuffer::skipLine() {
    unsigned char c;

    while ((c = read()) != '\0' && c != '\n') {
        _index++;
    }
    ++(*this);
}


int GzStreamBuffer::operator*() {
    return static_cast<int>(read());
}

void GzStreamBuffer::operator++() {
    _index++;
    read();
}

unsigned char GzStreamBuffer::read() {
    if (_index >= _size) {
        _size = gzread(_in, _buffer, sizeof(_buffer));
        _index = 0;
    }
    return (_index >= _size) ? '\0' : _buffer[_index];
}

void GzStreamBuffer::write(bool force) {
    if (_index >= kBufferSize || force) {
        gzwrite(_in, _buffer, _index);
        _index = 0;
    }
}

void GzStreamBuffer::write(const std::string& s) {
    for (const char& c : s) {
        _buffer[_index++] = c;
        write(false);
    }
}

void GzStreamBuffer::write(const char *s) {
    std::string ss(s);
    write(ss);
}

void GzStreamBuffer::writeInt(int n) {
    std::string s = std::to_string(n);
    write(s);
}

void GzStreamBuffer::flush() {
    write(true);
}

}  // namespace MapleCOMSPS