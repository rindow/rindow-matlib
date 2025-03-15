#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class TopK
{
private:
    static void swap_float(T *a, T *b)
    {
        T tmp = *a;
        *a = *b;
        *b = tmp;
    }

    static void swap_int(int32_t *a, int32_t *b)
    {
        int32_t tmp = *a;
        *a = *b;
        *b = tmp;
    }

    static void minHeapify(int32_t size, T heap[],  int32_t indices[], int32_t parent) {
        int32_t left = 2 * parent + 1;
        int32_t right = 2 * parent + 2;

        while (left < size) {
            int32_t smallest;
            if (right < size && heap[right] < heap[left]) {
                smallest = right;
            } else {
                smallest = left;
            }

            if (heap[parent] <= heap[smallest]) {
                break;
            }
            swap_float(&heap[parent],&heap[smallest]);
            swap_int(&indices[parent],&indices[smallest]);

            parent = smallest;
            left = 2 * parent + 1;
            right = 2 * parent + 2;
        }
    }

    static void findTopNumbers(
        int32_t size,
        const T arr[],
        int32_t k,
        T topNumbers[],
        int32_t indices[],
        int32_t sorted)
    {
        // Build minimum heap with first TOP_NUM element

        for (int32_t i = 0; i < k; ++i) {
            topNumbers[i] = arr[i];
            indices[i] = i;
        }
        for (int32_t i = (k / 2) - 1; i >= 0; --i) {
            minHeapify(k, topNumbers, indices, i);
        }

        // Process remaining elements
        for (int32_t i = k; i < size; ++i) {
            if (arr[i] > topNumbers[0]) {
                topNumbers[0] = arr[i];
                indices[0] = i;
                minHeapify(k, topNumbers, indices, 0);
            }
        }

        if(sorted) {
            // sort
            for (int32_t i = k - 1; i > 0; --i) {
                swap_float(&topNumbers[0],&topNumbers[i]);
                swap_int(&indices[0],&indices[i]);
                minHeapify(i, topNumbers, indices, 0);
            }
        }
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t m,
        int32_t n,
        const T *input,
        int32_t k,
        int32_t sorted,
        T *values,
        int32_t *indices
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            findTopNumbers(n, &input[i*n], k, &values[i*k], &indices[i*k], sorted);
        }
    }

public:
    static void execute(
        int32_t m,
        int32_t n,
        const T *input,
        int32_t k,
        int32_t sorted,
        T *values,
        int32_t *indices
        )
    {
        if(m <= 0 || n <= 0 || k <= 0) {
            return;
        }
        if(k>n) {
            return;
        }

        ParallelOperation::execute(
            m,kernel,
            m,
            n,
            input,
            k,
            sorted,
            values,
            indices
        );
    }
};

}

extern "C" {
void rindow_matlib_s_topk(
    int32_t m,      // batch num
    int32_t n,      // source input data cols
    const float *input,// input 2-D Array with m * n.
    int32_t k,      // Number of top element to look for
    int32_t sorted, // Whether to sort and output search results
    float *values,  // K largest elements searched
    int32_t *indices // Indices on the source data where the element searched was
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    TopK<float>::execute(
        m,
        n,
        input,
        k,
        sorted,
        values,
        indices
    );
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_topk(
    int32_t m,      // batch num
    int32_t n,      // source input data cols
    const double *input,// input 2-D Array with m * n.
    int32_t k,      // Number of top element to look for
    int32_t sorted, // Whether to sort and output search results
    double *values,  // K largest elements searched
    int32_t *indices // Indices on the source data where the element searched was
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    TopK<double>::execute(
        m,
        n,
        input,
        k,
        sorted,
        values,
        indices
    );
    RINDOW_END_CLEAR_EXCEPTION;
}

}
