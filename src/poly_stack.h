/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#ifndef POLYNOMIALS_POLY_STACK_H
#define POLYNOMIALS_POLY_STACK_H

#include "poly.h"

/** Typ używany do przechoywania rozmiaru stosu
 */
typedef unsigned long long int stackSizeT;

/**
 * Struktura stosu (implementowanego na tablicy) wielomianów zawierająca
 * tablice wielomianów, rozmiar tablicy (stosu) oraz indeks na którym
 * powinniśmy zapisać następny wielomian
 * @
 */
typedef struct StackT {
    Poly *polyArr;
    stackSizeT size;
    stackSizeT nextFreeInd;
} StackT;

/**
 * Sprawdcza czy stos jest pusty
 * @param[in] stack : stos
 * @return : czy stos jest pusty
 */
extern bool isEmpty(StackT stack);

/**
 * Sprawdza czy na stosie są przynajmniej 2 wielomiany
 * @param[in] stack : stos
 * @return : czy na stosie są przynajmniej 2 wielomiany
 */
extern bool has2Polys(StackT stack);

/**
 * Wklada przekazany wielomian na wierzchołek stosu
 * @param[in] stack : stos
 * @param[in] p : wielomian
 */
extern void Push(StackT *stack, Poly p);

/**
 * Zwraca wielomian z wierzchołka stosu
 * @param[in] stack : stos
 * @return
 */
extern Poly Top(StackT stack);

/**
 * Zdejmuje wielomian z wierzchołka stosu i go zwraca
 * @param[in] stack : stos
 * @return : wielomian zdjęty z wierzchołka stosu
 */
extern Poly Pop(StackT *stack);

/**
 * Zwraca drugi od góry wielomian na stosie
 * @param[in] stack : stos
 * @return : drugi od góry wielomian na stosie
 */
extern Poly GetSecondPoly (StackT *stack);

/**
 * Zwalnia zaalokowaną na stos pamięć
 * @param[in] stack : stos
 */
extern void StackDestroy(StackT *stack);

/**
 * Inicjalizuje stos o podanym rozmiarze
 * @param[in] size : rozmiar inicjalizowanego stosu
 * @return : zainicjalizowany stos
 */
extern StackT StackInit(stackSizeT size);


#endif //POLYNOMIALS_POLY_STACK_H
