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

// This class is needed to check the self-copying
struct copy_int {
    int32_t value;
    int32_t copied_times = 0;
    explicit copy_int(int32_t val = 0) {
        value = val;
    }

    copy_int& operator=(const copy_int& other) {
        if (&other == this)
            copied_times++;
        else {
            value = other.value;
            copied_times = other.copied_times;
        }
        return *this;
    }

    bool operator==(const copy_int& other) const {
        return (value == other.value);
    }
};

template <typename Iterator>
struct test_one_policy {
    std::size_t len;
    Iterator data_b;
    Iterator data_e;
    test_one_policy(Iterator data_, std::size_t len_) {
        len = len_;
        data_b = data_;
        data_e = std::next(data_b, len);
    }
    template <typename ExecutionPolicy, typename Iterator1, typename Iterator2, typename T, typename Predicate>
    void operator()(ExecutionPolicy&& exec, Iterator1 expected_b, Iterator1 expected_e, Iterator2 actual_b, Iterator2 actual_e, Predicate pred, const T& value, const T& old_value) {
        using namespace std;

        copy(data_b, data_e, expected_b);
        copy(data_b, data_e, actual_b);

        replace(expected_b, expected_e, old_value, value);
        replace(exec, actual_b, actual_e, old_value, value);

        EXPECT_TRUE((check<T, Iterator2>(actual_b, actual_e)), "wrong result of self assignment check");
        EXPECT_TRUE(equal(expected_b, expected_e, actual_b), "wrong result of replace");

        copy(data_b, data_e, expected_b);
        copy(data_b, data_e, actual_b);

        replace_if(expected_b, expected_e, pred, value);
        replace_if(exec, actual_b, actual_e, pred, value);
        EXPECT_TRUE(equal(expected_b, expected_e, actual_b), "wrong result of replace_if");
    }

    template<typename T, typename Iterator1>
    bool check(Iterator1 b, Iterator1 e) {
        return true;
    }

    template<typename T, typename Iterator1>
    typename std::enable_if<std::is_same<T, copy_int>::value, bool>::type_t check(Iterator1 b, Iterator1 e) {
        return std::all_of(b, e, [](const copy_int& elem) { return elem.copied_times == 0; });
    }
};

template <typename T1, typename T2, typename Pred>
void test(Pred pred) {
    typedef typename Sequence<T2>::iterator iterator_type;

    const std::size_t max_len = 100000;

    const T1 value = T1(0);
    const T1 new_value = T1(666);

    Sequence<T2> expected(max_len);
    Sequence<T2> actual(max_len);

    Sequence<T2> data(max_len, [&value](std::size_t i) {
        if (i % 3 == 2) {
            return T1(i);
        } else {
            return value;
        }
    });

    for (std::size_t len = 0; len < max_len; len = len <= 16 ? len + 1 : std::size_t(3.1415 * len)) {
        test_one_policy<iterator_type> temp(data.begin(), len);

        invoke_on_all_policies(temp,
            expected.begin(), expected.begin() + len,
            actual.begin(), actual.begin() + len,
            pred, new_value, value);
    }
}

int32_t main() {
    test<int32_t, float32_t>  (equal_value<int32_t>(666));
    test<uint16_t, uint8_t> ([](const uint16_t& elem) { return elem % 3 < 2; });
    test<float64_t, int64_t>([](const float64_t& elem) { return elem * elem - 3.5 * elem > 10; });
    test<copy_int, copy_int> ([](const copy_int& val) { return val.value / 5 > 2; });

    std::cout << done() << std::endl;
    return 0;
}
