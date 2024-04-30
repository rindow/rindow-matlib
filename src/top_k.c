#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

typedef struct {
    float value;
    int32_t index;
} FloatIndexPair;

typedef struct {
    double value;
    int32_t index;
} DoubleIndexPair;

#define RINDOW_MATLIB_MIN_HEAPIFY(pair_type, heap, heap_size, i) \
    int smallest = i; \
    int left = 2 * i + 1; \
    int right = 2 * i + 2; \
    while (left < heap_size) { \
        if (right < heap_size && heap[right].value < heap[left].value) { \
            smallest = right; \
        } else { \
            smallest = left; \
        } \
        if (heap[smallest].value >= heap[i].value) { \
            break; \
        } \
        pair_type temp = heap[i]; \
        heap[i] = heap[smallest]; \
        heap[smallest] = temp; \
        i = smallest; \
        left = 2 * i + 1; \
        right = 2 * i + 2; \
    }

#define RINDOW_MATLIB_GET_TOP_K_TEMPLATE(data_type, pair_type, temp, i, a, ldA, n, k, top_values, top_indices) \
    /* Initialize temp with first k elements */ \
    for (int j = 0; j < k; j++) { \
        temp[j].value = a[i * ldA + j]; \
        temp[j].index = j; \
    } \
    /* Build min-heap */ \
    for (int j = k / 2 - 1; j >= 0; j--) { \
        RINDOW_MATLIB_MIN_HEAPIFY(pair_type, temp, k, j); \
    } \
    /* Iterate through the rest of the elements in the row */ \
    for (int j = k; j < n; j++) { \
        data_type current_value = a[i * ldA + j]; \
        if (current_value > temp[0].value) { \
            temp[0].value = current_value; \
            temp[0].index = j; \
            int32_t idx = 0; \
            RINDOW_MATLIB_MIN_HEAPIFY(pair_type, temp, k, idx); \
        } \
    } \
    if(sorted) { \
        /* sort */ \
        for (int j = k - 1; j > 0; j--) { \
            /* swap */ \
            pair_type temp_pair = temp[j]; \
            temp[j] = temp[0]; \
            temp[0] = temp_pair; \
            int32_t idx = 0; \
            RINDOW_MATLIB_MIN_HEAPIFY(pair_type, temp, j, idx); \
        } \
    } \
    /* Extract top K values and indices from the heap */ \
    for (int j = 0; j < k; j++) { \
        top_values[i * k + j] = temp[j].value; \
        top_indices[i * k + j] = temp[j].index; \
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
    FloatIndexPair *heap = (FloatIndexPair *)malloc(k * sizeof(FloatIndexPair));

    int32_t i;
    #pragma omp parallel for
    for (i = 0; i < m; i++) {
        RINDOW_MATLIB_GET_TOP_K_TEMPLATE(float, FloatIndexPair, heap, i, a, ldA, n, k, top_values, top_indices);
    }

    free(heap);
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
    DoubleIndexPair *heap = (DoubleIndexPair *)malloc(k * sizeof(DoubleIndexPair));

    int32_t i;
    #pragma omp parallel for
    for (i = 0; i < m; i++) {
        RINDOW_MATLIB_GET_TOP_K_TEMPLATE(double, DoubleIndexPair, heap, i, a, ldA, n, k, top_values, top_indices);
    }

    free(heap);
}
