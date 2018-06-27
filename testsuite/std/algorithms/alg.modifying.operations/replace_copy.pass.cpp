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

// Tests for replace_copy and replace_copy_if

// Tests for copy_if and remove_copy_if
#include "support/pstl_test_config.h"

#include <type_traits>
#ifdef PSTL_STANDALONE_TESTS
#include "pstl/execution"
#include "pstl/algorithm"
#else
#include <execution>
#include <algorithm>
#endif // PSTL_STANDALONE_TESTS

#include "support/parallel_utils.h"

using namespace Parallel_TestUtils;

struct test_replace_copy {
    template<typename Policy, typename InputIterator, typename OutputIterator, typename OutputIterator2, typename Size, typename Predicate, typename T>
    void operator()(Policy&& exec, InputIterator first, InputIterator last, OutputIterator out_first, OutputIterator out_last,
        OutputIterator2 expected_first, OutputIterator2 expected_last, Size n, Predicate pred, const T& old_value, const T& new_value, T trash) {
        // Cleaning
        std::fill_n(expected_first, n, trash);
        std::fill_n(out_first, n, trash);
        // Run replace_copy
        auto i = std::replace_copy(first, last, expected_first, old_value, new_value);
        auto k = std::replace_copy(exec, first, last, out_first, old_value, new_value);
        EXPECT_EQ_N(expected_first, out_first, n, "wrong replace_copy effect");
        EXPECT_TRUE(out_last == k, "wrong return value from replace_copy");

        // Cleaning
        std::fill_n(expected_first, n, trash);
        std::fill_n(out_first, n, trash);
        // Run replace_copy_if
        i = replace_copy_if(first, last, expected_first, pred, new_value);
        k = replace_copy_if(exec, first, last, out_first, pred, new_value);
        EXPECT_EQ_N(expected_first, out_first, n, "wrong replace_copy_if effect");
        EXPECT_TRUE(out_last == k, "wrong return value from replace_copy_if");
    }
};

template<typename T, typename Convert, typename Predicate>
void test(T trash, const T& old_value, const T& new_value, Predicate pred, Convert convert) {
    // Try sequences of various lengths.
    for (size_t n = 0; n <= 100000; n = n <= 16 ? n + 1 : size_t(3.1415 * n) ) {
        Sequence<T> in(n, [&](size_t k) -> T {
            return convert(n^k);
        });
        Sequence<T> out(n, [=](size_t) {return trash; });
        Sequence<T> expected(n, [=](size_t) {return trash; });

        invoke_on_all_policies(test_replace_copy(), in.begin(), in.end(), out.begin(), out.end(), expected.begin(), expected.end(), out.size(), pred, old_value, new_value, trash);
        invoke_on_all_policies(test_replace_copy(), in.cbegin(), in.cend(), out.begin(), out.end(), expected.begin(), expected.end(), out.size(), pred, old_value, new_value, trash);
    }
}

int32_t main( ) {

    test<float64_t>( -666.0,
                 8.5,
                 0.33,
                 [](const float64_t& x) {return x*x <= 1024; },
                 [](size_t j){return ((j+1)%7&2)!=0? 8.5 : float64_t(j%32+j);});

    test<int32_t>( -666,
               42,
               99,
               [](const int32_t& x) {return x != 42; },
               [](size_t j){return ((j+1)%5&2)!=0? 42 : -1- int32_t(j);});

#if !__PSTL_ICC_17_TEST_MAC_RELEASE_32_BROKEN
    test<Number>( Number(42,OddTag()),
                  Number(2001,OddTag()),
                  Number(2017, OddTag()),
                  IsMultiple(3, OddTag()),
                  [](int32_t j){return ((j+1)%3&2)!=0? Number(2001,OddTag()) : Number(j,OddTag());});
#endif
    std::cout << "done" << std::endl;
    return 0;
}
