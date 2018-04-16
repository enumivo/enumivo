/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include <enumivo/chain/contracts/genesis_state.hpp>

// these are required to serialize a genesis_state
#include <fc/smart_ref_impl.hpp>   // required for gcc in release mode

namespace enumivo { namespace chain { namespace contracts {


chain::chain_id_type genesis_state_type::compute_chain_id() const {
   return initial_chain_id;
}

} } } // namespace enumivo::chain::contracts
