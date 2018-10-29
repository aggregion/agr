/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */

#include <agrio/utilities/tempdir.hpp>

#include <cstdlib>

namespace agrio { namespace utilities {

fc::path temp_directory_path()
{
   const char* eos_tempdir = getenv("EOS_TEMPDIR");
   if( eos_tempdir != nullptr )
      return fc::path( eos_tempdir );
   return fc::temp_directory_path() / "eos-tmp";
}

} } // agrio::utilities
