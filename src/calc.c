/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#define _GNU_SOURCE

#include "poly_stack.h"
#include <string.h>
#include "poly_parser.h"
#include "input.h"

/** Niepoprawny char
*/
#define INVALID_CHAR '@'

/** Znak oznaczający komentarz
 */
#define COMMENT_CHAR '#'

/** Znak oznaczjący nową linie
 */
#define NEW_LINE_CHAR '\n'

/** Początkowy rozmiar stosu
 */
#define INIT_STACK_SIZE 16

/** Początkowy rozmiar bufora w którym przechoywana jest wczytywana linia
 */
#define INIT_BUFFER_SIZE 16


int main(void) {

    StackT stack = StackInit(INIT_STACK_SIZE);
    char *buffer = safeMalloc(INIT_BUFFER_SIZE * sizeof (char));
    size_t bufSize = 1, currLine = 1;
    ssize_t lineLen;

    char c;
    while ((c = getchar()) != EOF) {
        ungetc(c, stdin);
        lineLen = safeGetline(&buffer, &bufSize);
        if (c != COMMENT_CHAR && c != NEW_LINE_CHAR) {

            size_t len = strlen(buffer);
            if (len != (size_t) lineLen) buffer[len] = INVALID_CHAR;
            if (isalpha(buffer[0]))
                parseCommand(&stack, currLine, buffer, lineLen);
            else
                parsePoly(&stack, currLine, buffer, lineLen);
        }
        currLine++;
    }

    free(buffer);
    StackDestroy(&stack);

    return 0;
}
