#include <system/storageMgr.h>
#include <fstream>

const char* strBasePath =	"./storage";
const char* strConfigPath = "./storage/config";
const char* strArrayPath =	"./storage/array";

void msdb::storageMgr::getFile(std::string path)
{
	std::fstream fs;
	fs.open(path, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);

	fs.close();
	return;
}

bool msdb::storageMgr::init()
{
	this->basePath_ = filePath(strBasePath);
	this->configPath_ = filePath(strConfigPath);
	this->arrayPath_ = filePath(strArrayPath);

	return true;
}