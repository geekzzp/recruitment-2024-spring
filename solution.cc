#include <vector>
// #include <immintrin.h>
#include <parallel/algorithm>
#include <omp.h>

void optimized_pre_phase1(size_t) {}

void optimized_post_phase1() {}

void optimized_pre_phase2(size_t) {}

void optimized_post_phase2() {}

struct Compare {
    inline bool operator()(const float& a, const float& b) const {
        return a < b;
    }
};

void optimized_do_phase1(float* data, const size_t size) {
    // 使用 __gnu_parallel::sort 进行并行排序，需要提供合适的比较器
    __gnu_parallel::sort(data, data + size, Compare());
}

void optimized_do_phase2(size_t* result, float* data, float* query, size_t size) {
    //利用缓存
    #pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        // 使用 std::lower_bound 来寻找不小于 query[i] 的第一个元素的位置
        auto it = std::lower_bound(data, data + size, query[i]);
        // 由于 it 是指向元素的迭代器，我们需要用 it - data 来获取索引
        result[i] = it - data;
    }
}
