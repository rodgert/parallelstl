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
#ifndef __PSTL_numeric_fwd_H
#define __PSTL_numeric_fwd_H

#include <type_traits>
#include <utility>

namespace __pstl {
namespace internal {

template<class _ForwardIterator1, class _ForwardIterator2, class _Tp, class _BinaryOperation1, class _BinaryOperation2>
_Tp brick_transform_reduce(_ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _Tp,
                           _BinaryOperation1, _BinaryOperation2,
                           /*__is_vector=*/std::true_type) noexcept;

template<class _ForwardIterator1, class _ForwardIterator2, class _Tp, class _BinaryOperation1, class _BinaryOperation2>
_Tp brick_transform_reduce(_ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _Tp,
                           _BinaryOperation1, _BinaryOperation2,
                           /*__is_vector=*/std::false_type) noexcept;

template<class _ForwardIterator1, class _ForwardIterator2, class _Tp, class _BinaryOperation1, class _BinaryOperation2, class _IsVector>
_Tp pattern_transform_reduce(_ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _Tp,
                             _BinaryOperation1, _BinaryOperation2, _IsVector,
                             /*is_parallel=*/std::false_type) noexcept;

template<class _RandomAccessIterator1, class _RandomAccessIterator2, class _Tp, class _BinaryOperation1,
         class _BinaryOperation2, class _IsVector>
  _Tp pattern_transform_reduce(_RandomAccessIterator1, _RandomAccessIterator1, _RandomAccessIterator2,
                               _Tp, _BinaryOperation1, _BinaryOperation2,
                               _IsVector __is_vector,  /*is_parallel=*/std::true_type) noexcept;

template< class _ForwardIterator, class _Tp, class _UnaryOperation, class _BinaryOperation >
_Tp brick_transform_reduce(_ForwardIterator, _ForwardIterator, _Tp, _BinaryOperation, _UnaryOperation,
                           /*is_vector=*/std::true_type) noexcept;

template< class _ForwardIterator, class _Tp, class _BinaryOperation, class _UnaryOperation >
_Tp brick_transform_reduce(_ForwardIterator, _ForwardIterator, _Tp, _BinaryOperation, _UnaryOperation,
                           /*is_vector=*/std::false_type) noexcept;
template<class _ForwardIterator, class _Tp, class _BinaryOperation, class _UnaryOperation, class _IsVector>
_Tp pattern_transform_reduce(_ForwardIterator, _ForwardIterator, _Tp, _BinaryOperation,
                             _UnaryOperation,
                             _IsVector, /*is_parallel=*/std::false_type ) noexcept;

template<class _ForwardIterator, class _Tp, class _BinaryOperation, class _UnaryOperation, class _IsVector>
_Tp pattern_transform_reduce(_ForwardIterator, _ForwardIterator, _Tp, _BinaryOperation,
                             _UnaryOperation,
                             _IsVector, /*is_parallel=*/std::true_type);

template<class _ForwardIterator, class _OutputIterator, class _UnaryOperation, class _Tp, class _BinaryOperation>
std::pair<_OutputIterator,_Tp>
brick_transform_scan(_ForwardIterator, _ForwardIterator, _OutputIterator,
                     _UnaryOperation, _Tp, _BinaryOperation,
                     /*Inclusive*/ std::false_type) noexcept;

template<class _ForwardIterator, class _OutputIterator, class _UnaryOperation, class _Tp, class _BinaryOperation>
std::pair<_OutputIterator,_Tp>
brick_transform_scan(_ForwardIterator, _ForwardIterator, _OutputIterator,
                     _UnaryOperation, _Tp, _BinaryOperation,
                     /*Inclusive*/std::true_type) noexcept;

template<class _ForwardIterator, class _OutputIterator, class _UnaryOperation, class _Tp,
         class _BinaryOperation, class _Inclusive, class _IsVector>

_OutputIterator pattern_transform_scan(_ForwardIterator, _ForwardIterator, _OutputIterator,
                                       _UnaryOperation, _Tp, _BinaryOperation, _Inclusive,
                                       _IsVector, /*is_parallel=*/std::false_type ) noexcept;

template<class _RandomAccessIterator, class _OutputIterator, class _UnaryOperation, class _Tp,
         class _BinaryOperation, class _Inclusive, class _IsVector>
_OutputIterator pattern_transform_scan(_RandomAccessIterator, _RandomAccessIterator, _OutputIterator,
                                       _UnaryOperation, _Tp, _BinaryOperation, _Inclusive,
                                       _IsVector, /*is_parallel=*/std::true_type );

template<class _ForwardIterator, class _OutputIterator, class _BinaryOperation>
_OutputIterator brick_adjacent_difference(_ForwardIterator, _ForwardIterator, _OutputIterator,
                                          _BinaryOperation,
                                          /*is_vector*/ std::false_type) noexcept;

template<class _ForwardIterator, class _OutputIterator, class _BinaryOperation>
_OutputIterator brick_adjacent_difference(_ForwardIterator, _ForwardIterator, _OutputIterator,
                                          _BinaryOperation,
                                          /*is_vector*/ std::true_type) noexcept;

template<class _ForwardIterator, class _OutputIterator, class _BinaryOperation, class _IsVector>
_OutputIterator pattern_adjacent_difference(_ForwardIterator, _ForwardIterator, _OutputIterator,
                                            _BinaryOperation,
                                            _IsVector, /*is_parallel*/ std::false_type) noexcept;

template<class _ForwardIterator, class _OutputIterator, class _BinaryOperation, class _IsVector>
_OutputIterator pattern_adjacent_difference(_ForwardIterator, _ForwardIterator, _OutputIterator,
                                            _BinaryOperation,
                                            _IsVector, /*is_parallel*/ std::true_type) noexcept;

} // namespace internal
} // namespace __pstl
#endif /* __PSTL_numeric_fwd_H */
