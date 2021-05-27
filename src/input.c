/** @file
 * @author Patryk Bundyra
 * @date 2021
 */
#define _GNU_SOURCE

#include "input.h"

#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)

ssize_t safeGetline(char **line, size_t *length) {
    errno = 0;
    ssize_t lineLen = getline(line, length, stdin);
    if (errno == ENOMEM) exit(1);
    return lineLen;
}

void *safeMalloc(size_t size) {
    void *res = malloc(size);
    CHECK_PTR(res);
    return res;
}
