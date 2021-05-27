/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#ifndef POLYNOMIALS_STACK_OPERATIONS_H
#define POLYNOMIALS_STACK_OPERATIONS_H

#include <stdio.h>
#include "poly_stack.h"

/**
 * Wkłada na stos wielomian zerowy
 * @param[in] stack
 */
extern void Zero(StackT *stack);

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem
 * – wypisuje na standardowe wyjście 0 lub 1
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void isCoeff(StackT *stack, size_t w);

/**
 *  Sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru
 *  – wypisuje na standardowe wyjście 0 lub 1;
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void isZero(StackT *stack, size_t w);

/**
 * Wstawia na stos kopię wielomianu z wierzchołka
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void Clone(StackT *stack, size_t w);

/**
 * Dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek
 * stosu ich sumę
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void Add(StackT *stack, size_t w);

/**
 * Mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek
 * stosu ich iloczyn;
 * stosu ich sumę
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void Mul(StackT *stack, size_t w);

/**
 * Neguje wielomian na wierzchołku stosu
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void Neg(StackT *stack, size_t w);

/**
 * Odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem,
 * usuwa je i wstawia na wierzchołek stosu różnicę;
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void Sub(StackT *stack, size_t w);

/**
 * Sprawdza, czy dwa wielomiany na wierzchu stosu są równe – wypisuje na
 * standardowe wyjście 0 lub 1;
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void isEq(StackT *stack, size_t w);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu
 * (−1 dla wielomianu tożsamościowo równego zeru);
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void Deg(StackT *stack, size_t w);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną
 * o numerze idx (−1 dla wielomianu tożsamościowo równego zeru);
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 * @param[in] idx : indeks zmiennej
 */
extern void DegBy(StackT *stack, size_t w, size_t idx);

/**
 * Wylicza wartość wielomianu w punkcie x, usuwa wielomian z wierzchołka
 * i wstawia na stos wynik operacji;
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 * @param[in] x : punkt dla którego jest wyliczana wartość
 */
extern void At(StackT *stack, size_t w, long long x);

/**
 * Usuwa wielomian z wierzchołka stosu.
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void PopInstr(StackT *stack, size_t w);

/**
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu;
 * @param[in] stack : stos
 * @param[in] w : nr wczytywanej linii
 */
extern void PrintStack(StackT *stack, size_t w);

#endif //POLYNOMIALS_STACK_OPERATIONS_H
