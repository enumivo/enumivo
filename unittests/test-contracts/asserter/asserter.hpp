/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enumivo/enu.hpp>

class [[enumivo::contract]] asserter : public enumivo::contract {
public:
   using enumivo::contract::contract;

   [[enumivo::action]]
   void procassert( int8_t condition, std::string message );

   [[enumivo::action]]
   void provereset();
};
