#include <system/storageMgr.h>
#include <system/exceptions.h>
#include <array/configArrays.h>
#include <xml/tinyxml2.h>
#include <fstream>

namespace msdb
{
const char* strBasePath =	"../storage/";
const char* strConfigPath = "../storage/config/";
const char* strArrayPath =	"../storage/array/";

const char* strIndexFolder = "indies";
const char* strIndexFilExtension = ".msdbindex";

const char* strArrayConfigFile = "arrays.xml";

//void msdb::storageMgr::getConfigFile(std::string path)
//{
//	std::fstream fs;
//	fs.open(path, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
//
//	fs.close();
//	return;
//}

storageMgr::storageMgr()
{
	std::cout << "current dir: " << std::filesystem::current_path() << std::endl;

	this->basePath_ = filePath(strBasePath);
	this->configPath_ = filePath(strConfigPath);
	this->arrayPath_ = filePath(strArrayPath);

	this->indexFolder_ = filePath(strIndexFolder);
}

storageMgr::~storageMgr()
{
}

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

void storageMgr::loadAttrIndex(arrayId arrId, attributeId attrId, pSerializable serialObj)
{
	filePath fpIndex = this->getArrayIndexPath(arrId) / std::to_string(attrId);
	fpIndex.replace_extension(strIndexFilExtension);

	std::ifstream fs;
	fs.open(fpIndex, std::fstream::out | std::fstream::binary);
	if(!fs.is_open())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_IO_ERROR, MSDB_ER_CANNOT_OPEN_FILE, fpIndex.generic_string().c_str()));
	}

	serialObj->deserialize(fs);
	fs.close();
}

void storageMgr::saveAttrIndex(arrayId arrId, attributeId attrId, pSerializable serialObj)
{
	filePath fpIndexFolder = this->getArrayIndexPath(arrId);
	filePath fpIndex = this->getArrayIndexPath(arrId) / std::to_string(attrId);
	fpIndex.replace_extension(strIndexFilExtension);
	if (!this->isExists(fpIndexFolder))
	{
		this->createDirs(fpIndexFolder);
	}

	std::ofstream fs;
	fs.open(fpIndex, std::fstream::in | std::fstream::trunc | std::fstream::binary);
	if(!fs.is_open())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_IO_ERROR, MSDB_ER_CANNOT_OPEN_FILE, fpIndex.generic_string().c_str()));
	}

	serialObj->serialize(fs);
	fs.close();
}

filePath storageMgr::getArrayPath(arrayId arrId)
{
	return this->arrayPath_ / this->getArrayFolder(arrId);
}

filePath storageMgr::getArrayFolder(arrayId arrId)
{
	return filePath(std::to_string(arrId));
}

filePath storageMgr::getArrayIndexPath(arrayId arrId)
{
	return this->getArrayPath(arrId) / this->indexFolder_;
}

bool storageMgr::createDirs(filePath& fp)
{
	return std::filesystem::create_directories(fp);
}

bool storageMgr::removeFile(filePath& fp)
{
	return std::filesystem::remove(fp);
}

bool storageMgr::isFile(filePath& fp)
{
	return std::filesystem::is_regular_file(fp);
}

bool storageMgr::isDir(filePath& fp)
{
	return std::filesystem::is_directory(fp);
}

bool storageMgr::isExists(filePath& fp)
{
	return std::filesystem::exists(fp);
}

//bool storageMgr::init()
//{
//
//
//	return true;
//}
}
