#pragma once
#include "MDLToolkit/tfhe_ctxt.hpp"
#include <tfhe.h>

namespace mdl {
template <typename Cipher>
class Decryptor {
public:
    virtual int decrypt(Cipher const&) const = 0;
};

class TFHEDecryptor : public Decryptor<TFHECtxt> {
public:
    TFHEDecryptor(const LweKey *sk) : _sk(sk) {}

    int decrypt(TFHECtxt const& ctx) const override {
        assert(_sk);
        Torus32 mu = lwePhase(ctx._bit.get(), _sk);
        return mu > 0 ? 1 : 0; 
    }
private:
    const LweKey *_sk;
};
} // namespace mdl

