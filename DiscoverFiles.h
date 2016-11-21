#ifndef	DISCOVERFILESH
#define	DIScOVERFILESH

#include <stdio.h>
#include <iostream>
#include <string>

class TDiscoverFiles {
private:
	std::string m_RootDir;
public:
	TDiscoverFiles(void);
	TDiscoverFiles(std::string RootDir);
	virtual ~TDiscoverFiles(void){};
};

#endif
