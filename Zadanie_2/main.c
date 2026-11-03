#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    int dzielna;
    int iloraz;
    int dzielnik;
    int reszta;
} Krok;

typedef struct {
    Krok tab[100];
    int top;
} StosKrokow;

typedef struct {
    int a;
    int x;
    int b;
    int y;
} Wynik;

void push(StosKrokow *stos, Krok krok) {
    if (stos->top < 100) {
        stos->tab[stos->top] = krok;
        stos->top++;
    }
}

Krok pop(StosKrokow *s) {
    if (s->top > 0) {
        s->top--;
        return s->tab[s->top];
    }
    Krok pusty = {0, 0, 0, 0};
    return pusty;
}

bool czyPusty(const StosKrokow *stos) {
    return stos->top == 0;
}

Krok topEl(const StosKrokow *stos) {
    if (stos->top > 0) {
        return stos->tab[stos->top - 1];
    }
    Krok pusty = {0, 0, 0, 0};
    return pusty;
}

Wynik stworzWynik(Krok krok) {
    Wynik w;
    w.x = 1;
    w.a = krok.dzielna;
    w.y = -krok.iloraz;
    w.b = krok.dzielnik;
    return w;
}

void aktualizujWynik(Wynik *wynik, Krok krok) {
    int staryX = wynik->x;
    int staryY = wynik->y;

    wynik->x = staryY;
    wynik->a = krok.dzielna;
    wynik->y = staryX + staryY * (-krok.iloraz);
    wynik->b = krok.dzielnik;
}

void wynikWypiszPodstawienie(const Wynik *wynik, Krok krok) {
    printf(" = ");
    if (wynik->x == 1) printf("%d", wynik->a);
    else if (wynik->x == -1) printf("-%d", wynik->a);
    else printf("%d * %d", wynik->x, wynik->a);

    if (wynik->y > 0) printf(" + %d * ", wynik->y);
    else printf(" - %d * ", abs(wynik->y));

    printf("(%d - %d * %d)", krok.dzielna, krok.iloraz, krok.dzielnik);
}

void wynikWypiszZredukowane(const Wynik *wynik, bool ostatniElement) {
    printf(" = ");
    if (ostatniElement) {
        printf("%d * %d + (%d) * %d", wynik->x, wynik->a, wynik->y, wynik->b);
    } else {
        if (wynik->x == -1) printf("-%d", wynik->a);
        else if (wynik->x == 1) printf("%d", wynik->a);
        else printf("%d * %d", wynik->x, wynik->a);

        if (wynik->y < 0) printf(" - %d * %d", abs(wynik->y), wynik->b);
        else printf(" + %d * %d", wynik->y, wynik->b);
    }
}

void wypiszZielonaRamke(StosKrokow *stos, int nwd, Wynik *wynik) {
    printf("%d = %d - %d * %d", nwd, wynik->a, abs(wynik->y), wynik->b);

    while (!czyPusty(stos)) {
        Krok krok = pop(stos);
        wynikWypiszPodstawienie(wynik, krok);
        aktualizujWynik(wynik, krok);
        wynikWypiszZredukowane(wynik, czyPusty(stos));
    }
    printf("\n");
}

void wynikWypiszCzarnaRamke(const int a, const int x, const int b, const int y, const int nwd) {
    printf("tj.\n");
    printf("   NWD(%d,%d) = %d = %d * %d + %d * (%d)\n", a, b, nwd, a, x, b, y);
    printf("            x = %d    y = %d\n\n", x, y);
}

int main(void) {
    int a, b;

    printf("Wprowadź a = "); scanf("%d", &a);
    printf("Wprowadź b = "); scanf("%d", &b);

    int tmpA = a, tmpB = b, reszta;
    StosKrokow stos = { .top = 0 };

    while (b != 0) {
        reszta = a % b;
        int iloraz = (int)floor((double)a / b);

        Krok krok = {a, iloraz, b, reszta};
        push(&stos, krok);

        a = b;
        b = reszta;
    }

    if (!czyPusty(&stos) && topEl(&stos).reszta == 0) {
        pop(&stos);
    }
    if (czyPusty(&stos)) return 0;

    Krok ostatniKrok = pop(&stos);
    Wynik wynik = stworzWynik(ostatniKrok);
    wypiszZielonaRamke(&stos, ostatniKrok.reszta, &wynik);
    wynikWypiszCzarnaRamke(tmpA, wynik.x, tmpB, wynik.y, ostatniKrok.reszta);

    return 0;
}