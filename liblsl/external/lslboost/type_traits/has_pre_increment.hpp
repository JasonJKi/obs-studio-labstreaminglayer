//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.lslboost.org/LICENSE_1_0.txt).
//
//  See http://www.lslboost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_PRE_INCREMENT_HPP_INCLUDED
#define BOOST_TT_HAS_PRE_INCREMENT_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_pre_increment
#define BOOST_TT_TRAIT_OP ++
#define BOOST_TT_FORBIDDEN_IF\
   ::lslboost::type_traits::ice_or<\
      /* bool */\
      ::lslboost::is_same< bool, Rhs_nocv >::value,\
      /* void* */\
      ::lslboost::type_traits::ice_and<\
         ::lslboost::is_pointer< Rhs_noref >::value,\
         ::lslboost::is_void< Rhs_noptr >::value\
      >::value,\
      /* (fundamental or pointer) and const */\
      ::lslboost::type_traits::ice_and<\
         ::lslboost::type_traits::ice_or<\
            ::lslboost::is_fundamental< Rhs_nocv >::value,\
            ::lslboost::is_pointer< Rhs_noref >::value\
         >::value,\
         ::lslboost::is_const< Rhs_noref >::value\
      >::value\
   >::value


#include <lslboost/type_traits/detail/has_prefix_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_FORBIDDEN_IF

#endif
