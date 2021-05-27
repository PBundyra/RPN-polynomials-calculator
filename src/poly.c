/** @file
 * @author Patryk Bundyra
 * @date 2021
 */

#include "poly.h"
#include <stdlib.h>
#include <errno.h>
#include "input.h"

/** Poczatkowy rozmiar tablicy monosow w PolyMul
 */
#define INIT_MONOS_SIZE 16

/** Sprawdza poprawność alokacji
*/
#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)

void PolyDestroy(Poly *p) {
    if (p->arr) {
        for (size_t i = 0; i < p->size; ++i) {
            MonoDestroy(&p->arr[i]);
        }
        free(p->arr);
    }
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) return PolyFromCoeff(p->coeff);

    Poly clone = {.size = p->size, .arr = calloc(p->size, sizeof(Mono))};
    CHECK_PTR(&clone.arr);
    for (size_t i = 0; i < p->size; ++i) {
        clone.arr[i] = MonoClone(&p->arr[i]);
    }
    return clone;
}

/**
 * Sprawdza równość jednomianów - funkcja wywoływana prze quicksort
 * @param[in] a : jednomian
 * @param[in] b : jednomian
 * @return : czy jednomiany są równe
 */
static int CmpMonos(const void *a, const void *b) {
    if ((*(Mono *) a).exp > (*(Mono *) b).exp) return 1;
    if ((*(Mono *) a).exp == (*(Mono *) b).exp) return 0;
    return -1;
}

/**
 * Sortuje wielomian w porządku rosnącym względem współczynnika potęgowego
 * przy zmienne x_0
 * @param[in] p : wielomian
 */
static void PolySort(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; ++i) {
            PolySort(&p->arr[i].p);
        }
        qsort(p->arr, p->size, sizeof(Mono), CmpMonos);
    }
}

/**
 * Sortuje tablice jednomianów w porządku rosnącym względem wspólczynnika
 * potęgowego przy zmiennej x_0
 * @param[in] count : długość tablicy @monos
 * @param[in] monos : tablica jednomianów
 */
static void SortMonos(size_t count, Mono monos[]) {
    qsort(monos, count, sizeof(Mono), CmpMonos);
    for (size_t i = 0; i < count; ++i) {
        PolySort(&monos[i].p);
    }
}

/**
 * Sprawdza czy suma dwóch wielomianów da wielomian zerowy
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return Czy suma dwóch wielomianów da wielomian zerowy
 */
static bool isAddResEqZero(Poly *p, Poly *q) {
    Poly pCopy = PolyClone(p), qCopy = PolyClone(q);
    Poly sum = PolyAdd(&pCopy, &qCopy);
    bool res = false;
    if (isPolyZeroRec(&sum)) res = true;
    PolyDestroy(&pCopy);
    PolyDestroy(&qCopy);
    PolyDestroy(&sum);
    return res;
}

/**
 * Dodaje do siebie dwa wielomiany spośród których p jest wielomianem stałym
 * a q jest wielomianem nie stałym
 * @param p : wielomian
 * @param q : wielomian
 * @return @f$p + q@f$
 */
static Poly AddPolyAndCoeff(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && !PolyIsCoeff(q));
    Poly r = {.size = q->size + 1, .arr = calloc((q->size + 1), sizeof(Mono))};
    CHECK_PTR(&r.arr);

    for (size_t i = 0; i < q->size; ++i) {
        r.arr[i] = MonoClone(&q->arr[i]);
    }

    if (!PolyIsZero(p)) {
        r.arr[q->size] = MonoFromPoly(p, 0);
    } else {
        r.size--;
        r.arr = realloc(r.arr, r.size * sizeof(Mono));
        CHECK_PTR(&r.arr);
        return r;
    }

    if (r.arr[0].exp == r.arr[q->size].exp) {
        Poly temp = r.arr[0].p;
        if (isAddResEqZero(&r.arr[0].p, &r.arr[q->size].p)) {
            for (size_t i = 0; i < r.size - 2; ++i) {
                Mono tempor = r.arr[i];
                r.arr[i] = MonoClone(&r.arr[i + 1]);
                MonoDestroy(&tempor);
            }
            r.size -= 2;
            r.arr = realloc(r.arr, r.size * sizeof(Mono));
            CHECK_PTR(&r.arr);
        } else {
            r.arr[0].p = PolyAdd(&r.arr[0].p, &r.arr[q->size].p);
            r.arr[0].exp = 0;
            r.size--;
            r.arr = realloc(r.arr, r.size * sizeof(Mono));
            CHECK_PTR(&r.arr);
        }
        PolyDestroy(&temp);
    } else PolySort(&r);

    return r;
}

/**
 * Łączy ze sobą 2 wielomiany dodając je w kolejności rosnącej do tablicy
 * wielomianu res, jeśli exp jednomianów w p i q są takie same to je dodaje
 * a wynik wpisuje do tablicy wielomianu res.
 * @param res : wielomian
 * @param p : wielomian
 * @param q : wielomian
 * @return : rozmiar tablicy wielomianu res
 */
static int merge2Polys(Poly *res, const Poly *p, const Poly *q) {
    size_t pInd = 0, qInd = 0, i = 0;

    while (pInd < p->size && qInd < q->size) {
        poly_exp_t p_exp = p->arr[pInd].exp, q_exp = q->arr[qInd].exp;

        if (p_exp < q_exp) {
            res->arr[i] = MonoClone(&p->arr[pInd]);
            pInd++;
            i++;
        } else if (p_exp > q_exp) {
            res->arr[i] = MonoClone(&q->arr[qInd]);
            qInd++;
            i++;
        } else {
            res->arr[i].p = PolyAdd(&p->arr[pInd].p, &q->arr[qInd].p);
            res->arr[i].exp = p_exp;
            i++;
            pInd++;
            qInd++;
        }
    }

    while (pInd < p->size) {
        res->arr[i] = MonoClone(&p->arr[pInd]);
        i++;
        pInd++;
    }

    while (qInd < q->size) {
        res->arr[i] = MonoClone(&q->arr[qInd]);
        i++;
        qInd++;
    }

    return i;
}

/**
 * Dodaje do siebie dwa wielomiany które nie są wielomianami stałymi
 * @param p : wielomian
 * @param q : wielomian
 * @return @f$p + q@f$
 */
static Poly Add2Polys(const Poly *p, const Poly *q) {
    Poly res;
    res.size = p->size + q->size;
    res.arr = calloc(res.size, sizeof(Mono));
    CHECK_PTR(&res.arr);

    size_t corrSize = merge2Polys(&res, p, q);

    if (corrSize < p->size + q->size) {
        if (corrSize == 0) { PolyDestroy(&res); return PolyZero(); }

        res.arr = realloc(res.arr, corrSize * sizeof(Mono));
        CHECK_PTR(&res.arr);
        res.size = corrSize;
        if (res.size == 1 && res.arr[0].exp == 0) {
            poly_coeff_t resCoeff = res.arr[0].p.coeff;
            PolyDestroy(&res);
            return PolyFromCoeff(resCoeff);
        }
    }

    for (size_t i = 0; i < res.size; ++i) {
        if (isPolyZeroRec(&res.arr[i].p)) {
            for (size_t j = i; j < res.size - 1; ++j) {
                Mono tempor = res.arr[j];
                res.arr[j] = MonoClone(&res.arr[j + 1]);
                MonoDestroy(&tempor);
            }
            res.size--;
        }
    }

    res.arr = realloc(res.arr, res.size * sizeof(Mono));
    CHECK_PTR(&res.arr);
    if (res.size == 0) return PolyZero();

    return res;
}


Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff(p->coeff + q->coeff);
    if (PolyIsCoeff(p)) return AddPolyAndCoeff(p, q);
    if (PolyIsCoeff(q)) return AddPolyAndCoeff(q, p);
    return Add2Polys(p, q);
}


bool isPolyZeroRec(const Poly *p) {
    if (PolyIsCoeff(p) && p->coeff == 0) return true;
    if (PolyIsCoeff(p) && p->coeff != 0) return false;

    for (size_t i = 0; i < p->size; ++i) {
        if (!isPolyZeroRec(&p->arr[i].p)) return false;
    }
    return true;
}


/**
 * Sprawdza czy wielomian jest stałą liczbą zapisaną w formie wielomianu
 * wykorzystując rekurencję
 * @param[in] p : wielomian
 * @return Czy wielomian @p jest współczynnikiem?
 */
bool isPolyCoeffRec(const Poly *p) {
    if (PolyIsCoeff(p)) return true;
    for (size_t i = 0; i < p->size; ++i) {
        if (!isPolyCoeffRec(&p->arr[i].p)) return false;
    }
    for (size_t i = 0; i < p->size; ++i) {
        if (p->arr[i].exp != 0) return false;
    }
    return true;
}

poly_coeff_t getCoeff(Poly *p) {
    assert(isPolyCoeffRec(p));
    if (PolyIsCoeff(p)) return p->coeff;
    return getCoeff(&p->arr[0].p);
}

/**
 * Tworzy płytką kopię tablicy wielomianow
 * @param[in] count : rozmiar tablicy
 * @param[in] monos : kopiowana tablica wielomianów
 * @param[in] monosCopy : skopiowana tablica wielomianów
 */
static void makeMonoCopy(size_t count, const Mono *monos, Mono *monosCopy) {
    for (size_t i = 0; i < count; ++i) {
        monosCopy[i] = monos[i];
    }
    SortMonos(count, monosCopy);
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {

    if (count == 0) return PolyZero();

    Poly p = {.size = count, .arr = calloc(count, sizeof(Mono))};
    CHECK_PTR(&p.arr);

    Mono *monosCopy = safeMalloc(count * sizeof(Mono));
    makeMonoCopy(count, monos, monosCopy);

    size_t index = 0;
    p.arr[0] = MonoClone(&monosCopy[0]);
    for (size_t i = 1; i < count; i++) {
        if (monosCopy[i - 1].exp == monosCopy[i].exp) {
            Poly temp = p.arr[index].p;
            p.arr[index].p = PolyAdd(&monosCopy[i].p, &temp);
            PolyDestroy(&temp);
        } else {
            if (isPolyZeroRec(&p.arr[index].p)) {
                MonoDestroy(&p.arr[index]);
            } else {
                index++;
            }
            p.arr[index] = MonoClone(&monosCopy[i]);
        }
    }

    for (size_t i = 0; i < count; ++i) {
        MonoDestroy(&monosCopy[i]);
    }
    free(monosCopy);

    if (isPolyZeroRec(&p)) { PolyDestroy(&p); return PolyZero(); }

    if (isPolyCoeffRec(&p)) {
        poly_coeff_t coeff = getCoeff(&p);
        PolyDestroy(&p);
        return PolyFromCoeff(coeff);
    }

    if (index + 1 != count) {
        p.size = index + 1;
        p.arr = realloc(p.arr, p.size * sizeof(Mono));
        CHECK_PTR(&p.arr);
    }

    return p;
}

/**
 * Mnoży przekazany wielomian m przez liczbę x
 * @param[in] m : jednomian
 * @param[in] x : liczba przez którą mnożymy
 */
static void MulMonoByNum(Mono *m, poly_coeff_t x) {
    if (PolyIsCoeff(&m->p)) {
        if ((x > 0 && m->p.coeff > 0) || (x < 0 && m->p.coeff < 0)) {
            if (x == 0 || m->p.coeff * x < 0) {
                m->p.coeff = 0;
            } else {
                m->p.coeff *= x;
            }
        } else {
            if (x == 0 || m->p.coeff * x > 0) {
                m->p.coeff = 0;
            } else {
                m->p.coeff *= x;
            }
        }
    } else {
        for (size_t i = 0; i < m->p.size; ++i) {
            MulMonoByNum(&m->p.arr[i], x);
        }
    }
}

/**
 * Zwraca wielomian @p pomnożony przez liczbę @num
 * @param p : wielomian
 * @param num  : liczba przez którą mnożymy wielomian
 * @return @f$p * num
 */
static Poly MulPolyByCoeff(const Poly *p, poly_coeff_t num) {
    Poly new = {.size = p->size, .arr = calloc(p->size, sizeof(Mono))};
    CHECK_PTR(&new.arr);
    for (size_t i = 0; i < p->size; ++i) {
        new.arr[i] = MonoClone(&p->arr[i]);

        MulMonoByNum(&new.arr[i], num);
    }

    if (isPolyZeroRec(&new)) {
        PolyDestroy(&new);
        return PolyZero();
    }

    return new;
}

void ExpandMonoArr(unsigned long int *monosSize, Mono **monosArr) {
    *monosSize *= 2;
    *monosArr = realloc(*monosArr, (*monosSize) * sizeof(Mono));
    CHECK_PTR(monosArr);
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (isPolyZeroRec(p) || isPolyZeroRec(q)) return PolyZero();

    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff((q->coeff) * (p->coeff));

    if (PolyIsCoeff(p)) return MulPolyByCoeff(q, p->coeff);

    if (PolyIsCoeff(q)) return MulPolyByCoeff(p, q->coeff);

    unsigned long int monosSize = INIT_MONOS_SIZE, k = 0;
    Mono *monos = safeMalloc(monosSize * sizeof(Mono));
    CHECK_PTR(monos);

    for (size_t i = 0; i < p->size; i++) {
        for (size_t j = 0; j < q->size; j++) {
            if (monosSize == k) ExpandMonoArr(&monosSize, &monos);

            monos[k].p = PolyMul(&p->arr[i].p, &q->arr[j].p);
            if (isPolyZeroRec(&monos[k].p)) {
                PolyDestroy(&monos[k].p);
                monos[k].p = PolyZero();
                monos[k].exp = 0;
            } else {
                monos[k].exp = p->arr[i].exp + q->arr[j].exp;
            }
            k++;
        }
    }
    Poly res = PolyAddMonos(k, monos);
    free(monos);
    return res;
}

void PolyNegHelp(Poly *p) {
    if (PolyIsCoeff(p)) {
        p->coeff = -(p->coeff);
        return;
    }

    for (size_t i = 0; i < p->size; ++i) {
        PolyNegHelp(&p->arr[i].p);
    }
}

Poly PolyNeg(const Poly *p) {
    if (p) {
        Poly cloned_p = PolyClone(p);
        PolyNegHelp(&cloned_p);
        return cloned_p;
    }
    return *p;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly neg_q = PolyNeg(q);
    Poly diff = PolyAdd(p, &neg_q);
    PolyDestroy(&neg_q);
    if (isPolyZeroRec(&diff)) { PolyDestroy(&diff); return PolyZero(); }
    return diff;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (isPolyCoeffRec(p)) {
        if (p->coeff == 0) return -1;
        return 0;
    }

    if (var_idx == 0) return p->arr[p->size - 1].exp;
    poly_exp_t max = 0;
    poly_exp_t curr;

    for (size_t i = 0; i < p->size; ++i) {
        curr = PolyDegBy(&(p->arr[i].p), var_idx - 1);
        if (curr > max) max = curr;
    }

    return max;
}

poly_exp_t MonoDeg(const Mono *m) {
    poly_exp_t deg = m->exp;
    poly_exp_t coeff_deg = PolyDeg(&m->p);

    if (coeff_deg == -1) {
        return 0;
    }

    return deg + coeff_deg;
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsCoeff(p)) {
        if (p->coeff == 0) return -1;

        return 0;
    }

    poly_exp_t max = 0;
    poly_exp_t curr;
    for (size_t i = 0; i < p->size; ++i) {
        curr = MonoDeg(&(p->arr[i]));
        if (curr > max) max = curr;
    }

    return max;
}

bool MonoIsEq(Mono *m, const Mono *n) {
    if (m->exp != n->exp) return false;

    return PolyIsEq(&m->p, &n->p);
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) return (p->coeff == q->coeff);
    if (PolyIsCoeff(p) || PolyIsCoeff(q)) return false;
    if (p->size != q->size) return false;

    for (size_t i = 0; i < p->size; ++i) {
        if (!MonoIsEq(&p->arr[i], &q->arr[i])) return false;
    }

    return true;
}

/**
 * Odpowiednik funkcji abs dla argumenty typu poly_coeff_t (long int)
 * @param[in] num : liczba na którą nakładamy wartość bezwględną
 * @return : wartość bezwględna @num
 */
static poly_coeff_t longAbs(poly_coeff_t num) {
    if (num > 0) return num;
    return -num;
}

/**
 * Zwraca base podniosioną do potęgi exp
 * @param[in] base : podstawa potęgi
 * @param[in] exp : wykładnik potęgi
 * @return base podniesiona do potęgi exp
 */
static poly_coeff_t ipow(poly_coeff_t base, poly_exp_t exp) {
    long long int result = 1;
    for (int i = 0; i < exp; ++i) {
        if (base == 0) {
            errno = ERANGE;
            return -1;
        }
        if ((LONG_MAX % longAbs(base) == 0) &&
            result > LONG_MAX / longAbs(base)) {
            errno = ERANGE;
            return -1;
        }
        if ((LONG_MAX % longAbs(base) != 0) &&
            result >= LONG_MAX / longAbs(base) + 1) {
            errno = ERANGE;
            return -1;
        }
        result *= base;
    }
    return result;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p)) return PolyFromCoeff(p->coeff);
    Mono new[p->size];
    Poly polys[p->size];

    for (size_t i = 0; i < p->size; ++i) {

        new[i] = MonoClone(&p->arr[i]);
        poly_coeff_t mulNum = ipow(x, new[i].exp);

        if (errno == ERANGE) {
            errno = 0;
            MonoDestroy(&new[i]);
            polys[i] = PolyZero();
        } else {

            MulMonoByNum(&new[i], mulNum);
            polys[i] = new[i].p;
        }
    }

    for (size_t i = 1; i < p->size; ++i) {
        if (!isPolyZeroRec(&polys[i])) {
            Poly temp = polys[0];
            polys[0] = PolyAdd(&polys[0], &polys[i]);
            PolyDestroy(&temp);
            PolyDestroy(&polys[i]);
        }
    }
    return polys[0];
}