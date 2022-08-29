#include "File.hpp"

#include "Inode.hpp"

#include <include/constant.h>

/*File::File(Dentry* d) //:
		//dentry(d), pos(0)
{

}*/

File::~File()
{

}



ssize_t File::write_internal(char*, size_t, loff_t)
{
	return 0;
}
