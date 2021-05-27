//
// Created by patryk on 23.05.2021.
//

Poly PolyAddMonos(size_t count, const Mono monos[]) {

    if (count == 0) return PolyZero();

    Poly p = {.size = count, .arr = calloc(count, sizeof(Mono))};
    CHECK_PTR(&p.arr);

    Mono *monosCopy = malloc(count * sizeof(Mono));
    for (size_t i = 0; i < count; ++i) {
        monosCopy[i] = monos[i];
    }
    SortMonos(count, monosCopy);

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

    if (isPolyZeroRec(&p)) {
        PolyDestroy(&p);
        return PolyZero();
    }

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