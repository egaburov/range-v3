/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.
//
// Algorithms from
// Elements of Programming
// by Alexander Stepanov and Paul McJones
// Addison-Wesley Professional, 2009
#ifndef RANGES_V3_ALGORITHM_MERGE_MOVE_HPP
#define RANGES_V3_ALGORITHM_MERGE_MOVE_HPP

#include <tuple>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/algorithm/move.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/utility/tagged_tuple.hpp>
#include <range/v3/algorithm/tagspec.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct merge_move_fn
        {
            template<typename I0, typename S0, typename I1, typename S1, typename O,
                typename C = ordered_less, typename P0 = ident, typename P1 = ident,
                CONCEPT_REQUIRES_(
                    IteratorRange<I0, S0>() &&
                    IteratorRange<I1, S1>() &&
                    MoveMergeable<I0, I1, O, C, P0, P1>()
                )>
            tagged_tuple<tag::in1(I0), tag::in2(I1), tag::out(O)>
            operator()(I0 begin0, S0 end0, I1 begin1, S1 end1, O out, C pred_ = C{},
                P0 proj0_ = P0{}, P1 proj1_ = P1{}) const
            {
                auto &&pred = as_function(pred_);
                auto &&proj0 = as_function(proj0_);
                auto &&proj1 = as_function(proj1_);
                for(; begin0 != end0 && begin1 != end1; ++out)
                {
                    if(pred(proj1(*begin1), proj0(*begin0)))
                    {
                        *out = iter_move(begin1);
                        ++begin1;
                    }
                    else
                    {
                        *out = iter_move(begin0);
                        ++begin0;
                    }
                }
                auto t0 = move(begin0, end0, out);
                auto t1 = move(begin1, end1, t0.second);
                return tagged_tuple<tag::in1(I0), tag::in2(I1), tag::out(O)>{t0.first, t1.first, t1.second};
            }

            template<typename Rng0, typename Rng1, typename O, typename C = ordered_less,
                typename P0 = ident, typename P1 = ident,
                typename I0 = range_iterator_t<Rng0>,
                typename I1 = range_iterator_t<Rng1>,
                CONCEPT_REQUIRES_(
                    Range<Rng0>() &&
                    Range<Rng1>() &&
                    MoveMergeable<I0, I1, O, C, P0, P1>()
                )>
            tagged_tuple<tag::in1(range_safe_iterator_t<Rng0>), tag::in2(range_safe_iterator_t<Rng1>), tag::out(O)>
            operator()(Rng0 &&rng0, Rng1 &&rng1, O out, C pred = C{}, P0 proj0 = P0{},
                P1 proj1 = P1{}) const
            {
                return (*this)(begin(rng0), end(rng0), begin(rng1), end(rng1), std::move(out),
                    std::move(pred), std::move(proj0), std::move(proj1));
            }
        };

        /// \sa `merge_move_fn`
        /// \ingroup group-algorithms
        namespace
        {
            constexpr auto&& merge_move = static_const<with_braced_init_args<merge_move_fn>>::value;
        }

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
