#pragma once

#include "MDLToolkit/ciphertext.hpp"
#include <memory>

class FakeCtxt : public mdl::Ctxt<FakeCtxt> {
public:
    FakeCtxt(bool bit) : _bit(bit) {}

    void swap(FakeCtxt &oth) {
        std::swap(_bit, oth._bit);
    }

    void OR(FakeCtxt const& oth) {
        _bit |= oth._bit;
    }

    void XOR(FakeCtxt const& oth) {
        _bit ^= oth._bit;
    }

    void XNOR(FakeCtxt const &oth) {
        XOR(oth);
        NOT();
    }

    void NOT() {
        _bit = ~_bit;
    }

private:
    bool _bit = false;
};
