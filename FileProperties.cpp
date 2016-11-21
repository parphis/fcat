/*
 * FileEntry.cpp
 *
 * Its purpose is to hold some property of a file.
 *
 * These properties are:
 *  - full path.
 *  - extension.
 *  - time of last access.
 *  - time of creation.
 *  - time of last modification.
 *  - size in bytes.
 *
 * It provides several getter methods to access the
 * stored properties.
 *
 *  Created on: 2013.03.28.
 *      Author: istvan_vig
 */

#include "FileProperties.h"

FileProperties::FileProperties() : m_ATime(0), m_CTime(0), m_MTime(0),
	m_Size(0L), m_FullPath(""), m_Extension("") {
	;

}

FileProperties::~FileProperties() {
	;
}

/**
 * Returns the extension of a given file.
 * It searches for a dot in the given file path
 * from the end of the string and assumes
 * that the last characters right after the dot
 * is the file extension.
 *
 * @param Path / std::string
 * @return std::string
 */
std::wstring FileProperties::ExtractExtension(std::wstring Path) {
	std::size_t pos = Path.find_last_of(L".");

	if(pos!=std::string::npos) {
		return Path.substr(pos+1, Path.length());
	}
	else	return L"";
}

/**
 * Returns the string representation of a given time_t structure
 * using the format string 'format'.
 *
 * @param t
 * @param format
 * @return string
 */
std::string FileProperties::ToDateString(time_t t, const char *format) {
	char buff[128];
	std::string res;

	memset(buff, '\0', 128);

	strftime(buff, sizeof(buff), format, localtime(&t));

	res = buff;

	return res;
}

std::wstring FileProperties::ToDateStringW(time_t t, const wchar_t *format) {
	wchar_t *buff = new wchar_t[128];
	std::wstring res;

	wmemset(buff, L'\0', 128);

	wcsftime(buff, 128, format, localtime(&t));

	res = buff;

	delete [] buff;

	return res;
}

/**
 * Public function for test.
 */
std::wstring FileProperties::ExtractExtensionTest(std::wstring Path) {
	return this->ExtractExtension(Path);
}

/**
 * Store the file properties provided by the
 * stat structure.
 *
 * @param Properties / struct stat
 * @param FullPath / std:.string
 */
void FileProperties::SetProperties(struct _stat Properties, std::wstring FullPath) {
	this->m_ATime = Properties.st_atime;
	this->m_CTime = Properties.st_ctime;
	this->m_MTime = Properties.st_mtime;
	this->m_Size = Properties.st_size;
	this->m_FullPathW = FullPath;
	this->m_ExtensionW = this->ExtractExtension(FullPath);
}

/*** Return raw types ***/
time_t FileProperties::getRawAccessTime(void) {
	return this->m_ATime;
}

std::wstring FileProperties::getAccessTimeString(const wchar_t *format) {
	return this->ToDateStringW(this->m_ATime, format);
}

time_t FileProperties::getRawCreationTime(void) {
	return this->m_CTime;
}

std::wstring FileProperties::getCreationTimeString(const wchar_t *format) {
	return this->ToDateStringW(this->m_CTime, format);
}

time_t FileProperties::getRawModifyTime(void) {
	return this->m_MTime;
}

std::wstring FileProperties::getModifyTimeString(const wchar_t *format) {
	return this->ToDateStringW(this->m_MTime, format);
}

long FileProperties::getSize(void) {
	return this->m_Size;
}

std::string FileProperties::getFullPath(void) {
	return this->m_FullPath;
}

std::string FileProperties::getExtension(void) {
	return this->m_Extension;
}

std::wstring FileProperties::getFullPathW(void) {
	return this->m_FullPathW;
}

std::wstring FileProperties::getExtensionW(void) {
	return this->m_ExtensionW;
}
