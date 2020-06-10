#include <array/buffer.h>
#include <assert.h>
#include <system/exceptions.h>

msdb::buffer::buffer()
{
}

void msdb::buffer::free()
{
	assert(false);
	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_NOT_IMPLEMENTED, "buffer::free()"));
}
