#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(_WIN32)
#include <io.h>
#define jestTerminalem() _isatty(_fileno(stdin))
#else
#include <unistd.h>
#define jestTerminalem() isatty(STDIN_FILENO)
#endif

typedef struct Krok {
    int dzielna;
    int iloraz;
    int dzielnik;
    int reszta;
    struct Krok *nastepny;
} Krok;

typedef struct {
    Krok *top;
} StosKrokow;

typedef struct {
    int a;
    int x;
    int b;
    int y;
} Wynik;

bool czyPusty(const StosKrokow *stos) {
    return stos->top == NULL;
}

void push(StosKrokow *stos, Krok krok) {
    Krok *nowy = malloc(sizeof(Krok));
    if (nowy == NULL) return;

    *nowy = krok;
    nowy->nastepny = stos->top;
    stos->top = nowy;
}

Krok pop(StosKrokow *stos) {
    if (czyPusty(stos)) return (Krok){0, 0, 0, 0, NULL};

    Krok *temp = stos->top;
    Krok krok = *temp;
    stos->top = temp->nastepny;

    free(temp);
    krok.nastepny = NULL;
    return krok;
}

Wynik stworzWynik(Krok krok) {
    Wynik w;
    w.x = 1;
    w.a = krok.dzielna;
    w.y = -krok.iloraz;
    w.b = krok.dzielnik;
    return w;
}

void zakonczEnterem(const char *komunikat) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (komunikat != NULL) printf("%s", komunikat);
    getchar();
}

void przeliczWspolczynniki(Wynik *wynik, Krok krok) {
    int staryX = wynik->x;
    int staryY = wynik->y;

    wynik->x = staryY;
    wynik->a = krok.dzielna;
    wynik->y = staryX + staryY * (-krok.iloraz);
    wynik->b = krok.dzielnik;
}

void wypiszPodstawienieRownania(const Wynik *wynik, Krok krok) {
    printf(" = ");
    wynik->x == 1 ? printf("%d", wynik->a) :
    wynik->x == -1 ? printf("-%d", wynik->a) : printf("%d * %d", wynik->x, wynik->a);

    printf(" %c %d * (%d - %d * %d)",
           wynik->y < 0 ? '-' : '+', abs(wynik->y),
           krok.dzielna, krok.iloraz, krok.dzielnik);
}

void wypiszRownanieZredukowane(const Wynik *wynik, bool ostatniElement) {
    printf(" = ");
    if (ostatniElement) {
        wynik->x < 0 ? printf("(%d) * %d", wynik->x, wynik->a) : printf("%d * %d", wynik->x, wynik->a);
        printf(" + ");
        wynik->y < 0 ? printf("(%d) * %d", wynik->y, wynik->b) : printf("%d * %d", wynik->y, wynik->b);
    } else {
        (wynik->x == 1) ? printf("%d", wynik->a) :
        (wynik->x == -1) ? printf("-%d", wynik->a) : printf("%d * %d", wynik->x, wynik->a);
        printf(" %c %d * %d", wynik->y < 0 ? '-' : '+', abs(wynik->y), wynik->b);
    }
}

void wypiszRozwiazanieZZielonejRamki(Wynik *wynik, int nwd, StosKrokow *stos) {
    printf("%d = %d - %d * %d", nwd, wynik->a, abs(wynik->y), wynik->b);

    while (!czyPusty(stos)) {
        Krok krok = pop(stos);
        wypiszPodstawienieRownania(wynik, krok);
        przeliczWspolczynniki(wynik, krok);
        wypiszRownanieZredukowane(wynik, czyPusty(stos));
    }
    printf("\n");
}

void wypiszRozwiazanieZCzarnejRamki(const Wynik *wynik, const int nwd) {
    int a = wynik->a, b = wynik->b, x = wynik->x, y = wynik->y;
    printf("NWD(%d, %d) = %d = %d * ", a, b, nwd, a);
    x < 0 ? printf("(%d)", x) : printf("%d", x);

    printf(" + %d * ", b);
    y < 0 ? printf("(%d)\n", y) : printf("%d\n", y);

    printf("x = %d    y = %d\n\n", x, y);
}

void przetworzPareDanych(int a, int b) {
    int tmpA = a, tmpB = b;
    StosKrokow stos = { .top = NULL };

    while (b != 0) {
        int reszta = a % b;
        if (reszta == 0) break;
        int iloraz = a / b;

        Krok krok = {a, iloraz, b, reszta, NULL};
        push(&stos, krok);

        a = b;
        b = reszta;
    }

    Wynik wynik;
    int nwd;

    if (czyPusty(&stos)) {
        nwd = tmpB;
        wynik = (Wynik){tmpA, 0, tmpB, 1};
    } else {
        Krok ostatniKrok = pop(&stos);
        nwd = ostatniKrok.reszta;
        wynik = stworzWynik(ostatniKrok);
    }

    wypiszRozwiazanieZZielonejRamki(&wynik, nwd, &stos);
    wypiszRozwiazanieZCzarnejRamki(&wynik, nwd);
}

int main(void) {
    int a, b;

    if (jestTerminalem()) {
        printf("Wprowadź a = ");
        if (scanf("%d", &a) != 1) return 1;
        printf("Wprowadź b = ");
        if (scanf("%d", &b) != 1) return 1;

        przetworzPareDanych(a, b);

        zakonczEnterem("Naciśnij Enter, aby zakończyć...");
    } else {
        int iloscWierszy;
        if (scanf("%d", &iloscWierszy) == 1) {
            for (int i = 0; i < iloscWierszy; i++) {
                if (scanf("%d %d", &a, &b) == 2) {
                    printf("DANE Z WIERSZA [%d]: a = %d, b = %d\n", i+1, a, b);
                    przetworzPareDanych(a, b);
                }
            }
        }
    }

    return 0;
}