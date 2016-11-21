/**
 * Class TGetConfiguration
 *
 * Purpose
 * Provide a generic interface to get and set application options
 * stored in a standard XML file.
 *
 * @author Istvan Vig
 * @since 02/10/2012
 * @compiler GCC 4.6.2 (MingW)
 *
 */

#include "GetConfiguration.h"
#include "AppSettings.h"
#include "Tokenizer.h"

/**
 * It fills up the map with the string pairs
 * coming from the received file extension /
 * description strings.
 * The first map tag is the extension name
 * the second map tag is the extension description.
 *
 * @param exts string
 * @param descs string
 * @param map_exts map<string,string>&
 */
void TGetConfiguration::FillExtMap(const std::string exts, const std::string descs,
		std::map<std::string, std::string>& map_exts) {
	std::string token;
	unsigned n;

	unsigned int exts_size = 0;
	unsigned int descs_size = 0;
	unsigned int pos = 0;

	// calculate how many items need
	// to store the extension and description lists
	do {
		exts_size += 1;
	}while ( (pos = exts.find(';', pos+1))!=std::string::npos);

	pos = 0;

	do {
		descs_size += 1;
	}while ( (pos = descs.find(';', pos+1))!=std::string::npos);

	std::string extensions[exts_size], descriptions[descs_size];

	Tokenizer str_to_slipt;

	str_to_slipt.setString(exts);
	str_to_slipt.setDelimiter(";");

	n = 0;

	while( (token=str_to_slipt.next()) !="" ) {
		extensions[n] = token;
		n += 1;
	}

	str_to_slipt.setString(descs);
	str_to_slipt.setDelimiter(";");

	n = 0;

	while( (token=str_to_slipt.next()) !="" ) {
		descriptions[n] = token;
		n += 1;
	}

	// add the value pairs to the map list
	for (n=0; (n < exts_size) && (n < descs_size); n++) {
		if ( (extensions[n]!="") && (descriptions[n]!="") )
			file_extensions_to_discover.insert(std::make_pair(extensions[n], descriptions[n]));
	}
}
/**
 * Receives the application name option
 * from the AppSettings class.
 *
 * @param string AppName
 */
void TGetConfiguration::ReceiveAppName(void) {

	std::string appname;

	if (settings->GetOption("/fcat/app[@name]", appname, "name")==0) {
		this->ApplicationName = "";
	}
	else {
		this->ApplicationName = appname;
	}
}

/**
 * Receives the extension list node values
 * from the AppSettings class. The string
 * is a semicolon - separated string!
 *
 * @param string ExtList
 */
void TGetConfiguration::ReceiveExtensionList(void) {

	std::string ExtList;

	if (settings->GetOption("/fcat/file-exts/ext", ExtList, "")==0) {
		;
	}
	else {
		std::string descs;
		settings->GetOption("/fcat/file-exts/ext[@desc]", descs, "desc");

		this->FillExtMap(ExtList, descs, file_extensions_to_discover);
	}
}

/**
 * Constructor and desctructor
 */
TGetConfiguration::TGetConfiguration(const std::string FileName)  : ApplicationName(""),
	WindowWidth(800), WindowHeight(600), WindowX(100), WindowY(100), AutoStart(false),
	SearchInSubfolders(true), m_FileName(".\\fcat.xml")
{
	this->m_FileName = FileName;

	settings = new TAppSettings(this->m_FileName);

	// TODO Log the error events using the settings->IsError(string&) function
}

TGetConfiguration::~TGetConfiguration(void) {
	if(settings) {
		delete settings;
		settings = NULL;
	}
}

void TGetConfiguration::GetAppName(std::string& AppName) {
	ReceiveAppName();

	std::string error;
	if(settings->IsError(error)) {
		AppName = "";
		// TODO handle the errors
		std::cout << error << std::endl;
	}
	else	AppName = this->ApplicationName;
}

void TGetConfiguration::GetExtensionMap(std::map<std::string, std::string>& ExtList) {
	ReceiveExtensionList();

	ExtList = this->file_extensions_to_discover;
}

std::wstring TGetConfiguration::getExtensionString(void) {
	std::string str;
	std::wstring ws;

	str = "";

	for(it=this->file_extensions_to_discover.begin(); it!=this->file_extensions_to_discover.end(); ++it) {
		str += (*it).first + (*it).second + ";";
	}

	wchar_t *s = new wchar_t[str.length()*2];

	mbstowcs(s, str.c_str(), str.length());

	ws = s;

	delete [] s;

	return ws;
}
