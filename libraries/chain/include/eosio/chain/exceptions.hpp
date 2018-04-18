/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <fc/exception/exception.hpp>
#include <enumivo/chain/protocol.hpp>
#include <enumivo/utilities/exception_macros.hpp>

namespace enumivo { namespace chain {

   FC_DECLARE_EXCEPTION( chain_exception, 3000000, "blockchain exception" )
   FC_DECLARE_DERIVED_EXCEPTION( database_query_exception,          enumivo::chain::chain_exception, 3010000, "database query exception" )
   FC_DECLARE_DERIVED_EXCEPTION( block_validate_exception,          enumivo::chain::chain_exception, 3020000, "block validation exception" )
   FC_DECLARE_DERIVED_EXCEPTION( transaction_exception,             enumivo::chain::chain_exception, 3030000, "transaction validation exception" )
   FC_DECLARE_DERIVED_EXCEPTION( action_validate_exception,         enumivo::chain::chain_exception, 3040000, "message validation exception" )
   FC_DECLARE_DERIVED_EXCEPTION( utility_exception,                 enumivo::chain::chain_exception, 3070000, "utility method exception" )
   FC_DECLARE_DERIVED_EXCEPTION( undo_database_exception,           enumivo::chain::chain_exception, 3080000, "undo database exception" )
   FC_DECLARE_DERIVED_EXCEPTION( unlinkable_block_exception,        enumivo::chain::chain_exception, 3090000, "unlinkable block" )
   FC_DECLARE_DERIVED_EXCEPTION( black_swan_exception,              enumivo::chain::chain_exception, 3100000, "black swan" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_block_exception,           enumivo::chain::chain_exception, 3110000, "unknown block" )
   FC_DECLARE_DERIVED_EXCEPTION( chain_type_exception,              enumivo::chain::chain_exception, 3120000, "chain type exception" )
   FC_DECLARE_DERIVED_EXCEPTION( missing_plugin_exception,          enumivo::chain::chain_exception, 3130000, "missing plugin exception" )
   FC_DECLARE_DERIVED_EXCEPTION( wallet_exception,                  enumivo::chain::chain_exception, 3140000, "wallet exception" )
   FC_DECLARE_DERIVED_EXCEPTION( rate_limiting_invariant_exception, enumivo::chain::chain_exception, 3150000, "rate limiting invariant violated" )

   FC_DECLARE_DERIVED_EXCEPTION( permission_query_exception,        enumivo::chain::database_query_exception, 3010001, "Permission Query Exception" )
   FC_DECLARE_DERIVED_EXCEPTION( account_query_exception,           enumivo::chain::database_query_exception, 3010002, "Account Query Exception" )
   FC_DECLARE_DERIVED_EXCEPTION( contract_table_query_exception,    enumivo::chain::database_query_exception, 3010003, "Contract Table Query Exception" )
   FC_DECLARE_DERIVED_EXCEPTION( contract_query_exception,          enumivo::chain::database_query_exception, 3010004, "Contract Query Exception" )

   FC_DECLARE_DERIVED_EXCEPTION( block_tx_output_exception,         enumivo::chain::block_validate_exception, 3020001, "transaction outputs in block do not match transaction outputs from applying block" )
   FC_DECLARE_DERIVED_EXCEPTION( block_concurrency_exception,       enumivo::chain::block_validate_exception, 3020002, "block does not guarantee concurrent execution without conflicts" )
   FC_DECLARE_DERIVED_EXCEPTION( block_lock_exception,              enumivo::chain::block_validate_exception, 3020003, "shard locks in block are incorrect or mal-formed" )
   FC_DECLARE_DERIVED_EXCEPTION( block_resource_exhausted,          enumivo::chain::block_validate_exception, 3020004, "block exhausted allowed resources" )
   FC_DECLARE_DERIVED_EXCEPTION( block_too_old_exception,           enumivo::chain::block_validate_exception, 3020005, "block is too old to push" )

   FC_DECLARE_DERIVED_EXCEPTION( tx_missing_auth,                   enumivo::chain::transaction_exception, 3030001, "missing required authority" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_missing_sigs,                   enumivo::chain::transaction_exception, 3030002, "signatures do not satisfy declared authorizations" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_irrelevant_auth,                enumivo::chain::transaction_exception, 3030003, "irrelevant authority included" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_irrelevant_sig,                 enumivo::chain::transaction_exception, 3030004, "irrelevant signature included" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_duplicate_sig,                  enumivo::chain::transaction_exception, 3030005, "duplicate signature included" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_committee_approval,        enumivo::chain::transaction_exception, 3030006, "committee account cannot directly approve transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_fee,                  enumivo::chain::transaction_exception, 3030007, "insufficient fee" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_missing_recipient,              enumivo::chain::transaction_exception, 3030009, "missing required recipient" )
   FC_DECLARE_DERIVED_EXCEPTION( checktime_exceeded,                enumivo::chain::transaction_exception, 3030010, "allotted processing time was exceeded" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_duplicate,                      enumivo::chain::transaction_exception, 3030011, "duplicate transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_transaction_exception,     enumivo::chain::transaction_exception, 3030012, "unknown transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_scheduling_exception,           enumivo::chain::transaction_exception, 3030013, "transaction failed during sheduling" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_unknown_argument,               enumivo::chain::transaction_exception, 3030014, "transaction provided an unknown value to a system call" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_resource_exhausted,             enumivo::chain::transaction_exception, 3030015, "transaction exhausted allowed resources" )
   FC_DECLARE_DERIVED_EXCEPTION( page_memory_error,                 enumivo::chain::transaction_exception, 3030016, "error in WASM page memory" )
   FC_DECLARE_DERIVED_EXCEPTION( unsatisfied_permission,            enumivo::chain::transaction_exception, 3030017, "Unsatisfied permission" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_msgs_auth_exceeded,             enumivo::chain::transaction_exception, 3030018, "Number of transaction messages per authorized account has been exceeded" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_msgs_code_exceeded,             enumivo::chain::transaction_exception, 3030019, "Number of transaction messages per code account has been exceeded" )
   FC_DECLARE_DERIVED_EXCEPTION( wasm_execution_error,              enumivo::chain::transaction_exception, 3030020, "Runtime Error Processing WASM" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_decompression_error,            enumivo::chain::transaction_exception, 3030021, "Error decompressing transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( expired_tx_exception,              enumivo::chain::transaction_exception, 3030022, "Expired Transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_exp_too_far_exception,          enumivo::chain::transaction_exception, 3030023, "Transaction Expiration Too Far" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_ref_block_exception,       enumivo::chain::transaction_exception, 3030024, "Invalid Reference Block" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_apply_exception,                enumivo::chain::transaction_exception, 3030025, "Transaction Apply Exception" )
   FC_DECLARE_DERIVED_EXCEPTION( wasm_serialization_error,          enumivo::chain::transaction_exception, 3030026, "Serialization Error Processing WASM" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_empty_region,                   enumivo::chain::transaction_exception, 3030027, "Transaction contains an empty region" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_empty_cycle,                    enumivo::chain::transaction_exception, 3030028, "Transaction contains an empty cycle" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_empty_shard,                    enumivo::chain::transaction_exception, 3030029, "Transaction contains an empty shard" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_receipt_inconsistent_status,    enumivo::chain::transaction_exception, 3030030, "Transaction receipt applied status does not match received status" )
   FC_DECLARE_DERIVED_EXCEPTION( cfa_irrelevant_auth,               enumivo::chain::transaction_exception, 3030031, "context-free action should have no required authority" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_no_action,                      enumivo::chain::transaction_exception, 3030032, "transaction should have at least one normal action" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_no_auths,                       enumivo::chain::transaction_exception, 3030033, "transaction should have at least one required authority" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_receipt_inconsistent_cpu,       enumivo::chain::transaction_exception, 3030034, "Transaction receipt applied kcpu_usage does not match received kcpu_usage" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_receipt_inconsistent_net,       enumivo::chain::transaction_exception, 3030035, "Transaction receipt applied net_usage_words does not match received net_usage_words" )

   FC_DECLARE_DERIVED_EXCEPTION( account_name_exists_exception,     enumivo::chain::action_validate_exception, 3040001, "account name already exists" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_action_args_exception,       enumivo::chain::action_validate_exception, 3040002, "Invalid Action Arguments" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_pts_address,               enumivo::chain::utility_exception, 3060001, "invalid pts address" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_feeds,                enumivo::chain::chain_exception, 37006, "insufficient feeds" )

   FC_DECLARE_DERIVED_EXCEPTION( pop_empty_chain,                   enumivo::chain::undo_database_exception, 3070001, "there are no blocks to pop" )

   FC_DECLARE_DERIVED_EXCEPTION( name_type_exception,               enumivo::chain::chain_type_exception, 3120001, "Invalid name" )
   FC_DECLARE_DERIVED_EXCEPTION( public_key_type_exception,         enumivo::chain::chain_type_exception, 3120002, "Invalid public key" )
   FC_DECLARE_DERIVED_EXCEPTION( private_key_type_exception,        enumivo::chain::chain_type_exception, 3120003, "Invalid private key" )
   FC_DECLARE_DERIVED_EXCEPTION( authority_type_exception,          enumivo::chain::chain_type_exception, 3120004, "Invalid authority" )
   FC_DECLARE_DERIVED_EXCEPTION( action_type_exception,             enumivo::chain::chain_type_exception, 3120005, "Invalid action" )
   FC_DECLARE_DERIVED_EXCEPTION( transaction_type_exception,        enumivo::chain::chain_type_exception, 3120006, "Invalid transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( abi_type_exception,                enumivo::chain::chain_type_exception, 3120007, "Invalid ABI" )
   FC_DECLARE_DERIVED_EXCEPTION( block_id_type_exception,           enumivo::chain::chain_type_exception, 3120008, "Invalid block ID" )
   FC_DECLARE_DERIVED_EXCEPTION( transaction_id_type_exception,     enumivo::chain::chain_type_exception, 3120009, "Invalid transaction ID" )
   FC_DECLARE_DERIVED_EXCEPTION( packed_transaction_type_exception, enumivo::chain::chain_type_exception, 3120010, "Invalid packed transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( asset_type_exception,              enumivo::chain::chain_type_exception, 3120011, "Invalid asset" )

   FC_DECLARE_DERIVED_EXCEPTION( missing_chain_api_plugin_exception,                 enumivo::chain::missing_plugin_exception, 3130001, "Missing Chain API Plugin" )
   FC_DECLARE_DERIVED_EXCEPTION( missing_wallet_api_plugin_exception,                enumivo::chain::missing_plugin_exception, 3130002, "Missing Wallet API Plugin" )
   FC_DECLARE_DERIVED_EXCEPTION( missing_account_history_api_plugin_exception,       enumivo::chain::missing_plugin_exception, 3130003, "Missing Account History API Plugin" )
   FC_DECLARE_DERIVED_EXCEPTION( missing_net_api_plugin_exception,                   enumivo::chain::missing_plugin_exception, 3130004, "Missing Net API Plugin" )

   FC_DECLARE_DERIVED_EXCEPTION( wallet_exist_exception,            enumivo::chain::wallet_exception, 3140001, "Wallet already exists" )
   FC_DECLARE_DERIVED_EXCEPTION( wallet_nonexistent_exception,      enumivo::chain::wallet_exception, 3140002, "Nonexistent wallet" )
   FC_DECLARE_DERIVED_EXCEPTION( wallet_locked_exception,           enumivo::chain::wallet_exception, 3140003, "Locked wallet" )
   FC_DECLARE_DERIVED_EXCEPTION( wallet_missing_pub_key_exception,  enumivo::chain::wallet_exception, 3140004, "Missing public key" )
   FC_DECLARE_DERIVED_EXCEPTION( wallet_invalid_password_exception, enumivo::chain::wallet_exception, 3140005, "Invalid wallet password" )

   FC_DECLARE_DERIVED_EXCEPTION( rate_limiting_state_inconsistent,  enumivo::chain::rate_limiting_invariant_exception, 3150001, "internal state is no longer consistent" )
   FC_DECLARE_DERIVED_EXCEPTION( rate_limiting_overcommitment,      enumivo::chain::rate_limiting_invariant_exception, 3150002, "chain resource limits are overcommitted" )


   #define ENU_RECODE_EXC( cause_type, effect_type ) \
      catch( const cause_type& e ) \
      { throw( effect_type( e.what(), e.get_log() ) ); }

} } // enumivo::chain
