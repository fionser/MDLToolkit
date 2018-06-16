#pragma once

#include "MDLToolkit/ciphertext.hpp"

class FakeCtxt : public mdl::Ctxt<FakeCtxt> {
public:
    FakeCtxt(bool bit) : _bit(bit) {}

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
