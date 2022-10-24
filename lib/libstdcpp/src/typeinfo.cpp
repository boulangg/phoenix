#include <typeinfo>

namespace std
{

   type_info::~type_info ( void )
   {
   }

   type_info::type_info ( const type_info& arg )
      : tname ( arg.tname )
   {
   }

   type_info::type_info ( const char* pname )
      : tname ( pname )
   {
   }

   const char* type_info::name ( void ) const
   {
      return tname;
   }

   bool type_info::operator == ( const type_info& arg ) const
   {
      return tname == arg.tname;
   }

   bool type_info::operator != ( const type_info& arg ) const
   {
      return tname != arg.tname;
   }

}

namespace __cxxabiv1
{
#define ADD_CXX_TYPEINFO_SOURCE(t) \
  t::~t ( void ) {;} \
  t::t ( const char* n ) : std::type_info ( n ) {;} \

ADD_CXX_TYPEINFO_SOURCE ( __fundamental_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __array_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __function_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __enum_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __pbase_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __pointer_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __pointer_to_member_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __class_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __si_class_type_info )
ADD_CXX_TYPEINFO_SOURCE ( __vmi_class_type_info )

#undef ADD_CXX_TYPEINFO_SOURCE
}
