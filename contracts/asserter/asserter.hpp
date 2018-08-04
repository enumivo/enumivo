/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */

#include <enulib/enu.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      ENULIB_SERIALIZE( assertdef, (condition)(message) )
   };
}
