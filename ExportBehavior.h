/*
 * ExportBehavior.h
 *
 *  Created on: 2013.03.28.
 *      Author: istvan_vig
 */

#ifndef EXPORTBEHAVIOR_H_
#define EXPORTBEHAVIOR_H_

#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "easylogging++.h"
#include "FileProperties.h"

class ExportBehavior {
private:

	virtual bool exportto() = 0;
protected:
	std::string m_Params;

	std::vector<FileProperties> m_FP;
public:
	ExportBehavior();
	virtual ~ExportBehavior();
	bool exportTo(void);

	void setParameters(std::string params);
	void setFilePropertiesList(std::vector<FileProperties> fp);
};

#endif /* EXPORTBEHAVIOR_H_ */
