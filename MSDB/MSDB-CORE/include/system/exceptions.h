#pragma once
#ifndef _MSDB_EXCEPTIONS_H_
#define _MSDB_EXCEPTIONS_H_

#include <iostream>

namespace msdb
{
#define EXCEPTIONS(_error_category, _error_code)               \
    msdb::UserException(REL_FILE, __FUNCTION__, __LINE__,      \
                         CORE_ERROR_NAMESPACE,                  \
                         _error_category, _error_code,             \
                         #_error_category, #_error_code)

    class msdb_exception : public virtual std::exception
    {
    public:
        msdb_exception() = default;

        msdb_exception(const char* file, const char* function, int32_t line,
                       const char* errors_namespace,
                       int32_t error_category, int32_t error_code,
                       const char* stringified_error_category,
                       const char* stringified_error_code);

        virtual ~msdb_exception() noexcept = default;

    private:
        std::string _file;
        std::string _function;
        int32_t _line;
        std::string _errors_namespace;
        int32_t _error_category;
        int32_t _error_code;
        std::string _stringified_error_category;
        std::string _stringified_error_code;
    };
}

#endif