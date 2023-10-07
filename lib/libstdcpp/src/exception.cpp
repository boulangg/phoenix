#include <exception>

#include <stdlib.h>

using namespace std;

exception::exception() noexcept : _msg("")
{}

exception::exception(const exception& exc) noexcept : _msg(exc._msg)
{}

exception& exception::operator=(const exception& exc) noexcept
{
	_msg = exc._msg;
	return *this;
}

exception::~exception()
{}

const char* exception::what() const noexcept
{
	return _msg;
}

void std::terminate() noexcept
{
	abort();
}

#define DEFINE_GDB_PY_SCRIPT(script_name) \
  asm("\
.pushsection \".debug_gdb_scripts\", \"MS\",@progbits,1\n\
.byte 1 /* Python */\n\
.asciz \"" script_name "\"\n\
.popsection \n\
");

//DEFINE_GDB_PY_SCRIPT("debug/gdb-printers.py")

//asm(
//	".pushsection \".debug_gdb_scripts\", \"MS\",@progbits,1\n"
//	".byte 4\n"
//	".ascii \"gdb.inlined-script\\n\"\n"
//	".ascii \"class test_cmd (gdb.Command):\\n\"\n"
//	".ascii \"  def __init__ (self):\\n\"\n"
//	".ascii \"    super (test_cmd, self).__init__ ("
//	"\\\"test-cmd\\\", gdb.COMMAND_OBSCURE)\\n\"\n"
//	".ascii \"  def invoke (self, arg, from_tty):\\n\"\n"
//	".ascii \"    print (\\\"test-cmd output, arg = %s\\\" % arg)\\n\"\n"
//	".ascii \"test_cmd ()\\n\"\n"
//	".byte 0\n"
//	".popsection\n"
//);
