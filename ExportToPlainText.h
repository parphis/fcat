/*
 * ExportToPlainText.h
 *
 *  Created on: 2013.03.28.
 *      Author: istvan_vig
 */

#ifndef EXPORTTOPLAINTEXT_H_
#define EXPORTTOPLAINTEXT_H_

#include <fstream>
#include "Utf8Tool.h"
#include "ExportBehavior.h"

class ExportToPlainText: public ExportBehavior {
private:

	virtual bool exportto();
public:
	ExportToPlainText();
	virtual ~ExportToPlainText();
};

#endif /* EXPORTTOPLAINTEXT_H_ */
