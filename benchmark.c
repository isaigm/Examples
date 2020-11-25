#include <gmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
int main()
{
    bool is_prime = true;
    mpz_t n, counter;
    mpz_init_set_ui(counter, 2);
    mpz_init_set_ui(n, 18014398241046527);
    mpz_t integer_sqrt, mod;
    mpz_init(mod);
    mpz_init(integer_sqrt);
    mpz_sqrt(integer_sqrt, n);
    clock_t start = clock();
    while (mpz_cmp(counter, integer_sqrt) < 0 && is_prime)
    {
        mpz_mod(mod, n, counter);
        if (mpz_cmp_ui(mod, 0) == 0)
        {
            is_prime = false;
        }
        mpz_add_ui(counter, counter, 1);
    }
    clock_t end = clock();
    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    gmp_printf("%Zd ", n);
    if (is_prime)
    {
        printf("is prime\n");
    }
    else
    {
        printf("is not a prime\n");
    }
    mpz_clear(mod);
    mpz_clear(counter);
    mpz_clear(integer_sqrt);
    mpz_clear(n);
    return 0;
}