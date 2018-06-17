#pragma once
#include <vector>
#include <cassert>
#include "MDLToolkit/encryptor.hpp"
#include "MDLToolkit/decryptor.hpp"
#include "MDLToolkit/ciphertext.hpp"

template <class Cipher>
struct Integer {

    Integer(int nbits) : _nbits(nbits) {
        assert(nbits > 0);
    }

    Integer(const Integer &oth) :
        _nbits(oth._nbits),
        _bits(oth._bits) {
    }

    Integer& operator=(Integer oth) {
        swap(oth);
        return *this;
    }

    void swap(Integer &oth) {
        std::swap(_nbits, oth._nbits);
        std::swap(_bits, oth._bits);
    }

    Integer& add(Integer<Cipher> const& oth) {
        add_full(_bits.data(), _bits.data(), 
                 oth._bits.data(), _nbits) ;
        return *this;
    }

    Integer add(Integer<Cipher> const& oth) const {
        Integer cpy{*this};
        cpy.add(oth);
        return cpy;
    }

    Integer& sub(Integer<Cipher> const& oth) {
        if (_nbits == 0)
            return *this;
        Cipher borrow = _bits.front().copy();
        borrow.zero();
        for (int i = 0; i < _nbits; ++i) {
            auto bxa = _bits[i];
            auto bxc = oth._bits[i];
            bxa.XOR(oth._bits[i]); 
            bxc.XOR(borrow);

            _bits[i] = bxa;
            _bits[i].XOR(borrow);
            bxa.AND(bxc);
            borrow.XOR(bxa);
        }
        return *this;
    }

    Integer& mul(Integer<Cipher> const& oth) {
        if (_nbits == 0)
            return *this;
        Integer sum = *this;
        Integer tmp = *this;
        for (int i = 0; i < _nbits; ++i)
            sum._bits[i].zero();

        for (int i = 0; i < _nbits; ++i) {
            for (int k = 0; k < _nbits - i; ++k)
                tmp._bits[k] = oth._bits[k].AND(_bits[i]);

            add_full(sum._bits.data() + i, 
                     sum._bits.data() + i,
                     tmp._bits.data(), _nbits - i);
        }
        std::swap(_bits, sum._bits);
        return *this;
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

    uint32_t decrypt(const mdl::Decryptor<Cipher> *decryptor) const {
        if (!decryptor || _bits.empty())
            return 0;
        uint32_t ans{0};
        for (auto itr = _bits.crbegin(); itr != _bits.crend(); ++itr)
            ans = (ans << 1) | decryptor->decrypt(*itr);
        return ans;
    }

protected:
    void add_full(
        Cipher *dest, 
        Cipher const* op1, 
        Cipher const* op2,
        int size) const 
    {
        if (size == 0)
            return ;
        Cipher carry = op1[0].copy();
        carry.zero();
        for (int i = 0; i < size; ++i) {
            auto axc = op1[i];
            auto bxc = op2[i];

            axc.XOR(carry);
            bxc.XOR(carry);
            dest[i] = op1[i].XOR(bxc);

            axc.AND(bxc);
            carry.XOR(axc);
        }
    }

protected:
    int _nbits;
    std::vector<Cipher> _bits;
};

