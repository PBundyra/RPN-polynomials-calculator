/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#include "poly_stack.h"
#include <stdbool.h>
#include "input.h"

/**
 * Zwraca nowy (dwukrotnie wiekszy) rozmiar stosu
 * @param[in] num : obecny rozmiar stosu
 * @return : dwukrotnie wiekszy rozmiar stosu
 */
static stackSizeT newSize(stackSizeT num) {
    return 2 * num + 1;
}

/**
 * Zwieksza dwukrotnie zaalokowana pamięć na stos
 * @param[in] stack : stos
 */
static void ExpandStack(StackT *stack) {
    stack->size = newSize(stack->size);
    stack->polyArr = realloc(stack->polyArr, stack->size * sizeof(Poly));
}

bool isEmpty(StackT stack) { return stack.nextFreeInd == 0; }

bool has2Polys(StackT stack) { return stack.nextFreeInd > 1; }

void Push(StackT *stack, Poly p) {
    if (stack->size == stack->nextFreeInd + 1) {
        ExpandStack(stack);
    }

    stack->polyArr[stack->nextFreeInd++] = p;
}

Poly Top(StackT stack) { return stack.polyArr[stack.nextFreeInd - 1]; }

Poly Pop(StackT *stack) {
    Poly tempPoly = PolyClone(&stack->polyArr[stack->nextFreeInd - 1]);
    PolyDestroy(&stack->polyArr[stack->nextFreeInd - 1]);
    stack->nextFreeInd--;
    return tempPoly;
}

Poly GetSecondPoly (StackT *stack){
    Poly tempPoly = Pop(stack);
    Poly res = Top(*stack);
    Push(stack, tempPoly);
    return res;
}

StackT StackInit(stackSizeT size){
    StackT stack;
    stack.size = size;
    stack.polyArr = safeMalloc(stack.size * sizeof(Poly));
    stack.nextFreeInd = 0;
    return stack;
}

void StackDestroy(StackT *stack) {
    for (stackSizeT i = 0; i < stack->nextFreeInd; ++i) {
        PolyDestroy(&stack->polyArr[i]);
    }

    free(stack->polyArr);
}

