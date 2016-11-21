#ifndef	DISCOVERFILES_H
#define	DIScOVERFILES_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>

#include "AppSettings.h"
// thanks to http://www.songho.ca/misc/tokenizer/tokenizer.html
// using free license - Shong Ho Ahn
#include "Tokenizer.h"

class TGetConfiguration {
private:
	std::string ApplicationName;
	int WindowWidth, WindowHeight, WindowX, WindowY;
	bool AutoStart, SearchInSubfolders;
	std::string m_FileName;

	std::map<std::string, std::string> file_extensions_to_discover;
	std::map<std::string, std::string>::const_iterator it;

	TAppSettings *settings;

	void FillExtMap(const std::string exts, const std::string descs,
			std::map<std::string,std::string>& map_exts);
	void ReceiveAppName(void);
	void ReceiveExtensionList(void);
public:
	TGetConfiguration(const std::string FileName);
	virtual ~TGetConfiguration(void);

	void GetAppName(std::string& AppName);
	void GetExtensionMap(std::map<std::string, std::string>& ExtList);
	std::wstring getExtensionString(void);
};

#endif
