/*
 * ExportToPlainText.cpp
 *
 * Export the collected file properties in a readable
 * plain text format.
 *
 * Structure is:
 * CreationTime\tFullPath/tSize
 *
 *  Created on: 2013.03.28.
 *      Author: istvan_vig
 */

#include "ExportToPlainText.h"


ExportToPlainText::ExportToPlainText() {
	;
}

ExportToPlainText::~ExportToPlainText() {
	;
}

bool ExportToPlainText::exportto(void) {
	std::ofstream out;
	std::vector<FileProperties>::const_iterator it;

	out.open(this->m_Params.c_str());

	for(it=this->m_FP.begin(); it<this->m_FP.end(); ++it) {
		FileProperties f = (*it);

		std::wstring s = L"Creation time " + f.getCreationTimeString(L"%Y.%m.%d. %H:%M:%S");
		s += L"\tModification time " + f.getModifyTimeString(L"%Y.%m.%d. %H:%M:%S");
		s += L"\tLast access time " + f.getAccessTimeString(L"%Y.%m.%d. %H:%M:%S");
		s += L"\t" + f.getFullPathW();
		s += L"\t Extension " + f.getExtensionW();
		s += L"\tSize ";

		// due to the to_utf8() this class can not be cross-compiled
		// it uses the WideCharToMultibyte() to convert the wchar_t string
		// into UTF-8
		std::string str = to_utf8(s);

		out << str << f.getSize() << std::endl;
	}

	out.close();

	return true;
}
