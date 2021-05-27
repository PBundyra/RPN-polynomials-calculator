/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#ifndef POLYNOMIALS_INPUT_H
#define POLYNOMIALS_INPUT_H

#include "ctype.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/**
 * Zapewnia bezpieczne wczytanie linii, jeśli zabraknie pamięci program
 * zakończy się z kodem 1. Zwraca -1 jeśli wczytanie się nie uda
 * @param[in] line : tablica charów w której przechowywana będzie wczytana linia
 * @param[in] length : rozmiar tablicy
 * @return : długość wczytanej linii
 */
extern ssize_t safeGetline(char **line, size_t *length);

/**
 * Zapewnia bezpieczną alokację pamięci, jeśli zabraknie pamięci program
 * zakończy się z kodem 1.
 * @param[in] size : wielkość zaalokowanej pamięci
 * @return : wskaźnik na zaalokwaną pamięć
 */
extern void *safeMalloc(size_t size);


#endif //POLYNOMIALS_INPUT_H
