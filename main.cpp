#include "MDLToolkit/ciphertext.hpp"
#include "MDLToolkit/fake_ctxt.hpp"
#include "MDLToolkit/tfhe_ctxt.hpp"
#include "MDLToolkit/Integer.hpp"

#include <tfhe_core.h>
#include <tfhe.h>
#include <iostream>
#include <ctime>

int main() {
    const int minimum_lambda = 110;
    auto params = new_default_gate_bootstrapping_parameters(minimum_lambda);
    uint32_t seed[] = { 314, 1592, 657 };
    tfhe_random_generator_setSeed(seed, 3);
    auto key = new_random_gate_bootstrapping_secret_keyset(params);

    mdl::TFHEEncryptor encryptor(key);
    mdl::TFHEDecryptor decryptor(key->lwe_key);

    Integer<TFHECtxt> a(32), b(32);
    a.encrypt(8, &encryptor);
    b.encrypt(7, &encryptor);


    auto _st = std::clock();
    a.add(a);
    std::cout << "add " << (std::clock() - _st) / (double)CLOCKS_PER_SEC << "\n";

    _st = std::clock();
    a.sub(b);
    std::cout << "sub " << (std::clock() - _st) / (double)CLOCKS_PER_SEC << "\n";

    _st = std::clock();
    a.mul(b);
    std::cout << "mul " << (std::clock() - _st) / (double)CLOCKS_PER_SEC << "\n";

    std::cout << a.decrypt(&decryptor) << "\n";
    std::cout << b.decrypt(&decryptor) << "\n";

    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);
    return 0;
}
