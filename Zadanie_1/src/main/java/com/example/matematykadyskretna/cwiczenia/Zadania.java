package com.example.matematykadyskretna.cwiczenia;

public class Zadania {

    // --- Zadanie 1 ---
    public static String getFactors1(int number) {
        if (number <= 1) return String.valueOf(number);

        StringBuilder wynik = new StringBuilder();
        int temp = number;

        for (int dzielnik = 2; dzielnik * dzielnik <= temp; dzielnik++) {
            if (temp % dzielnik == 0) {
                int wykladnik = 0;

                while (temp % dzielnik == 0) {
                    wykladnik++;
                    temp /= dzielnik;
                }

                if (!wynik.isEmpty()) wynik.append("*");
                wynik.append(dzielnik);
                if (wykladnik > 1) wynik.append("^").append(wykladnik);
            }
        }

        if (temp > 1) {
            if (!wynik.isEmpty()) wynik.append("*");
            wynik.append(temp);
        }

        return wynik.toString();
    }

    // --- Zadanie 2 ---
    public static String getFactors2(int number) {
        if (number <= 1) return "[" + number + "]";

        StringBuilder wynik = new StringBuilder("[");
        int temp = number;
        int aktualnaPierwsza = 2;

        while (temp > 1) {
            int wykladnik = 0;
            while (temp % aktualnaPierwsza == 0) {
                wykladnik++;
                temp /= aktualnaPierwsza;
            }

            wynik.append(wykladnik);

            if (temp > 1) {
                aktualnaPierwsza = nastepnaLiczbaPierwsza(aktualnaPierwsza);
            }
        }

        wynik.append("]");
        return wynik.toString();
    }

    // --- Zadanie 3 ---
    public static int getNWD(int a, int b) {
        a = Math.abs(a);
        b = Math.abs(b);

        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // --- Zadanie 4 ---
    public static int getNWW(int a, int b) {
        if (a == 0 || b == 0) return 0;

        a = Math.abs(a);
        b = Math.abs(b);

        return (a / getNWD(a, b)) * b;
    }

    // --- Zadanie 5 ---
    public static boolean checkPrimary(int number) {
        if (number < 2) return false;
        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) return false;
        }
        return true;
    }

    // --- Zadanie 6 ---
    public static boolean checkCoprimary(int a, int b) {
        return getNWD(a, b) == 1;
    }

    // --- Zadanie 7 ---
    public static int getPrimaryNumbers(int number) {
        if (number < 2) return 0;

        int licznik = 0;
        int pierwsza = 2;

        while (pierwsza <= number) {
            licznik++;
            pierwsza = nastepnaLiczbaPierwsza(pierwsza);
        }
        return licznik;
    }

    // --- Zadanie 8 ---
    public static int getEuler(int number) {
        if (number <= 0) return 0;

        int count = 0;
        for (int i = 1; i <= number; i++) {
            boolean liczbyWzgledniePierwsze = checkCoprimary(number, i);
            if (liczbyWzgledniePierwsze) count++;
        }
        return count;
    }






    private static int nastepnaLiczbaPierwsza(int start) {
        int liczba = start + 1;
        while (!checkPrimary(liczba)) {
            liczba++;
        }
        return liczba;
    }



    public static void main(String[] args) {
        System.out.println("--- Zadanie 1 ---");
        System.out.println("getFactors1(72) -> " + getFactors1(72));
        System.out.println("getFactors1(12) -> " + getFactors1(12));
        System.out.println("getFactors1(13) -> " + getFactors1(13));
        System.out.println("getFactors1(100) -> " + getFactors1(100));

        System.out.println("--- Zadanie 2 ---");
        System.out.println("getFactors2(72) -> " + getFactors2(72));
        System.out.println("getFactors2(20) -> " + getFactors2(20));
        System.out.println("getFactors2(12) -> " + getFactors2(12));
        System.out.println("getFactors2(100) -> " + getFactors2(100));
        System.out.println("getFactors2(13) -> " + getFactors2(13));

        System.out.println("--- Zadanie 3 ---");
        System.out.println("getNWD(6, 14) -> " + getNWD(6, 14));
        System.out.println("getNWD(192, 270) -> " + getNWD(192, 270));
        System.out.println("getNWD(17, 13) -> " + getNWD(17, 13));

        System.out.println("--- Zadanie 4 ---");
        System.out.println("getNWW(6, 14) -> " + getNWW(6, 14));
        System.out.println("getNWW(12, 18) -> " + getNWW(12, 18));
        System.out.println("getNWW(7, 5) -> " + getNWW(7, 5));

        System.out.println("--- Zadanie 5 ---");
        System.out.println("checkPrimary(6) -> " + checkPrimary(6));
        System.out.println("checkPrimary(7) -> " + checkPrimary(7));
        System.out.println("checkPrimary(2) -> " + checkPrimary(2));
        System.out.println("checkPrimary(1) -> " + checkPrimary(1));

        System.out.println("--- Zadanie 6 ---");
        System.out.println("checkCoprimary(6, 14) -> " + checkCoprimary(6, 14));
        System.out.println("checkCoprimary(8, 15) -> " + checkCoprimary(8, 15));
        System.out.println("checkCoprimary(17, 13) -> " + checkCoprimary(17, 13));

        System.out.println("--- Zadanie 7 ---");
        System.out.println("getPrimaryNumbers(14) -> " + getPrimaryNumbers(14));
        System.out.println("getPrimaryNumbers(13) -> " + getPrimaryNumbers(13));
        System.out.println("getPrimaryNumbers(20) -> " + getPrimaryNumbers(20));

        System.out.println("--- Zadanie 8 ---");
        System.out.println("getEuler(72) -> " + getEuler(72));
        System.out.println("getEuler(9) -> " + getEuler(9));
        System.out.println("getEuler(10) -> " + getEuler(10));
    }
}
