/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */

#include <agrio/chain/chain_id_type.hpp>
#include <agrio/chain/exceptions.hpp>

namespace agrio { namespace chain {

   void chain_id_type::reflector_verify()const {
      AGR_ASSERT( *reinterpret_cast<const fc::sha256*>(this) != fc::sha256(), chain_id_type_exception, "chain_id_type cannot be zero" );
   }

} }  // namespace agrio::chain

namespace fc {

   void to_variant(const agrio::chain::chain_id_type& cid, fc::variant& v) {
      to_variant( static_cast<const fc::sha256&>(cid), v);
   }

   void from_variant(const fc::variant& v, agrio::chain::chain_id_type& cid) {
      from_variant( v, static_cast<fc::sha256&>(cid) );
   }

} // fc
