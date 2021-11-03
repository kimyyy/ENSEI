// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include "seal/seal.h"
#include "seal/util/ntt.h"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;
using namespace seal;
using namespace seal::util;

inline vector<Modulus> select_modchain(uint64_t poly_degree){
    vector<Modulus> mod_chain;
    switch(poly_degree){
        case 1024:
            // from BFVDefault
            mod_chain = {Modulus(0x7e00001)};
            break;
        case 2048:
            // from BFVDefault
            mod_chain = {Modulus(0x3fffffff000001)};
            break;
        default:
            // 4096 or larger
            mod_chain = {Modulus(0xffffff00000001)};
            break;
    }
    return mod_chain;
}

void print_vector(vector<uint64_t>& v, uint64_t num){
    for(uint64_t i = 0;i < num;i++){
        cout << v[i] << " ";
    }
    cout << endl;
}


inline auto ntt_seal(vector<uint64_t> &input, NTTTables &tables, bool is_inverse, MemoryPoolHandle &pool){
    auto poly_size = input.size();
    auto poly(allocate_poly(poly_size, 1, pool));
    set_poly(input.data(), input.size(), 1, poly.get());
    if(is_inverse){
        inverse_ntt_negacyclic_harvey(poly.get(), tables);
    }else{
        ntt_negacyclic_harvey(poly.get(), tables);
    }
    set_poly(poly.get(), input.size(), 1, input.data());
    return input;
}

vector<uint64_t> ensei_mult(vector<uint64_t>& input, vector<uint64_t>& kernel,Modulus &plain_modulus, uint64_t poly_modulus_degree)
{
    // client
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(select_modchain(poly_modulus_degree));
    parms.set_plain_modulus(plain_modulus);

    SEALContext context(parms);

    cout << "Set encryption parameters and print" << endl;
    cout << "Parameter validation (success): " << context.parameter_error_message() << endl;

    auto qualifiers = context.first_context_data()->qualifiers();
    cout << "Batching enabled: " << boolalpha << qualifiers.using_batching << endl;

    cout << endl;

    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    BatchEncoder batch_encoder(context);

    // generate input and kernel and batch into plaintexts
    size_t slot_count = batch_encoder.slot_count();
    cout << "slot_count: " << slot_count << endl;
    input.resize(slot_count);
    kernel.resize(slot_count);
    Plaintext input_plain;
    Plaintext kernel_plain;
    batch_encoder.encode(input, input_plain);
    batch_encoder.encode(kernel, kernel_plain);

    Ciphertext input_enc;
    encryptor.encrypt(input_plain, input_enc);

    cout << "    + size of freshly encrypted x: " << input_enc.size() << endl;

    cout << "    + noise budget in freshly encrypted x: " << decryptor.invariant_noise_budget(input_enc) << " bits"
         << endl;

    // server 
    auto mult_pre = chrono::high_resolution_clock::now();
    evaluator.multiply_plain_inplace(input_enc, kernel_plain);
    auto mult_post = chrono::high_resolution_clock::now();
    auto mult_time = chrono::duration_cast<chrono::microseconds>(mult_post - mult_pre);
    cout << "mult: " << mult_time.count() << "us" << endl;
    
    // client
    Plaintext input_dec;
    decryptor.decrypt(input_enc, input_dec);
    vector<uint64_t> input_result;
    batch_encoder.decode(input_dec, input_result);
    return input_result;
}

void ensei_bench(uint64_t poly_modulus_degree){
    MemoryPoolHandle pool = MemoryPoolHandle::Global();
    Modulus plain_modulus = PlainModulus::Batching(poly_modulus_degree, 20);
    cout << "plain_modulus: " << plain_modulus.value() << endl;
    uint64_t ntt_degree = 4;
    uint64_t coeff_count_power = 2;
    NTTTables tables(coeff_count_power, plain_modulus, pool);
    vector<uint64_t> input = {1, 2, 3, 4};
    vector<uint64_t> kernel= {1, 2, 0, 0};
    ntt_seal(input, tables, false, pool);
    ntt_seal(kernel, tables, false, pool);
    vector<uint64_t> validate_mult(ntt_degree);
    for(uint64_t i = 0;i < ntt_degree;i++){
        validate_mult[i] = input[i] * kernel[i];
    }
    auto conved = ensei_mult(input, kernel ,plain_modulus, poly_modulus_degree);
    //print_vector(conved, 10);
    ntt_seal(conved, tables, true, pool);
    print_vector(conved, 4);
    cout << endl;
}


void test_seal_ntt(){
    MemoryPoolHandle pool = MemoryPoolHandle::Global();
    uint64_t degree_ntt = 4;
    uint64_t coeff_count_power = 2;
    Modulus modulus(40961);
    NTTTables tables(coeff_count_power, modulus, pool);
    vector<uint64_t> input = {1, 2, 3, 4};
    ntt_seal(input, tables, false, pool);
    cout << "ntted" << endl;
    print_vector(input, 4);
    ntt_seal(input, tables, true, pool);
    cout << "intted" << endl;
    print_vector(input, 4);
}


int main(int argc, char **argv){
    if(argc !=2){
        cout << "Usage: ./ensei_bench poly_degree" << endl;
        exit(1);
    }
    uint64_t poly_degree = (uint64_t)atol(argv[1]);
    ensei_bench(poly_degree);
}
