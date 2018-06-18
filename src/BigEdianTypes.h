//
// Created by Benjamin DROUARD on 18/06/2018.
//

#ifndef PGSENCODER_BIGEDIANTYPES_H
#define PGSENCODER_BIGEDIANTYPES_H

#include <arpa/inet.h> // for ntohs() etc.
#include <cstdint>

class be_uint32_t {
public:
    be_uint32_t() : be_val_(0) {}
    // Transparently cast from uint32_t
    be_uint32_t(const uint32_t &val) : be_val_(htons(val)) {}
    // Transparently cast to uint32_t
    operator uint32_t() const {
        return ntohs(be_val_);
    }
private:
    uint32_t be_val_;
} __attribute__((packed));

class be_uint24_t {
public:
    be_uint24_t() : be_val_(0) {}
    // Transparently cast from uint32_t
    be_uint24_t(const uint32_t &val) : be_val_(htons(val)) {}
    // Transparently cast to uint32_t
    operator uint32_t() const {
        return ntohs(be_val_);
    }
private:
    unsigned int be_val_ : 24;
} __attribute__((packed));

class be_uint16_t {
public:
    be_uint16_t() : be_val_(0) {}
    // Transparently cast from uint16_t
    be_uint16_t(const uint16_t &val) : be_val_(htons(val)) {}
    // Transparently cast to uint16_t
    operator uint16_t() const {
        return ntohs(be_val_);
    }
private:
    uint16_t be_val_;
} __attribute__((packed));

#define be_uint8_t uint8_t

#endif //PGSENCODER_BIGEDIANTYPES_H
