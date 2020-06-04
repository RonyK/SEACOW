#pragma once
#ifndef _MSDB_STORAGEMGR_H_
#define _MSDB_STORAGEMGR_H_
#include <util/singleton.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

namespace msdb
{
using storage_version_type = unsigned int;
const storage_version_type MSDB_STORAGE_FORMAT_VERSION = 1;

extern const char* strBasePath;
extern const char* strConfigPath;
extern const char* strArrayPath;

class storageMgr : public singleton<storageMgr>
{
	using filePath = std::filesystem::path;

	struct storageHeader
	{
		storage_version_type version;
	};

public:
	void getFile(std::string path);

	//const filePath& getConfigPath();
	const filePath& getArrayPath();
protected:
	bool init();

	filePath basePath_;
	filePath configPath_;
	filePath arrayPath_;
};
}

#endif	//_MSDB_STORAGEMGR_H_
