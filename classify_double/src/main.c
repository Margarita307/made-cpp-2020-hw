#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit(const uint64_t number, const uint8_t index) {
    return (number & ((uint64_t)1 << index)) != 0;
}


/**
 * Checkers here:
 */

bool checkForPlusZero(uint64_t number) {
    return number == 0;
}

bool checkForMinusZero(uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf(uint64_t number) {
    uint64_t mask = ~(uint64_t)0;
    mask <<= 52;
    mask &= ~0x8000000000000000;
    return number == mask;
}

bool checkForMinusInf(uint64_t number) {
    uint64_t mask = ~(uint64_t)0;
    mask <<= 52;
    return number == mask;
}

bool checkForPlusNormal(uint64_t number) {
    uint64_t mask = ~(uint64_t)0;
    mask <<= 52;
    mask &= ~0x8000000000000000;
    return getBit(number, 63) == 0 && ((number & mask) != 0) && ((number & mask) != mask);
}

bool checkForMinusNormal(uint64_t number) {
    return getBit(number, 63) && checkForPlusNormal(number & (~0x8000000000000000));
}

bool checkForPlusDenormal(uint64_t number) {
    return number != 0 && ((number >> 52) == 0);
}

bool checkForMinusDenormal(uint64_t number) {
    return getBit(number, 63) && checkForPlusDenormal(number & (~0x8000000000000000));
}

bool checkForSignalingNan(uint64_t number) {
    uint64_t mask = ~0;
    mask <<= 52;
    number |= 0x8000000000000000;
    return (number & mask) == mask && getBit(number, 51) == 0 && ((number << 12) != 0);
}

bool checkForQuietNan(uint64_t number) {
    uint64_t mask = ~0;
    mask <<= 51;
    number |= 0x8000000000000000;
    return (number & mask) == mask;
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
