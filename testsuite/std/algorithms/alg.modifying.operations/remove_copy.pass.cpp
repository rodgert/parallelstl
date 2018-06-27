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

// Tests for remove_copy

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

struct run_remove_copy {
    template<typename Policy, typename InputIterator, typename OutputIterator, typename OutputIterator2, typename Size, typename T>
    void operator()(Policy&& exec, InputIterator first, InputIterator last, OutputIterator out_first, OutputIterator out_last,
        OutputIterator2 expected_first, OutputIterator2 expected_last, Size n, const T& value, T trash) {
        // Cleaning
        std::fill_n(expected_first, n, trash);
        std::fill_n(out_first, n, trash);

        // Run copy_if
        auto i = remove_copy(first, last, expected_first, value);
        auto k = remove_copy(exec, first, last, out_first, value);
        EXPECT_EQ_N(expected_first, out_first, n, "wrong remove_copy effect");
        for (size_t j = 0; j < GuardSize; ++j) {
            ++k;
        }
        EXPECT_TRUE(out_last == k, "wrong return value from remove_copy");
    }
};

template<typename T, typename Convert>
void test(T trash, const T& value, Convert convert, bool check_weakness = true) {
    // Try sequences of various lengths.
    for (size_t n = 0; n <= 100000; n = n <= 16 ? n + 1 : size_t(3.1415 * n) ) {
        // count is number of output elements, plus a handful
        // more for sake of detecting buffer overruns.
        size_t count = GuardSize;
        Sequence<T> in(n, [&](size_t k) -> T {
            T x = convert(n^k);
            count += !(x==value) ? 1 : 0;
            return x;
        });
        using namespace std;

        Sequence<T> out(count, [=](size_t){return trash;});
        Sequence<T> expected(count, [=](size_t){return trash;});
        if (check_weakness) {
            auto expected_result = remove_copy(in.cfbegin(), in.cfend(), expected.begin(), value);
            size_t m = expected_result - expected.begin();
            EXPECT_TRUE(n / 4 <= m && m <= 3 * (n + 1) / 4, "weak test for remove_copy");
        }
        invoke_on_all_policies(run_remove_copy(), in.begin(), in.end(), out.begin(), out.end(), expected.begin(), expected.end(), count, value, trash);
        invoke_on_all_policies(run_remove_copy(), in.cbegin(), in.cend(), out.begin(), out.end(), expected.begin(), expected.end(), count, value, trash);
    }
}

int32_t main( ) {

    test<float64_t>( -666.0,
                 8.5,
                 [](size_t j){return ((j+1)%7&2)!=0? 8.5 : float64_t(j%32+j);});

    test<int32_t>( -666,
               42,
               [](size_t j){return ((j+1)%5&2)!=0? 42 : -1- int32_t(j);});

    test<Number>( Number(42,OddTag()),
                  Number(2001,OddTag()),
                  [](int32_t j){return ((j+1)%3&2)!=0? Number(2001,OddTag()) : Number(j,OddTag());});
    std::cout << "done" << std::endl;
    return 0;
}
