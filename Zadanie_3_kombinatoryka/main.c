#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <io.h>
    #define jestTerminalem() _isatty(_fileno(stdin))
#else
    #include <unistd.h>
    #define jestTerminalem() isatty(STDIN_FILENO)
#endif

typedef struct {
    char elementy[100][256];
    int rozmiar;
} Zbior;

void dodajDoZbioru(Zbior *zbior, const char *element) {
    if (zbior->rozmiar < 100) {
        strcpy(zbior->elementy[zbior->rozmiar++], element);
    }
}

typedef struct {
    const char *elementy[100];
    unsigned long long licznik;
} Wynik;

bool czyZawiera(const Wynik *wynik, int dlugosc, const char *element) {
    for (int i = 0; i < dlugosc; i++) {
        if (wynik->elementy[i] == element) return true;
    }
    return false;
}

void wypiszWynik(Wynik *wynik, int k) {
    wynik->licznik++;
    printf("[%llu] ", wynik->licznik);
    for (int i = 0; i < k; i++) {
        printf("%s%s", wynik->elementy[i], (i + 1 < k) ? ", " : "");
    }
    printf("\n");
}


void zakonczEnterem(const char *komunikat) {
    if (komunikat != NULL) printf("\n%s", komunikat);
    getchar();
}

bool czyRowne(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

bool czyZaczynaSieOd(const char *tekst, const char *prefiks) {
    return strncmp(tekst, prefiks, strlen(prefiks)) == 0;
}

bool czyZawieraZnak(const char *tekst, char znak) {
    return strchr(tekst, znak) != NULL;
}

unsigned long long silnia(int n) {
    unsigned long long w = 1;
    for (int i = 2; i <= n; i++) w *= i;
    return w;
}

unsigned long long potega(int a, int b) {
    unsigned long long w = 1;
    for (int i = 0; i < b; i++) w *= a;
    return w;
}


void generujWariacjeBezPowt(const Zbior *zbior, Wynik *wynik, int k, int poz) {
    if (poz == k) {
        wypiszWynik(wynik, k);
        return;
    }
    for (int i = 0; i < zbior->rozmiar; i++) {
        if (!czyZawiera(wynik, poz, zbior->elementy[i])) {
            wynik->elementy[poz] = zbior->elementy[i];
            generujWariacjeBezPowt(zbior, wynik, k, poz + 1);
        }
    }
}

void generujWariacjeZPowt(const Zbior *zbior, Wynik *wynik, int k, int poz) {
    if (poz == k) {
        wypiszWynik(wynik, k);
        return;
    }
    for (int i = 0; i < zbior->rozmiar; i++) {
        wynik->elementy[poz] = zbior->elementy[i];
        generujWariacjeZPowt(zbior, wynik, k, poz + 1);
    }
}

void generujKombinacje(const Zbior *zbior, Wynik *wynik, int k, int poz, int start) {
    if (poz == k) {
        wypiszWynik(wynik, k);
        return;
    }
    for (int i = start; i <= zbior->rozmiar - (k - poz); i++) {
        wynik->elementy[poz] = zbior->elementy[i];
        generujKombinacje(zbior, wynik, k, poz + 1, i + 1);
    }
}

void obliczIWypisz(const Zbior *zbior, const char *title, bool generateAll, int k) {
    Wynik wynik = {0};
    int n = zbior->rozmiar;

    if (czyRowne(title, "PERMUTACJA")) {
        printf("TOTAL=%llu\n", silnia(n));
        if (generateAll) generujWariacjeBezPowt(zbior, &wynik, n, 0);
    }
    else if (czyRowne(title, "KOMBINACJA")) {
        unsigned long long total = (k < 0 || k > n) ? 0 : silnia(n) / (silnia(k) * silnia(n - k));
        printf("TOTAL=%llu\n", total);
        if (generateAll && total > 0) generujKombinacje(zbior, &wynik, k, 0, 0);
    }
    else if (czyRowne(title, "WARIACJA_BEZ_POWTÓRZEŃ")) {
        unsigned long long total = (k < 0 || k > n) ? 0 : silnia(n) / silnia(n - k);
        printf("TOTAL=%llu\n", total);
        if (generateAll && total > 0) generujWariacjeBezPowt(zbior, &wynik, k, 0);
    }
    else if (czyRowne(title, "WARIACJA_Z_POWTÓRZENIAMI")) {
        printf("TOTAL=%llu\n", potega(n, k));
        if (generateAll) generujWariacjeZPowt(zbior, &wynik, k, 0);
    }
}

void przetworzStrumien(FILE *wejscie) {
    Zbior zbior = {0};

    char linia[512];
    char title[128] = "";
    bool generateAll = false;
    int k = 0, oczekiwaneN = 0, nrSekcji = 0;

    while (fgets(linia, sizeof(linia), wejscie) != NULL) {
        linia[strcspn(linia, "\r\n")] = '\0';

        if (czyZaczynaSieOd(linia, "#")) continue;

        if (czyZaczynaSieOd(linia, "TITLE=")) {
            strcpy(title, linia + strlen("TITLE="));
        } else if (czyZaczynaSieOd(linia, "GENERATE_ALL_SETS=")) {
            generateAll = czyRowne(linia + strlen("GENERATE_ALL_SETS="), "TRUE");
        } else if (czyZaczynaSieOd(linia, "K_ELEMENTS=")) {
            k = atoi(linia + strlen("K_ELEMENTS="));
        } else if (czyZaczynaSieOd(linia, "CARD_OF_SET=")) {
            oczekiwaneN = atoi(linia + strlen("CARD_OF_SET="));
        } else if (oczekiwaneN > 0 && !czyZawieraZnak(linia, '=')) {
            dodajDoZbioru(&zbior, linia);

            if (zbior.rozmiar == oczekiwaneN) {
                if (nrSekcji++ > 0) printf("\n");
                obliczIWypisz(&zbior, title, generateAll, k);

                zbior.rozmiar = 0;
                oczekiwaneN = 0;
                k = 0;
                generateAll = false;
                title[0] = '\0';
            }
        }
    }
}

int main(void) {
    if (jestTerminalem()) {
        char sciezka[512];
        printf("Podaj sciezke do pliku: ");
        fflush(stdout);

        if (fgets(sciezka, sizeof(sciezka), stdin) == NULL) return 1;
        sciezka[strcspn(sciezka, "\r\n")] = '\0';

        FILE *plik = fopen(sciezka, "r");
        if (plik == NULL) {
            printf("Nie mozna otworzyc pliku.\n");
            return 1;
        }

        przetworzStrumien(plik);
        fclose(plik);

        zakonczEnterem("Naciśnij Enter, aby zakończyć...");
    } else {
        przetworzStrumien(stdin);
    }

    return 0;
}