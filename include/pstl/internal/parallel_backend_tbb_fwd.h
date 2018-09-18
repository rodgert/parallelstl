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

#ifndef __PSTL_parallel_backend_tbb_fwd_H
#define __PSTL_parallel_backend_tbb_fwd_H

namespace __pstl {
namespace par_backend {

//! Evaluation of brick f[i,j) for each subrange [i,j) of [first,last)
template<class _Index, class _Fp>
void parallel_for(_Index, _Index, _Fp);

template<class _Value, class _Index, typename _RealBody, typename _Reduction>
_Value parallel_reduce(_Index, _Index, const _Value&, const _RealBody&, const _Reduction&);


template<class _Index, class _Up, class _Tp, class _Cp, class _Rp>
_Tp parallel_transform_reduce( _Index, _Index, _Up, _Cp, _Rp);

template<class _Index, class _Up, class _Tp, class _Cp, class _Rp, class _Sp>
_Tp parallel_transform_scan(_Index, _Up, _Tp, _Cp, _Rp, _Sp);

template<typename _Index, typename _Tp, typename _Rp, typename _Cp, typename _Sp, typename _Ap>
void parallel_strict_scan(_Index, _Tp, _Rp, _Cp, _Sp, _Ap);

template<typename _RandomAccessIterator, typename _Compare, typename _LeafSort>
void parallel_stable_sort(_RandomAccessIterator, _RandomAccessIterator, _Compare, _LeafSort);

template<typename _RandomAccessIterator1, typename _RandomAccessIterator2,
         typename _RandomAccessIterator3, typename _Compare, typename _LeafMerge>
void parallel_merge(_RandomAccessIterator1, _RandomAccessIterator1,
										_RandomAccessIterator2, _RandomAccessIterator2,
										_RandomAccessIterator3, _Compare, _LeafMerge);

template<typename _RandomAccessIterator, typename _Compare>
void parallel_partial_sort(_RandomAccessIterator, _RandomAccessIterator, _RandomAccessIterator,
                           _Compare);

struct backend_impl_t {
	 static cancel_execution();
};

} // namespace par_backend
} // namespace __pstl

#endif /* __PSTL_parallel_backend_tbb_fwd_H */
