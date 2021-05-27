/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#define _GNU_SOURCE

#include <string.h>
#include "poly_parser.h"
#include "input.h"

/** Kod ASCII oznaczający 0
 */
#define ASCII_0 48

/** Kod ASCII oznaczający 0
 */
#define ASCII_9 57

/** Początkowa wielkość tablicy monosów używanej w funkcji convertStrToPoly
 */
#define INIT_MONOS_SIZE 16


/**
 * Sprawdza czy tablica charów zawiera tylko chary wyrażające liczby
 * @param[in] str : tablica charów
 * @return : czy @str jest tożsamościowy równy liczbie
 */
static bool containsOnlyNums(char *str) {
    char c;
    str = strtok(str, "\n");

    if (str == NULL) return false;

    ssize_t loopLen = (ssize_t) strlen(str);
    if (loopLen == 1 && str[0] == '-') return false;
    for (ssize_t i = 0; i < loopLen; ++i) {
        c = str[i];
        if (!((c >= ASCII_0 && c <= ASCII_9) || c == '\n' || c == '-')) return false;
    }
    return true;
}

/**
 * Sprawdza poprawność parametru przy wczytywaniu komendy AT i jeśli parametr
 * jest poprawny wykonuje komendę AT
 * @param[in] str : wczytywana linia
 * @param[in] lineLen : długość wczytywanej linii
 * @param[in] w : nr wczytywanej linii
 * @param[in] stack : stos
 */
static void parseAtComm(char *str, ssize_t lineLen, size_t w, StackT *stack) {
    if (lineLen > 4 && containsOnlyNums(&str[3]) && str[2] == ' ') {
        char *str_end;
        poly_coeff_t x = strtol(&str[3], &str_end, 10);
        if (errno == ERANGE) {
            fprintf(stderr, "ERROR %zu AT WRONG VALUE\n", w);
            return;
        }
        At(stack, w, x);
    } else {
        fprintf(stderr, "ERROR %zu AT WRONG VALUE\n", w);
    }
}

/**
 * Sprawdza poprawność parametru przy wczytywaniu komendy DEG_BY
 * i jeśli parametr jest poprawny wykonuje komendę DEG_BY
 * @param[in] str : wczytywana linia
 * @param[in] lineLen : długość wczytywanej linii
 * @param[in] w : nr wczytywanej linii
 * @param[in] stack : stos
 */
static void
parseDegByComm(char *str, ssize_t lineLen, size_t w, StackT *stack) {
    if (lineLen >= 8 && containsOnlyNums(&str[7]) && str[6] == ' ' &&
        str[7] != '-') {
        char *str_end;
        size_t idx = strtoul(&str[7], &str_end, 10);
        if (errno == ERANGE) {
            fprintf(stderr, "ERROR %zu DEG BY WRONG VARIABLE\n", w);
            return;
        }
        DegBy(stack, w, idx);
    } else {
        fprintf(stderr, "ERROR %zu DEG BY WRONG VARIABLE\n", w);
    }
}

void parseCommand(StackT *stack, size_t currLine, char *str, ssize_t lineLen) {
    char *command = strtok(str, "\n");

    if (strcmp(command, "ZERO") == 0) Zero(stack);
    else if (strcmp(command, "IS_COEFF") == 0)
        isCoeff(stack, currLine);
    else if (strcmp(command, "IS_ZERO") == 0)
        isZero(stack, currLine);
    else if (strcmp(command, "CLONE") == 0)
        Clone(stack, currLine);
    else if (strcmp(command, "ADD") == 0)
        Add(stack, currLine);
    else if (strcmp(command, "MUL") == 0)
        Mul(stack, currLine);
    else if (strcmp(command, "SUB") == 0)
        Sub(stack, currLine);
    else if (strcmp(command, "NEG") == 0)
        Neg(stack, currLine);
    else if (strcmp(command, "POP") == 0)
        PopInstr(stack, currLine);
    else if (strcmp(command, "IS_EQ") == 0)
        isEq(stack, currLine);
    else if (strcmp(command, "DEG") == 0)
        Deg(stack, currLine);
    else if (strcmp(command, "PRINT") == 0)
        PrintStack(stack, currLine);
    else if (strncmp(command, "AT", 2) == 0)
        parseAtComm(str, lineLen, currLine, stack);
    else if (strncmp(command, "DEG_BY", 6) == 0)
        parseDegByComm(str, lineLen, currLine, stack);
    else
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", currLine);

}

/**
 * Sprawdza czy wczytywana linia zawiera tylko znaki mogące wystąpić
 * w poprawnym wielomianie
 * @param[in] buffer : wczytywana linia
 * @param[in] lineLen : długość wczytywanej linii
 * @return : czy linia zawiera tylko poprawne znaki występujące w wielomianie
 */
static bool containsPolyChars(char *buffer, ssize_t lineLen) {
    char c;
    for (ssize_t i = 0; i < lineLen; ++i) {
        c = buffer[i];
        if (!((c >= ASCII_0 && c <= ASCII_9) || c == '+' || c == '-' || c == '(' ||
              c == ')' || c == ',' || c == '\n')) {
            return false;
        }
    }
    return true;
}

/**
 * Sprawdza czy wczytywany wielomian jest poprawny
 * @param[in] buffer : wczytywana linia
 * @param[in] lineLen : długość wczytywanej linii
 * @return : czy wczytywany wielomian jest poprawny
 */
static bool corrPolyInput(char *buffer, ssize_t lineLen) {
    char c;
    ssize_t bracketCount = 0, commaCount = 0, openBracketCount = 0;

    // Sprawdzam czy nawiasy są dobrze rozstawione oraz czy liczba przecinków
    // jest równa liczbie otwierających nawiasów
    for (ssize_t i = 0; i < lineLen; ++i) {
        c = buffer[i];
        if (c == ',') commaCount++;
        if (c == '(') {
            bracketCount++;
            openBracketCount++;
        }
        if (c == ')') bracketCount--;
        if (bracketCount < 0) return false;
    }
    if (bracketCount != 0 || openBracketCount != commaCount) return false;

    // Sprawdzam czy wielomian ma poprawną strukturę opierając się na znakach
    // interpunkcyjnych oraz plusach i minusach
    for (ssize_t i = 0; i < lineLen; ++i) {
        c = buffer[i];
        char *endPtr;

        if (c == ',') {
            if (i == lineLen - 1) return false;
            if (!isdigit(buffer[i + 1])) return false;
            unsigned long idx = strtoul(&buffer[i + 1], &endPtr, 10);
            if (errno == ERANGE || idx > INT_MAX) return false;
        }
        if (c == '+') {
            if (i == lineLen - 1 || i == 0) return false;
            if (buffer[i + 1] != '(' || buffer[i - 1] != ')') return false;
        }
        if (c == '-') {
            if (i == lineLen - 1) return false;
            if (buffer[i + 1] < ASCII_0 || buffer[i + 1] > ASCII_9) return false;
            strtoll(&buffer[i], &endPtr, 10);
            if (errno == ERANGE) return false;
        }
        if (c == ')') {
            if (i <= lineLen - 3 && buffer[i + 1] == ',' &&
                (buffer[i + 2] < ASCII_0 || buffer[i + 2] > ASCII_9))
                return false;
            if (i <= lineLen - 2 && (buffer[i + 1] == '('
                                     || buffer[i + 1] == ')'))
                return false;
        }
        if (c == '(') {
            if (buffer[i + 1] == '+' || buffer[i + 1] == ',' ||
                buffer[i + 1] == ')')
                return false;
            if (buffer[i + 1] != '(' && buffer[i + 1] != '-') {
                strtoll(&buffer[i + 1], &endPtr, 10);
                if (errno == ERANGE) return false;
            }
        }
    }
    return true;
}

/**
 * Parsuje wczytywaną linię do wielomianu
 * @param[in] str : wczytywana linia
 * @param[in] strSize : długość wczytywanej linii
 * @param[in] strIndex : indeks na którym w tablicy znajduje się nowy wielomian
 * @return : sparsowany wielomian
 */
static Poly convertStrToPoly(char *str, ssize_t strSize, ssize_t *strIndex) {

    Poly res = PolyZero();
    size_t monosSize = INIT_MONOS_SIZE, nextFreeInd = 0;
    Mono *monos = safeMalloc(monosSize * sizeof(Mono));
    char *endPtr;
    for (ssize_t i = *strIndex; i < strSize; ++i) {

        if (str[i] == '(' && str[i + 1] == '(') {

            *strIndex = i + 1;
            Poly p = convertStrToPoly(str, strSize, strIndex);
            poly_exp_t exp = (poly_exp_t) strtol(&str[*strIndex], &endPtr, 10);
            Mono m = {.p = p, .exp = exp};
            monos[nextFreeInd++] = m;
            i = *strIndex;

        } else if (str[i] == '(' && str[i + 1] != '(') {

            if (monosSize == nextFreeInd) ExpandMonoArr(&monosSize, &monos);

            poly_coeff_t coeff = strtol(&str[i + 1], &endPtr, 10);
            poly_exp_t exp = (poly_exp_t) strtol((endPtr + 1), &endPtr, 10);
            i += (int) (endPtr - &(str[i])) - 1;

            Poly p = PolyFromCoeff(coeff);
            if (coeff != 0) {
                Mono m = MonoFromPoly(&p, exp);
                monos[nextFreeInd++] = m;
            }
        } else if ((str[i] == ')' && i == strSize - 1)
                   || (i < strSize - 1 && str[i] == ')' && str[i + 1] == ',') ||
                   str[i] == '\n') {

            monosSize = nextFreeInd;
            *strIndex = i + 2;
            res = PolyAddMonos(monosSize, monos);
            free(monos);
            return res;
        }
    }
    return res;
}

void parsePoly(StackT *stack, size_t currLine, char *buffer, ssize_t lineLen) {

    // Sprawdzam czy wielomian jest poprawny
    if (!containsPolyChars(buffer, lineLen) ||
        !corrPolyInput(buffer, lineLen)) {
        fprintf(stderr, "ERROR %zu WRONG POLY\n", currLine);
        return;
    }

    // Jeśli wielomian jest stałą
    if (buffer[0] != '(') {
        char *endPtr;
        long long coeff = strtoll(&buffer[0], &endPtr, 10);
        if (errno == ERANGE) {
            fprintf(stderr, "ERROR %zu WRONG POLY\n", currLine);
        }
        Push(stack, PolyFromCoeff(coeff));
        return;
    }
    // Jeśli jest jednomianem lub sumą jednomianów
    ssize_t strInd = 0;
    if (buffer[lineLen - 1] == '\n') {
        Poly p = convertStrToPoly(buffer, lineLen - 1, &strInd);
        Push(stack, p);
    }
}