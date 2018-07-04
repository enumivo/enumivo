#pragma once

#include <enulib/enu.hpp>

namespace enumivo {

   class sudo : public contract {
      public:
         sudo( account_name self ):contract(self){}

         void exec();

   };

} /// namespace enumivo
