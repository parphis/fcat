#ifndef	APPSETTINGS_H
#define	APPSETTINGS_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

/**
 * from http://pugixml.org/
 * version 0.9 / 07.01.2012
 */
#include "./xml/pugixml.hpp"

class TAppSettings {
public:
	TAppSettings(std::string FileName);
	virtual ~TAppSettings(void) {;}

	bool IsError(std::string& error_str);

	int GetOption(const std::string xpath, std::string& res, const std::string attrib="");
private:
    std::string m_FileName;
    std::string m_Errors;
};

#endif
