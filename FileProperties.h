/*
 * FileEntry.h
 *
 *  Created on: 2013.03.28.
 *      Author: istvan_vig
 */

#ifndef FILEPROPERTIES_H_
#define FILEPROPERTIES_H_

#include <time.h>
#include <string>
#include <string.h>
#include <sys/stat.h>

class FileProperties {
private:
	time_t m_ATime;
	time_t m_CTime;
	time_t m_MTime;
	long m_Size;
	std::string m_FullPath;
	std::wstring m_FullPathW;
	std::string m_Extension;
	std::wstring m_ExtensionW;

	std::wstring ExtractExtension(std::wstring Path);
	std::string ToDateString(time_t t, const char *format);
	std::wstring ToDateStringW(time_t t, const wchar_t *format);
public:
	FileProperties();
	virtual ~FileProperties();

	std::wstring ExtractExtensionTest(std::wstring Path);
	void SetProperties(struct _stat Properties, std::wstring FullPath);

	time_t getRawAccessTime(void);
	std::wstring getAccessTimeString(const wchar_t *format);
	time_t getRawCreationTime(void);
	std::wstring getCreationTimeString(const wchar_t *format);
	time_t getRawModifyTime(void);
	std::wstring getModifyTimeString(const wchar_t *format);
	long getSize(void);
	std::string getFullPath(void);
	std::string getExtension(void);
	std::wstring getFullPathW(void);
	std::wstring getExtensionW(void);
};

#endif /* FILEENTRY_H_ */
