#include <vector>
#include <parallel/algorithm>
#include <omp.h>

void optimized_pre_phase1(size_t) {}

void optimized_post_phase1() {}

void optimized_pre_phase2(size_t) {}

void optimized_post_phase2() {}

struct Compare {
    bool operator()(const float& a, const float& b) const {
        return a < b;
    }
};

void optimized_do_phase1(float* data, size_t size) {
    // 使用 __gnu_parallel::sort 进行并行排序，需要提供合适的比较器
    __gnu_parallel::sort(data, data + size, Compare());
}

void optimized_do_phase2(size_t* result, float* data, float* query, size_t size) {
    std::vector<std::pair<float, size_t>> q(size);
    
    // 并行初始化查询数组
    #pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        q[i] = std::make_pair(query[i], i);
    }
    
    // 使用并行STL算法进行排序（如果可用），否则使用单线程排序
    __gnu_parallel::sort(q.begin(), q.end()); // 注意：需要OpenMP 4.5以上或第三方库支持并行
    
    // 并行搜索匹配位置
    #pragma omp parallel for
    for (size_t query_pos = 0; query_pos < size; ++query_pos) {
        float query_current = q[query_pos].first;
        size_t data_pos = 0;
        
        // 使用二分搜索找到第一个不小于 query_current 的位置
        auto lower = std::lower_bound(data, data + size, query_current);
        data_pos = lower - data;
        
        result[q[query_pos].second] = data_pos;
    }
}

// void optimized_do_phase2(size_t* result, float* data, float* query, size_t size) {
//     std::vector<std::pair<float, size_t> > q;
//     for (size_t i = 0; i < size; ++i) {
//         q.push_back(std::make_pair(query[i], i));
//     }
//     std::sort(q.begin(), q.end());

//     size_t data_pos = 0;
//     size_t query_pos = 0;
//     while (query_pos < size) {
//         float query_current = q[query_pos].first;
//         while (data[data_pos] < query_current) {
//             data_pos += 1;
//         }
//         result[q[query_pos].second] = data_pos;
//         query_pos += 1;
//     }
// }