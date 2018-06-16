#pragma once

#include "MDLToolkit/ciphertext.hpp"

#include <tfhe_core.h>
#include <tfhe.h>
#include <memory>
#include <cassert>

class TFHECtxt : public mdl::Ctxt<TFHECtxt> {
public:
    using BootKey = TFheGateBootstrappingCloudKeySet;

    explicit TFHECtxt(BootKey const* bk = nullptr) :  _bk(bk) {
    }

    TFHECtxt(TFHECtxt const& oth) {
        _bk = oth._bk;
        assert(_bk);
        _bit.reset(new LweSample(_bk->params->in_out_params));
        lweCopy(_bit.get(), oth._bit.get(), _bk->params->in_out_params);
    }

    // swap idiom
    TFHECtxt& operator=(TFHECtxt oth) {
        swap(oth);
        return *this;
    }

    void swap(TFHECtxt &oth) {
        std::swap(_bk, oth._bk);
        std::swap(_bit, oth._bit);
    }

    void set(LweSample *ptr) {
        _bit.reset(ptr);
    }

    TFHECtxt copy() const {
        return *this;
    }

    TFHECtxt& XOR(TFHECtxt const& oth) {
        LweSample *result = new LweSample(_bk->params->in_out_params);
        if (result) {
            bootsXOR(result, _bit.get(), oth._bit.get(), _bk);
            _bit.reset(result);
        }
        return *this;
    }

    TFHECtxt& OR(TFHECtxt const& oth) {
        LweSample *result = new LweSample(_bk->params->in_out_params);
        if (result) {
            bootsOR(result, _bit.get(), oth._bit.get(), _bk);
            _bit.reset(result);
        }
        return *this;
    }

    TFHECtxt& AND(TFHECtxt const& oth) {
        LweSample *result = new LweSample(_bk->params->in_out_params);
        if (result) {
            bootsAND(result, _bit.get(), oth._bit.get(), _bk);
            _bit.reset(result);
        }
        return *this;
    }

    TFHECtxt& XNOR(TFHECtxt const& oth) {
        LweSample *result = new LweSample(_bk->params->in_out_params);
        if (result) {
            bootsXNOR(result, _bit.get(), oth._bit.get(), _bk);
            _bit.reset(result);
        }
        return *this;
    }

    TFHECtxt& NOT() {
        lweNegate(_bit.get(), _bit.get(), _bk->params->in_out_params);
        return *this;
    }

    TFHECtxt NOT() const {
        auto cpy{copy()};
        cpy.NOT(); 
        return cpy;
    }

#define DECLAR_CONST_FUNCTION(OP) \
    TFHECtxt OP(TFHECtxt const& oth) const { \
        auto cpy{copy()}; \
        cpy.OP(oth); \
        return cpy; \
    }

    DECLAR_CONST_FUNCTION(XOR);
    DECLAR_CONST_FUNCTION(OR);
    DECLAR_CONST_FUNCTION(AND);
    DECLAR_CONST_FUNCTION(XNOR);

protected:
    std::unique_ptr<LweSample> _bit;    

private:
    const BootKey *_bk;
};

#undef DECLAR_CONST_FUNCTION


