/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enulib/enu.hpp>

class [[enumivo::contract]] payloadless : public enumivo::contract {
public:
   using enumivo::contract::contract;

   [[enumivo::action]]
   void doit();
};
