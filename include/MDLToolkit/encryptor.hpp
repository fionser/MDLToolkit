#pragma once
#include "MDLToolkit/tfhe_ctxt.hpp"
#include <tfhe.h>

namespace mdl {

template <typename Cipher>
class Encryptor {
public:
    virtual Cipher encrypt(int bit) const = 0;
    virtual Cipher encrypt(bool bit) const = 0;
};


class FakeEncrytor : public Encryptor<FakeCtxt> {
public:
    FakeCtxt encrypt(bool bit) const {
        return FakeCtxt(bit);
    }

    FakeCtxt encrypt(int bit) const {
        return encrypt(bit > 0);
    }
};

class TFHEEncryptor : public Encryptor<TFHECtxt> {
public:
    using GBKeys = TFheGateBootstrappingSecretKeySet;

    TFHEEncryptor(GBKeys const* keys) : _keys(keys) {}

    TFHECtxt encrypt(bool bit) const {
        LweSample *ptr = new LweSample(_keys->params->in_out_params);
        const double alpha = _keys->params->in_out_params->alpha_min; 
        lweSymEncrypt(ptr, bit ? _1s8 : -_1s8, alpha, _keys->lwe_key);
        TFHECtxt ctx(&(_keys->cloud));
        ctx.set(ptr);
        return ctx;
    }

    TFHECtxt encrypt(int bit) const {
        return encrypt(bit > 0);
    }

private:
    const Torus32 _1s8 = modSwitchToTorus32(1, 8);
    const GBKeys *_keys;
};

} // namespace mdl
