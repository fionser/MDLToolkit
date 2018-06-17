#pragma once
#ifndef MDL_TOOLKIT_CIPHERTEXT_HPP
#define MDL_TOOLKIT_CIPHERTEXT_HPP
namespace mdl {
#define DECLAR_CONST_FUNCTION(OP) \
    Ctxt<T> OP(Ctxt<T> const& oth) const { \
        auto cpy = derived().copy(); \
        cpy.OP(oth.derived()); \
        return cpy; \
    }

#define DECLAR_FUNCTION(OP) \
    Ctxt<T>& OP(Ctxt<T> const& oth) { \
        T &d = derived(); \
        d.OP(oth.derived()); \
        return d; \
    }

template <typename T>
class Ctxt {
public:
    explicit Ctxt() {}

    ~Ctxt() {}

    Ctxt<T> copy() const {
        return derived().copy();
    }

    Ctxt(const Ctxt<T>& oth) {
        derived() = oth.derived();
    }

    Ctxt<T>& operator=(Ctxt<T> oth) {
        derived().swap(oth.derived());
        return *this;
    }

    DECLAR_FUNCTION(XOR);
    DECLAR_FUNCTION(OR);
    DECLAR_FUNCTION(AND);
    DECLAR_FUNCTION(XNOR);

    DECLAR_CONST_FUNCTION(XOR);
    DECLAR_CONST_FUNCTION(OR);
    DECLAR_CONST_FUNCTION(AND);
    DECLAR_CONST_FUNCTION(XNOR);

    Ctxt<T> NOT() const {
        auto cpy = derived().copy();
        cpy.NOT();
        return cpy;
    }

    Ctxt<T>& NOT() {
        derived().NOT();
        return derived();
    }

    const T& derived() const {
        return *(static_cast<const T *>(this));
    }

    T& derived() {
        return *(static_cast<T *>(this));
    } 
};

}
#undef DECLAR_FUNCTION
#undef DECLAR_CONST_FUNCTION
#endif
