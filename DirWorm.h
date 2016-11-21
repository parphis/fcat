/*
 * DirWorm.h
 *
 *  Created on: 2013.03.21.
 *      Author: istvan_vig
 */

#ifndef DIRWORM_H_
#define DIRWORM_H_

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "easylogging++.h"
#include "FileProperties.h"
#include "ExportToPlainText.h"

class DirWorm {
private:
	bool m_GoSubdirs;
	long int m_RootLevel;
	long int m_SubdirLevel;
	double maxSubdirStack;

	char m_DirectorySeparator;
	std::string m_RootDir;
	std::wstring m_RootDirW;
	std::string m_CurrentPath;
	std::wstring m_CurrentPathW;
	std::string m_ExtensionList;
	std::wstring m_ExtensionListW;

	DIR *m_dRoot;
	// this variable needed to read Unicode
	// directories but due to this the class can not be
	// cross compiled...
	_WDIR *m_dRootW;
	struct dirent *m_Dirent;
	struct _wdirent *m_DirentW;
	struct stat m_StatBuffer;
	struct _stat m_StatBufferW;

	std::vector<std::string> m_SubdirStack;
	std::vector<std::wstring> m_SubdirStackW;
	std::vector<FileProperties> m_FP;
	std::vector<std::wstring> m_ExtList;

	ExportBehavior *e;

	std::wstring StringToWstring(std::string s);
	void DiscoverDirectoryA(void);
	void DiscoverDirectoryW(void);
	long int GetDirectoryLevel(std::string current_path);
	long int GetDirectoryLevelW(std::wstring current_path);
	inline bool hasExtension(std::wstring ext);
	void NewFile(void);
	void goA(void);
	void goW(void);
	bool FileNeedToStore(std::wstring current_file);
public:
	enum t_exporttypes {
			plaintext = 0,
			MSSQL,
			MySQL
		};

	DirWorm();
	virtual ~DirWorm();

	void setRootDir(std::string dir_and_extensions);
	void setRootDirW(std::wstring dir_and_extensions);
	std::string getRootDir(void);
	std::wstring getRootDirW(void);

	void setGoSubdirs(bool g);
	bool getGoSubdirs(void);

	void setSubdirLevel(long int l);
	long int getSubdirLevel(void);

	void go(void);
	void exportto(t_exporttypes t, std::string fn);

	long int GetDirectoryLevelTest(std::string current_path);

	void addNewExtension(std::wstring ext);
};

#endif /* DIRWORM_H_ */
