/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#include "stack_operations.h"

void Zero(StackT *stack) {
    Push(stack, PolyZero());
}

void isCoeff(StackT *stack, size_t w) {
    if (!isEmpty(*stack)) {
        Poly p = Top(*stack);
        fprintf(stdout, "%d\n", isPolyCoeffRec(&p));
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
    }
}

void isZero(StackT *stack, size_t w) {
    if (!isEmpty(*stack)) {
        Poly p = Top(*stack);
        fprintf(stdout, "%d\n", isPolyZeroRec(&p));
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
    }

}

void Clone(StackT *stack, size_t w) {
    if (!isEmpty(*stack)) {
        Poly p = Top(*stack);
        Poly res = PolyClone(&p);
        Push(stack, res);
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
    }
}

void Add(StackT *stack, size_t w) {
    if (has2Polys(*stack)) {
        Poly p1 = Pop(stack), p2 = Pop(stack);
        Poly res = PolyAdd(&p1, &p2);
        PolyDestroy(&p1);
        PolyDestroy(&p2);
        Push(stack, res);
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
    }
}

void Mul(StackT *stack, size_t w) {
    if (has2Polys(*stack)) {
        Poly p1 = Pop(stack), p2 = Pop(stack);
        Poly res = PolyMul(&p1, &p2);
        PolyDestroy(&p1);
        PolyDestroy(&p2);
        Push(stack, res);
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
    }
}

void Neg(StackT *stack, size_t w) {
    if (!isEmpty(*stack)) {
        Poly p1 = Pop(stack);
        Poly res = PolyNeg(&p1);
        PolyDestroy(&p1);
        Push(stack, res);
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
    }
}

void Sub(StackT *stack, size_t w) {
    if (has2Polys(*stack)) {
        Poly p1 = Pop(stack), p2 = Pop(stack);
        Poly res = PolySub(&p1, &p2);
        PolyDestroy(&p1);
        PolyDestroy(&p2);
        Push(stack, res);
        return;
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
        return;
    }
}

void isEq(StackT *stack, size_t w) {
    if (has2Polys(*stack)) {
        Poly p1 = GetSecondPoly(stack), p2 = Top(*stack);
        fprintf(stdout, "%d\n", PolyIsEq(&p1, &p2));
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
    }
}

void Deg(StackT *stack, size_t w) {
    if (!isEmpty(*stack)) {
        Poly p = Top(*stack);
        fprintf(stdout, "%d\n", PolyDeg(&p));
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
        return;
    }
}

void DegBy(StackT *stack, size_t w, size_t idx) {
    if (!isEmpty(*stack)) {
        Poly p = Top(*stack);
        fprintf(stdout, "%d\n", PolyDegBy(&p, idx));
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
        return;
    }
}

void At(StackT *stack, size_t w, long long x) {
    if (!isEmpty(*stack)) {
        Poly p = Pop(stack);
        Poly res = PolyAt(&p, x);
        PolyDestroy(&p);
        Push(stack, res);
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
        return;
    }
}

/**
 * Wypisuje jednomian na standardowe wejście
 * @param[in] m : jednomian
 */
static void PrintMono(Mono *m);


/**
 * Wypisuje wielomian na standardowe wejście
 * @param[in] p : wielomian
 */
static void PrintPoly(Poly *p) {
    if (isPolyCoeffRec(p))
        printf("%ld", getCoeff(p));
    else {
        if (p->size == 1) {
            PrintMono(&p->arr[0]);
            return;
        }
        for (size_t i = 0; i < p->size; i++) {
            PrintMono(&p->arr[i]);
            if (i != p->size - 1 && p->size != 1)
                printf("+");
        }
    }
}

static void PrintMono(Mono *m) {
    printf("(");
    PrintPoly(&m->p);
    printf(",%d)", m->exp);
}



void PopInstr(StackT *stack, size_t w) {
    if (!isEmpty(*stack)) {
        Poly p = Pop(stack);
        PolyDestroy(&p);
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
        return;
    }
}

void PrintStack(StackT *stack, size_t w) {
    if (!isEmpty(*stack)) {
        Poly p = Top(*stack);
        PrintPoly(&p);
        printf("\n");
    } else {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", w);
        return;
    }
}