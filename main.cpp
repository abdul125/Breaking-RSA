#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <gmpxx.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Please specify the file name.\n";
        return 0;
    }

    // Load the ciphertext from the file.
    std::vector<mpz_class> ciphertext;
    {
        std::fstream fs;
        fs.open(argv[1], std::fstream::in);
        std::string line;
        while (std::getline(fs, line)) {
            ciphertext.push_back(mpz_class(line.c_str(), 16));
        }
    }

    // Public key.
    mpz_class n, a;
    n.set_str("a65db2074039053f9980a4d06884cc7c8cd72e3bd2e883e8f75c1f17", 16);
    a.set_str("35ef8e080e741222ddd5256b9f2e2144c32e2fb1d909f5ae160805", 16);

    // Factorize n using msieve tool.
    // ./msieve -q 0xa65db2074039053f9980a4d06884cc7c8cd72e3bd2e883e8f75c1f17
    /*
    Msieve v. 1.52 (SVN Unversioned directory)
    Fri Mar 21 03:13:48 2014
    random seeds: 447f26f3 b0cc27a0
    factoring 17520384478424538193194355741160194705641053444117392856320046800663 (68 digits)
    no P-1/P+1/ECM available, skipping
    commencing quadratic sieve (68-digit input)
    using multiplier of 31
    using generic 32kb sieve core
    sieve interval: 12 blocks of size 32768
    processing polynomials in batches of 17
    using a sieve bound of 177101 (8029 primes)
    using large prime bound of 13990979 (23 bits)
    using trial factoring cutoff of 24 bits
    polynomial 'A' values have 9 factors
    restarting with 4159 full and 44314 partial relations

    8682 relations (4159 full + 4523 combined from 44314 partial), need 8125
    sieving complete, commencing postprocessing
    begin with 48474 relations
    reduce to 12584 relations in 2 passes
    attempting to read 12584 relations
    recovered 12584 relations
    recovered 9745 polynomials
    attempting to build 8682 cycles
    found 8682 cycles in 1 passes
    distribution of cycle lengths:
       length 1 : 4159
       length 2 : 4523
    largest cycle: 2 relations
    matrix is 8029 x 8682 (1.2 MB) with weight 252612 (29.10/col)
    sparse part has weight 252612 (29.10/col)
    filtering completed in 4 passes
    matrix is 7217 x 7281 (1.0 MB) with weight 203634 (27.97/col)
    sparse part has weight 203634 (27.97/col)
    commencing Lanczos iteration
    memory use: 1.0 MB
    lanczos halted after 115 iterations (dim = 7211)
    recovered 61 nontrivial dependencies
    prp34 factor: 3889923060761232077444551752098261
    prp34 factor: 4504043963017591265757550206945083
    elapsed time 00:00:00
    */
    mpz_class f1, f2;
    f1.set_str("3889923060761232077444551752098261", 10);
    f2.set_str("4504043963017591265757550206945083", 10);

    // Compute phi(n).
    mpz_class phi_n = (f1 - 1) * (f2 - 1);

    // Compute b = a^k mod phi(n).
    mpz_class b;
    mpz_powm(b.get_mpz_t(), a.get_mpz_t(), mpz_class(-1).get_mpz_t(), phi_n.get_mpz_t());

    // Decrypt.
    for (auto& x : ciphertext) {
        // Compute d = x^b mod n.
        mpz_class d;
        mpz_powm(d.get_mpz_t(), x.get_mpz_t(), b.get_mpz_t(), n.get_mpz_t());

        // Extract ASCII characters.
        std::vector<char> str;
        while (d > 0) {
            mpz_class temp = d & 0xff;
            str.push_back(temp.get_ui());
            d >>= 8;
        }

        // Print in reverse.
        for (int i = str.size() - 1; i >= 0; i--) {
            std::cout << str[i];
        }
    }

    return 0;
}
