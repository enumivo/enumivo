/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#include "noop.hpp"

using namespace enumivo;

void noop::anyaction( name                       from,
                      const ignore<std::string>& type,
                      const ignore<std::string>& data )
{
   require_auth( from );
}
