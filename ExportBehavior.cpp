/*
 * ExportBehavior.cpp
 *
 *  Created on: 2013.03.28.
 *      Author: istvan_vig
 */

#include "ExportBehavior.h"

ExportBehavior::ExportBehavior() : m_Params("") {
	;
}

ExportBehavior::~ExportBehavior() {
	;
}

bool ExportBehavior::exportTo(void) {
	return this->exportto();
}

void ExportBehavior::setParameters(std::string params) {
	this->m_Params = params;
}

void ExportBehavior::setFilePropertiesList(std::vector<FileProperties> fp) {
	this->m_FP = fp;
}
