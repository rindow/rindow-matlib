#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_GET_TOP_K(data_type, pair_type, i, a, ldA, n, k, top_values, top_indices, compare_func) \
    pair_type *temp = (pair_type *)malloc(n * sizeof(pair_type)); \
    /* Initialize temp with all elements and their indices */ \
    for (int j = 0; j < n; j++) { \
        temp[j].value = a[i * ldA + j]; \
        temp[j].index = j; \
    } \
    /* Sort temp array based on values in descending order */ \
    qsort(temp, n, sizeof(pair_type), compare_func); \
    /* Select the top K items */ \
    for (int j = 0; j < k; j++) { \
        top_values[i * k + j] = temp[j].value; \
        top_indices[i * k + j] = temp[j].index; \
    } \
    /* Free the allocated memory */ \
    free(temp);

#define RINDOW_MATLIB_SORT_TOP_K(data_type, pair_type, i, k, top_values, top_indices, compare_func) \
    pair_type *temp = (pair_type *)malloc(k * sizeof(pair_type)); \
    /* Initialize temp with top K values and indices */ \
    for (int j = 0; j < k; j++) { \
        temp[j].value = top_values[i * k + j]; \
        temp[j].index = top_indices[i * k + j]; \
    } \
    /* Sort temp array based on values in descending order */ \
    qsort(temp, k, sizeof(pair_type), compare_func); \
    /* Copy back the sorted values and indices */ \
    for (int j = 0; j < k; j++) { \
        top_values[i * k + j] = temp[j].value; \
        top_indices[i * k + j] = temp[j].index; \
    } \
    /* Free the allocated memory */ \
    free(temp);

typedef struct {
    float value;
    int32_t index;
} FloatIndexPair;

typedef struct {
    double value;
    int32_t index;
} DoubleIndexPair;

int compare_float(const void *a, const void *b) {
    const FloatIndexPair *pairA = (const FloatIndexPair *)a;
    const FloatIndexPair *pairB = (const FloatIndexPair *)b;
    return (pairB->value > pairA->value) - (pairB->value < pairA->value);
}

int compare_double(const void *a, const void *b) {
    const DoubleIndexPair *pairA = (const DoubleIndexPair *)a;
    const DoubleIndexPair *pairB = (const DoubleIndexPair *)b;
    return (pairB->value > pairA->value) - (pairB->value < pairA->value);
}


void rindow_matlib_s_top_k(
    int32_t m, // Number of rows or vectors
    int32_t n, // Number of columns or elements in each vector
    float *a, // Input data (array of vectors)
    int32_t ldA, // Leading dimension of the input (usually equal to n)
    int32_t k, // Number of top elements to find
    int32_t sorted, // Whether to return sorted values and indices
    float *top_values, // Output array for the top K values
    int32_t *top_indices // Output array for the indices of the top K values
)
{
    // Calculate top K for each row using RINDOW_MATLIB_GET_TOP_K macro
    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        RINDOW_MATLIB_GET_TOP_K(float, FloatIndexPair, i, a, ldA, n, k, top_values, top_indices, compare_float);
    }

    // If sorting is required, sort the top K using RINDOW_MATLIB_SORT_TOP_K macro
    if (sorted) {
        #pragma omp parallel for
        for (int i = 0; i < m; i++) {
            RINDOW_MATLIB_SORT_TOP_K(float, FloatIndexPair, i, k, top_values, top_indices, compare_float);
        }
    }
}

void rindow_matlib_d_top_k(
    int32_t m, // Number of rows or vectors
    int32_t n, // Number of columns or elements in each vector
    double *a, // Input data (array of vectors)
    int32_t ldA, // Leading dimension of the input (usually equal to n)
    int32_t k, // Number of top elements to find
    int32_t sorted, // Whether to return sorted values and indices
    double *top_values, // Output array for the top K values
    int32_t *top_indices // Output array for the indices of the top K values
)
{
    // Calculate top K for each row using RINDOW_MATLIB_GET_TOP_K macro
    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        RINDOW_MATLIB_GET_TOP_K(double, DoubleIndexPair, i, a, ldA, n, k, top_values, top_indices, compare_double);
    }

    // If sorting is required, sort the top K using RINDOW_MATLIB_SORT_TOP_K macro
    if (sorted) {
        #pragma omp parallel for
        for (int i = 0; i < m; i++) {
            RINDOW_MATLIB_SORT_TOP_K(double, DoubleIndexPair, i, k, top_values, top_indices, compare_double);
        }
    }
}


