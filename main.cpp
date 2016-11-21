/**
 * Main.cpp / FCat
 *
 * Main module for the file catalogiser command line.
 *
 * Compiler:
 * g++ (GCC) 4.6.2
 * GNU Make 3.82
 * Built for i386-pc-mingw32
 * Copyright (C) 2010  Free Software Foundation, Inc.
 * License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 *
 *
 * @author Istvan Vig
 * @since 23/05/2014
 */

#include <iostream>
#include "easylogging++.h"
#include "DirWorm.h"
//#include "GetConfiguration.h"

_INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]) {
    // versions before 9.13 always create myeasylog.log files.
    // since 9.13 it supports #define _ELPP_NO_DEFAULT_LOG_FILE macro to avoid this BUT
    // 9.13 needs C++11...
    // so I have changed the default file name in the easyloggingpp header
    //init the easylogging++ from a file called logcfg
    easyloggingpp::Configurations confFromFile(".\\logcfg");  // Load configuration from file
    easyloggingpp::Loggers::reconfigureAllLoggers(confFromFile); // Re-configures all the loggers to current configuration file

    LINFO << "Starting fcat...";  // Log message:  INFO: Log using conf from file

	/**
     * TODO load the software configuration from an SQLite database.
	 */


    /**
     * Start discovering the folders
     */
	DirWorm *worm = new DirWorm();

    /**
     * TODO use the extensions loaded from the application configuration here
     */
    LINFO << "Adding extension list";
    /**< if the first item of the extension list is an * (asterix) each file will
    be collected even if the extension list contains additional items */
    // only files with these extensions will be discovered
	worm->addNewExtension(L".txt");
	//worm->addNewExtension(L".xls");
	//worm->addNewExtension(L".pdf");
	//worm->addNewExtension(L".doc");

    /**
     * TODO use the options loaded from the application configuration here
     */
	// do not forget the ending slash when passing only volume letter!
	// adding * after the # sign means each file
	worm->setRootDirW(L"C:\\temp\\");
	// look into the sub-folders too
	worm->setGoSubdirs(true);
	// do not look into the sub-folders below this deep level
	worm->setSubdirLevel(0);

	LINFO << L"Root directory is " << worm->getRootDirW();

	// discover the files and save every available file property
	worm->go();

	// export the collected items, take care of the parameters
	worm->exportto(DirWorm::plaintext, "export_test.txt");

	// do something with the collected files...

	delete worm;
}
