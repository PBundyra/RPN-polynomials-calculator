/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#ifndef POLYNOMIALS_POLY_PARSER_H
#define POLYNOMIALS_POLY_PARSER_H

#include "stack_operations.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Sprawdza poprawność wczytywanej komendy, jeśli komenda jest poprawna
 * wykonuje ją
 * @param[in] stack : stos
 * @param[in] currLine : nr wczytywanej linii
 * @param[in] buffer : wczytywana linia
 * @param[in] lineLen : długość wczytywanej linii
 */
extern void parseCommand(StackT *stack, size_t currLine, char *buffer, ssize_t lineLen);

/**
 * Sprawdza poprawność wczytywanego wielomianu, jeśli wielomian jest poprawny
 * parsuje tablicę charów do odpowiedniego wielomianu i wkłada go na stos
 * @param[in] stack : stos
 * @param[in] currLine : nr wczytywanej linii
 * @param[in] buffer : wczytywana linia
 * @param[in] lineLen : długość wczytywanej linii
 */
extern void parsePoly(StackT *stack, size_t currLine, char *buffer, ssize_t lineLen);

#endif //POLYNOMIALS_POLY_PARSER_H