#pragma once
#ifndef _MSDB_IO_H_
#define _MSDB_IO_H_

#include <system/exceptions.h>

// EXCEPTION MACROS
#define _TRY_BEGIN try {
#define _CATCH(x) \
    }             \
    catch (x) {
#define _CATCH_ALL \
    }              \
    catch (...) {
#define _CATCH_END }

#define _RAISE(x) throw x
#define _RERAISE throw
#define _THROW(x) throw x
//////////////////////////////

#define _TRY_IO_BEGIN       _TRY_BEGIN
#define _CATCH_CA_IO_END                                            \
    _CATCH_ALL                                                      \
        throw EXCEPTIONS(MSDB_EC_IO_ERROR, MSDB_ER_ALL_IO_ERROR);   \
    _CATCH_END


#endif