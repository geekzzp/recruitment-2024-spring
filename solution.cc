#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <algorithm>

void optimized_pre_phase1(size_t) {}

void optimized_post_phase1() {}

void optimized_pre_phase2(size_t) {}

void optimized_post_phase2() {}

static int cmp(void const* a, void const* b) {
    return *(float*)a < *(float*)b ? -1 : 1;
}

void optimized_do_phase1(float* data, size_t size) {
    qsort(data, size, sizeof(data[0]), cmp);
}

// void optimized_do_phase2(size_t* result, float* data, float* query, size_t size) {
//     for (size_t i = 0; i < size; ++i) {
//         size_t l = 0, r = size;
//         while (l < r) {
//             size_t m = l + (r - l) / 2;
//             if (data[m] < query[i]) {
//                 l = m + 1;
//             } else {
//                 r = m;
//             }
//         }
//         result[i] = l;
//     }
// }

void optimized_do_phase2(size_t* result, float* data, float* query, size_t size) {
    std::vector<std::pair<float, size_t> > q;
    for (size_t i = 0; i < size; ++i) {
        q.push_back(std::make_pair(query[i], i));
    }
    std::sort(q.begin(), q.end());

    size_t data_pos = 0;
    size_t query_pos = 0;
    while (query_pos < size) {
        float query_current = q[query_pos].first;
        while (data[data_pos] < query_current) {
            data_pos += 1;
        }
        result[q[query_pos].second] = data_pos;
        query_pos += 1;
    }
}