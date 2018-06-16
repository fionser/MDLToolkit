#pragma once
#include <vector>
#include <cassert>
#include "MDLToolkit/encryptor.hpp"

template <class Cipher>
struct Integer {
    Integer(int nbits) : _nbits(nbits) {
        assert(nbits > 0);
    }

    void encrypt(
        uint32_t m, 
        const mdl::Encryptor<Cipher> *encryptor) 
    {
        _bits.clear();
        _bits.reserve(_nbits);
        for (int i = 0; i < _nbits; ++i) {
            int b = m & 1;
            m >>= 1;
            _bits.emplace_back(encryptor->encrypt(b));
        }
    }

    void compare(Integer<Cipher> const &oth) const {
        for (int i = 0; i < _nbits; ++i)
            _bits[i].OR(oth._bits[i]);
    }

    const int _nbits;

protected:
    Cipher compare_bit(
        Cipher const& bit1, 
        Cipher const& bit2,
        Cipher const& lsb_carry,
        Cipher &tmp) const 
    {
        tmp = bit1.copy();
        tmp.XNOR(bit2);
    }
    std::vector<Cipher> _bits;
};

