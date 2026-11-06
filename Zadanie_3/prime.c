#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <sysinfoapi.h>

#define FALSE 0
#define TRUE 1

typedef unsigned long long int ull;

#if defined(_WIN32) || defined(_WIN64)
    #include <io.h>
    #define jestTerminalem() _isatty(_fileno(stdin))
    #define FAST_GETCHAR() _getchar_nolock()
    #define FAST_PUTCHAR(c) _putchar_nolock(c)
#else
    #include <unistd.h>
    #define jestTerminalem() isatty(STDIN_FILENO)
    #define FAST_GETCHAR() getchar_unlocked()
    #define FAST_PUTCHAR(c) putchar_unlocked(c)
#endif

void zakonczEnterem(const char *komunikat) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (komunikat != NULL) printf("%s", komunikat);
    getchar();
}

static inline bool isPrime(ull number) {
    if (number < 2) return false;
    if (number == 2 || number == 3) return true;
    if ((number % 2 == 0) || (number % 3 == 0)) return false;

    for (ull i = 5; i * i <= number; i += 6) {
        if ((number % i == 0) || (number % (i + 2) == 0))
            return false;
    }
    return true;
}

static inline ull getNextPrime(ull number) {
    if (number < 2) return 2;
    ull i = (number % 2 == 0) ? number + 1 : number + 2;
    while (!isPrime(i)) {
        i += 2;
    }
    return i;
}

static inline bool pobierzLiczbe(ull *val) {
    int c = FAST_GETCHAR();
    while (c >= 0 && c <= ' ') c = FAST_GETCHAR();
    if (c < 0) return false;

    ull res = 0;
    while (c >= '0' && c <= '9') {
        res = res * 10 + (c - '0');
        c = FAST_GETCHAR();
    }
    *val = res;
    return true;
}

static inline void wypiszLiczbe(ull val) {
    if (val == 0) {
        FAST_PUTCHAR('0');
        return;
    }
    char bufor[24];
    int i = 0;
    while (val > 0) {
        bufor[i++] = (char)('0' + (val % 10));
        val /= 10;
    }
    while (--i >= 0) {
        FAST_PUTCHAR(bufor[i]);
    }
}

void processNumber(ull number) {
    wypiszLiczbe(number);
    FAST_PUTCHAR(' ');
    FAST_PUTCHAR('=');
    FAST_PUTCHAR(' ');
    FAST_PUTCHAR('[');

    if (number > 1) {
        ull temp = number;
        ull prime = 2;
        bool is_first = true;

        while (temp > 1) {
            ull exp = 0;
            while (temp % prime == 0) {
                temp /= prime;
                exp++;
            }

            if (!is_first) {
                FAST_PUTCHAR(',');
                FAST_PUTCHAR(' ');
            }
            wypiszLiczbe(exp);
            is_first = false;

            if (temp > 1) {
                prime = getNextPrime(prime);
            }
        }
    }

    FAST_PUTCHAR(']');
    FAST_PUTCHAR('\n');
}

void runUnitTests(void) {
    assert(sizeof(unsigned long long int) == 8);

    assert(isPrime(0) == false);
    assert(isPrime(1) == false);
    assert(isPrime(2) == true);
    assert(isPrime(3) == true);
    assert(isPrime(4) == false);

    assert(isPrime(25) == false);
    assert(isPrime(35) == false);
    assert(isPrime(49) == false);
    assert(isPrime(121) == false);

    assert(isPrime(97) == true);
    assert(isPrime(2147483647ULL) == true);
    assert(isPrime(2147483649ULL) == false);

    assert(getNextPrime(0) == 2);
    assert(getNextPrime(1) == 2);
    assert(getNextPrime(2) == 3);
    assert(getNextPrime(3) == 5);

    assert(getNextPrime(7) == 11);
    assert(getNextPrime(23) == 29);
    assert(getNextPrime(89) == 97);
}

int main(void) {
    runUnitTests();

    if (jestTerminalem()) {
        printf("Podaj liczbe zestawów danych: ");

        ull data_counter;
        if (pobierzLiczbe(&data_counter) && data_counter > 0) {
            ull *numbers = malloc(data_counter * sizeof(ull));
            if (numbers == NULL) {
                printf("Błąd alokacji pamięci!\n");
                return 1;
            }

            for (ull i = 0; i < data_counter; i++) {
                printf("Podaj liczbę [%llu/%llu]: ", i + 1, data_counter);
                pobierzLiczbe(&numbers[i]);
            }

            ull start = GetTickCount64();
            for (ull data_index = 0; data_index < data_counter; data_index++) {
                processNumber(numbers[data_index]);
            }
            ull stop = GetTickCount64();
            free(numbers);
            printf("Elapsed time is: %llu [ms]\n", stop - start);
        }

        zakonczEnterem("Naciśnij Enter, aby zakończyć...");
    } else {
        ull start = GetTickCount64();
        ull data_counter;
        if (pobierzLiczbe(&data_counter)) {
            for (ull data_index = 0; data_index < data_counter; data_index++) {
                ull number;
                if (pobierzLiczbe(&number)) {
                    processNumber(number);
                }
            }
        }
        ull stop = GetTickCount64();
        printf("Elapsed time is: %llu [ms]\n", stop - start);
    }

    return 0;
}