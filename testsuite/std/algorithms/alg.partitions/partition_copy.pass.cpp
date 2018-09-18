/*
    Copyright (c) 2017-2018 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.




*/

// Tests for stable_partition and partition_copy
// Tests for copy_if and remove_copy_if
#include "support/pstl_test_config.h"

#include <type_traits>
#include <cstdlib>
#include <iterator>
#ifdef PSTL_STANDALONE_TESTS
#include "pstl/execution"
#include "pstl/algorithm"
#else
#include <execution>
#include <algorithm>
#endif // PSTL_STANDALONE_TESTS

#include "support/parallel_utils.h"

using namespace Parallel_TestUtils;

struct test_partition_copy {
    template <typename Policy, typename InputIterator, typename OutputIterator, typename OutputIterator2, typename UnaryOp>
    void operator()(Policy&& exec, InputIterator first, InputIterator last, OutputIterator true_first, OutputIterator true_last,
        OutputIterator2 false_first, OutputIterator2 false_last, UnaryOp unary_op) {

        auto actual_ret = std::partition_copy(exec, first, last, true_first, false_first, unary_op);

        EXPECT_TRUE(std::distance(true_first, actual_ret.first) ==
            std::count_if(first, last, unary_op), "partition_copy has wrong effect from true sequence");
        EXPECT_TRUE(std::distance(false_first, actual_ret.second) ==
            std::count_if(first, last, not_pred<UnaryOp>(unary_op)), "partition_copy has wrong effect from false sequence");
    }

    //dummy specialization by iterator type and policy type, in case of broken configuration
#if __PSTL_ICC_1800_TEST_MONOTONIC_RELEASE_64_BROKEN
    template <typename InputIterator, typename OutputIterator, typename OutputIterator2, typename UnaryOp>
    void operator()(__pstl::execution::unsequenced_policy, std::reverse_iterator<InputIterator> first, std::reverse_iterator<InputIterator> last,
        std::reverse_iterator<OutputIterator> true_first, std::reverse_iterator<OutputIterator> true_last,
        std::reverse_iterator<OutputIterator2> false_first, OutputIterator2 false_last, UnaryOp unary_op) { }
    template <typename InputIterator, typename OutputIterator, typename OutputIterator2, typename UnaryOp>
    void operator()(__pstl::execution::parallel_unsequenced_policy, std::reverse_iterator<InputIterator> first, std::reverse_iterator<InputIterator> last,
        std::reverse_iterator<OutputIterator> true_first, std::reverse_iterator<OutputIterator> true_last,
        std::reverse_iterator<OutputIterator2> false_first, OutputIterator2 false_last, UnaryOp unary_op) { }
#endif
};

template <typename T, typename UnaryPred>
void test( UnaryPred pred) {

    const std::size_t max_size = 100000;
    Sequence<T> in(max_size, [](std::size_t v)->T { return T(v); });
    Sequence<T> actual_true(max_size);
    Sequence<T> actual_false(max_size);
    for (std::size_t n = 0; n <= max_size; n = n <= 16 ? n + 1 : std::size_t(3.1415 * n)) {

        // for non-const input iterators
        invoke_on_all_policies(test_partition_copy(), in.begin(), in.begin() + n,
            actual_true.begin(), actual_true.begin() + n, actual_false.begin(), actual_false.begin() + n, pred);

        // for const input iterators
        invoke_on_all_policies(test_partition_copy(), in.cbegin(), in.cbegin() + n,
            actual_true.begin(), actual_true.begin() + n, actual_false.begin(), actual_false.begin() + n, pred);
    }
}

int32_t main() {
    test<int32_t>([](const int32_t value) {return value % 2; });

#if !__PSTL_ICC_16_17_TEST_REDUCTION_RELEASE_BROKEN
    test<int32_t>([](const int32_t value) {return true; });
#endif

    test<float64_t>([](const float64_t value) {return value > 2 << 6; });
    test<Wrapper<float64_t>>([](const Wrapper<float64_t>& value) -> bool {return value.get_my_field() != nullptr; });

    return 0;
}
