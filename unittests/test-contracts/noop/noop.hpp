/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enulib/enu.hpp>

class [[enumivo::contract]] noop : public enumivo::contract {
public:
   using enumivo::contract::contract;

   [[enumivo::action]]
   void anyaction( enumivo::name                       from,
                   const enumivo::ignore<std::string>& type,
                   const enumivo::ignore<std::string>& data );
};
