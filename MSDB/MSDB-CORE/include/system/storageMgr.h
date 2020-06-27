#pragma once
#ifndef _MSDB_STORAGEMGR_H_
#define _MSDB_STORAGEMGR_H_

#include <array/arrayId.h>
#include <array/attributeId.h>
#include <util/singleton.h>
#include <io/bitstream.h>
#include <io/serializable.h>
#include <system/config.h>
#include <iostream>
#include <string>
#include <filesystem>

namespace msdb
{
using storage_version_type = unsigned int;
const storage_version_type MSDB_STORAGE_FORMAT_VERSION = 1;

extern const char* strBasePath;
extern const char* strConfigPath;
extern const char* strArrayPath;

using filePath = std::filesystem::path;

class storageMgr : public singleton<storageMgr>
{
private:
	storageMgr();

	~storageMgr();

public:
	config* loadConfigFile(ConfigType type);
	void saveConfigFile(config* cFile);

	void loadAttrIndex(arrayId arrId, attributeId attrId, pSerializable serialObj);
	void saveAttrIndex(arrayId arrId, attributeId attrId, pSerializable serialObj);

protected:
	filePath getArrayPath(arrayId arrId);
	filePath getArrayFolder(arrayId arrId);
	filePath getArrayIndexPath(arrayId arrId);

	// STD::FILESYSTEM
	bool createDirs(filePath& fp);
	bool removeFile(filePath& fp);
	bool isFile(filePath& fp);
	bool isDir(filePath& fp);
	bool isExists(filePath& fp);

	//const filePath& getConfigPath();
	//const filePath& getArrayPath();

protected:
	//bool init();

	// Paths
	filePath basePath_;
	filePath configPath_;
	filePath arrayPath_;

	// Folders
	filePath indexFolder_;

protected:
	friend singleton<storageMgr>;
};
}
#endif	//_MSDB_STORAGEMGR_H_
