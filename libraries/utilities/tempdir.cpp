/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */

#include <agrio/utilities/tempdir.hpp>

#include <cstdlib>

namespace agrio { namespace utilities {

fc::path temp_directory_path()
{
   const char* agr_tempdir = getenv("AGR_TEMPDIR");
   if( agr_tempdir != nullptr )
      return fc::path( agr_tempdir );
   return fc::temp_directory_path() / "agr-tmp";
}

} } // agrio::utilities
