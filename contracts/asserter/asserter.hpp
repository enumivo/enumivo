/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include <eosiolib/eosio.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      ENULIB_SERIALIZE( assertdef, (condition)(message) )
   };
}
