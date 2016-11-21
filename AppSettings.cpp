/**
 * TAppSettings class
 *
 * Purpose
 * Providing an interface to query and/or save XML files
 * using the pugixml classes.
 * @credit http://pugixml.org/
 *
 * @since 04/10/2012
 * @author Istvan Vig
 */

#include "AppSettings.h"

TAppSettings::TAppSettings(std::string FileName) : m_FileName(".\\fcat.xml")
{
	this->m_FileName = FileName;
}

/**
 * Returns true if the Errors string of the class
 * is not empty.
 * Also sets the error_str attribute to Error.
 *
 * @param error_str [Out]
 */
bool TAppSettings::IsError(std::string& error_str) {
	if (!this->m_Errors.empty()) {
		error_str = this->m_Errors;
		return true;
	}

	error_str = "";
	return false;
}

/**
 * Receive one or more node value or node attribute from the given XML file.
 * The function needs an XPath expression. This will be used to receive the
 * needed node value or attribute.
 * Leaving the attrib argument empty means that the function will collect
 * the value or values of node or nodes given in the XPath expression.
 * If the attrib has an attribute name the function will get the value of
 * this attribute.
 *
 * An example to receive an attribute value:
 * 	GetOption("/fcat/app[@name]", AppName, "name")
 * An example to receive a collection of node values:
 * 	GetOption("/fcat/file-exts/ext", ExtList, "")
 *
 * @param xpath string
 * @param res string&
 * @param attrib string
 * @return int 0 on error 1 otherwise
 */
int TAppSettings::GetOption(const std::string xpath, std::string& res, const std::string attrib)
{
	if (xpath.empty()) {
		res = "";
		this->m_Errors.append("Empty XPath value. Could not receive the option.;");
		return 0;
	}

	pugi::xml_document doc;
	pugi::xml_parse_result result;

	if ( (result=doc.load_file(this->m_FileName.c_str())) ) {
		try {
			pugi::xpath_node_set option = doc.select_nodes(xpath.c_str());

			pugi::xpath_node_set::const_iterator it;

			if (option.size()==0) {
				res = "";
				this->m_Errors.append("The XPath query has resulted an empty set.;");
				return 0;
			}

			for (it = option.begin(); it!=option.end(); ++it) {
				pugi::xpath_node node = *it;

				if (attrib!="") {
					res.append(node.node().attribute(attrib.c_str()).value());
					if (option.size() > 1)	res.append(";");
				}
				else {
					res.append(node.node().child_value());
					if (option.size() > 1)	res.append(";");
				}
			}
		}
		catch (const pugi::xpath_exception& e) {
			res = "";
			this->m_Errors.append("XPath exception has occured: ");
			this->m_Errors.append(e.what());
			this->m_Errors.append(";");
			return 0;
		}
	}
	else {
		res = "";
		this->m_Errors.append("Could not load document: ");
		this->m_Errors.append(this->m_FileName);
		this->m_Errors.append(". ");
		this->m_Errors.append(result.description());
		this->m_Errors.append(";");
		return 0;
	}

	return res=="" ? 0 : 1;
}
