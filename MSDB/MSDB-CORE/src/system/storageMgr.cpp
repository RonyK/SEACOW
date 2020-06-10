#include <system/storageMgr.h>
#include <system/exceptions.h>
#include <array/configArrays.h>
#include <xml/tinyxml2.h>
#include <fstream>

namespace msdb
{
const char* strBasePath = "./storage/";
const char* strConfigPath = "./storage/config/";
const char* strArrayPath = "./storage/array/";

const char* strArrayConfigFile = "arrays.xml";

//void msdb::storageMgr::getConfigFile(std::string path)
//{
//	std::fstream fs;
//	fs.open(path, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
//
//	fs.close();
//	return;
//}

config* storageMgr::loadConfigFile(ConfigType type)
{
	tinyxml2::XMLDocument xmlDoc;

	switch (type)
	{
	case ConfigType::ARRAYLIST:
		xmlDoc.LoadFile(strArrayConfigFile);
		return new configArrayList(xmlDoc.FirstChild());
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_UNKNOWN_ERROR, MSDB_ER_UNKNOWN_ERROR));
	}

	return nullptr;
}

void storageMgr::saveConfigFile(config* cFile)
{
	switch(cFile->getType())
	{
	case ConfigType::ARRAYLIST:
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_UNKNOWN_ERROR, MSDB_ER_UNKNOWN_ERROR));
	}
}

bool storageMgr::init()
{
	this->basePath_ = filePath(strBasePath);
	this->configPath_ = filePath(strConfigPath);
	this->arrayPath_ = filePath(strArrayPath);

	return true;
}
}
