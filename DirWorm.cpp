/*
 * DirWorm.cpp
 *
 * Purpose
 * Try to provide a common interface to enumerate files
 * within a given root directory.
 * The collected file information stored in memory and
 * can be accessed by getter methods.
 *
 * File information stored are in a collection of
 * FileProperties classes:
 *
 *  - full path.
 *  - extension.
 *  - time of last access.
 *  - time of creation.
 *  - time of last modification.
 *  - size in bytes.
 *
 * The access rights can not be stored because querying it
 * on the Windows OS needs header files which are do not
 * accessible using a cross compiler like MingW32 or Cygwin.
 *
 *  Created on: 2013.03.21.
 *      Author: istvan_vig
 */

#include "DirWorm.h"

/**
 * Convert a string to wstring.
 *
 * @param string
 * @return wstring
 */
std::wstring DirWorm::StringToWstring(std::string s) {
	wchar_t *buf = new wchar_t[ s.size() ];

	size_t num_chars = mbstowcs( buf, s.c_str(), s.size() );

	std::wstring ws( buf, num_chars );

	delete[] buf;

	return ws;
}

/**
 * Count how many back slashes are in the given string.
 * Then it corrects the value using the m_RootLevel in
 * order to return the sub directory level from the
 * root directory point of view.
 *
 * E.g.
 * m_RootDir = "D:\\temp\\sample"
 * m_RootLevel = 2
 *
 * When we are in the sub directory "D:\\temp\\sample\\egy"
 * the sub directory level will be
 * GetDirectoryLevel("D:\\temp\\sample\\egy") - m_RootLevel = 1!
 *
 * @param current_path
 * @return level
 */
long int DirWorm::GetDirectoryLevel(std::string current_path) {
	long int i = 0;
	long int pos = 1;
	long int level = 0;

	while ( (pos = current_path.find(this->m_DirectorySeparator, i))!=std::string::npos ) {
		level++;
		i = pos + 1;
	}

	return (level-this->m_RootLevel) < 0 ? 0 : (level-this->m_RootLevel);
}

long int DirWorm::GetDirectoryLevelW(std::wstring current_path) {
	long int i = 0;
	long int pos = 1;
	long int level = 0;

	while ( (pos = current_path.find(this->m_DirectorySeparator, i))!=std::wstring::npos ) {
		level++;
		i = pos + 1;
	}

	return (level-this->m_RootLevel) < 0 ? 0 : (level-this->m_RootLevel);
}

/**
 * Determine if the passed extension string can be found in the m_ExtList vector.
 *
 * @param ext
 * @return true if the ext can be found in the m_ExtList vector
 */
inline bool DirWorm::hasExtension(std::wstring ext) {
    for (unsigned int i=0; i < this->m_ExtList.size(); i++) {
        if (this->m_ExtList.at(i).compare(ext)==0) {
            return true;
        }
    }
    return false;
}

/**
 * Public function and can be called from within the unit test.
 */
long int DirWorm::GetDirectoryLevelTest(std::string current_path) {
	return this->GetDirectoryLevel(current_path);
}

void DirWorm::NewFile(void) {
	FileProperties fp;

	fp.SetProperties(this->m_StatBufferW, this->m_CurrentPathW);

	this->m_FP.push_back(fp);
}

bool DirWorm::FileNeedToStore(std::wstring current_file) {
	unsigned int pos;
	std::wstring ext;

	pos = current_file.find_last_of(L".");

	ext = current_file.substr(pos);

	return this->hasExtension(ext);
}

void DirWorm::DiscoverDirectoryA(void) {
	if(this->m_dRoot!=NULL) {

		if(!this->m_SubdirStack.empty()) {
			this->m_SubdirStack.pop_back();
		}

		while ( (this->m_Dirent = readdir(this->m_dRoot))!=NULL ) {
			// do nothing with the current directory . and ..
			if( (strcmp(this->m_Dirent->d_name,".\0")==0) || (strcmp(this->m_Dirent->d_name,"..\0")==0) ) {
				continue;
			}

			this->m_CurrentPath = this->m_RootDir + this->m_DirectorySeparator + this->m_Dirent->d_name;

			if (stat(this->m_CurrentPath.c_str(), &this->m_StatBuffer)==-1) {
				if (errno==ENOENT) {
					LERROR << L"stat() failed with error ENOENT (" << this->StringToWstring(this->m_CurrentPath) << L")";
				}
				else {
					LERROR << L"stat() failed with error " << this->StringToWstring(this->m_CurrentPath) << L" " << errno;
				}
			}

			if(S_ISDIR(this->m_StatBuffer.st_mode)) {
				// do not add new sub directory path to the stack if
				//   - no need to go into the sub directories AND
				//   - the sub directory limit value is equals to
				//     or greater than the current sub directory level
				if (this->m_GoSubdirs) {
					if (this->GetDirectoryLevel(this->m_CurrentPath) <= this->m_SubdirLevel) {
						this->m_SubdirStack.push_back(this->m_CurrentPath);
					}
				}
			}
			else {
				LINFO << L"Found file: " << this->StringToWstring(this->m_CurrentPath);
				// save the file properties to memory
				if (FileNeedToStore(this->m_CurrentPathW)) {
					this->NewFile();
				}
			}

		}

		closedir(this->m_dRoot);

		if(!this->m_SubdirStack.empty()) {
			this->m_RootDir = this->m_SubdirStack.back();
		}
	}
	else {
		LERROR << L"Could not open directory \"" << this->StringToWstring(this->m_RootDir) << L"\"";

		if(!this->m_SubdirStack.empty()) {
			this->m_SubdirStack.pop_back();
			this->m_RootDir = this->m_SubdirStack.back();
		}
	}
}

void DirWorm::DiscoverDirectoryW(void) {
	if(this->m_dRootW!=NULL) {

		if(!this->m_SubdirStackW.empty()) {
			this->m_SubdirStackW.pop_back();
		}

		while ( (this->m_DirentW = _wreaddir(this->m_dRootW))!=NULL ) {
			// do nothing with the current directory . and ..
			if( (wcscmp(this->m_DirentW->d_name, L".\0")==0) || (wcscmp(this->m_DirentW->d_name, L"..\0")==0) ) {
				continue;
			}

			// decide if the ending slash has to be appended or not
			if (this->m_RootDirW.at(this->m_RootDirW.length()-1)==L'\\')
				this->m_CurrentPathW = this->m_RootDirW + this->m_DirentW->d_name;
			else
				this->m_CurrentPathW = this->m_RootDirW + L"\\" + this->m_DirentW->d_name;

			if (_wstat(this->m_CurrentPathW.c_str(), &this->m_StatBufferW)==-1) {
				if (errno==ENOENT) {
					LERROR << L"stat() failed with error ENOENT (" << this->m_CurrentPathW << L")";
				}
				else {
					LERROR << L"stat() failed with error " << this->m_CurrentPathW << L" " << errno;
				}
			}

			if(S_ISDIR(this->m_StatBufferW.st_mode)) {
				// do not add new sub directory path to the stack if
				//   - no need to go into the sub directories AND
				//   - the sub directory limit value is equals to
				//     or greater than the current sub directory level
				if (this->m_GoSubdirs) {
					if (this->GetDirectoryLevelW(this->m_CurrentPathW) <= this->m_SubdirLevel) {
						this->m_SubdirStackW.push_back(this->m_CurrentPathW);
					}
				}
			}
			else {
				LINFO << L"Found file: " << this->m_CurrentPathW;

                // if there was no any extension list specified write a log entry
				if (this->m_ExtList.size() > 0) {
                    // if the first item of the extension list is an * (asterix)
                    // add the found file to the memory
                    if (this->m_ExtList.at(0).compare(L"*")==0) {
                        this->NewFile();
                    }
                    else {
                        // save the file properties to memory
                        if (FileNeedToStore(this->m_CurrentPathW)) {
                            this->NewFile();
                        }
                    }
				}
				else {
                    LWARNING << "The extension list is empty! Stop file discovery...";
                    return;
				}
			}

		}

		_wclosedir(this->m_dRootW);

		if(!this->m_SubdirStackW.empty()) {
			this->m_RootDirW = this->m_SubdirStackW.back();
		}
	}
	else {
		LERROR << "Could not open directory \"" << this->m_RootDirW << "\"";

		if(!this->m_SubdirStackW.empty()) {
			this->m_SubdirStackW.pop_back();
			this->m_RootDirW = this->m_SubdirStackW.back();
		}
	}
}

void DirWorm::goA(void) {
	this->maxSubdirStack = this->m_SubdirStack.max_size();

	this->m_SubdirStack.clear();

	do {
		// try to avoid too many sub directory paths to be added
		if(this->m_SubdirStack.size()==this->maxSubdirStack) {
			LERROR << L"Reached the std::vector max_size limit (" << this->maxSubdirStack << L")!";
			break;
		}

		this->m_dRoot = opendir(this->m_RootDir.c_str());

		this->DiscoverDirectoryA();
	} while(!this->m_SubdirStack.empty());
}

void DirWorm::goW(void) {
	this->maxSubdirStack = this->m_SubdirStack.max_size();

	this->m_SubdirStackW.clear();

	do {
		// try to avoid too many sub directory paths to be added
		if(this->m_SubdirStackW.size()==this->maxSubdirStack) {
			LERROR << L"Reached the std::vector max_size limit (" << this->maxSubdirStack << L")!";
			break;
		}

		this->m_dRootW = _wopendir(this->m_RootDirW.c_str());

		this->DiscoverDirectoryW();
	} while(!this->m_SubdirStackW.empty());
}

DirWorm::DirWorm() : m_GoSubdirs(true), m_RootLevel(0),
		m_SubdirLevel(1000000), maxSubdirStack(1000000), m_DirectorySeparator('\\'),
		m_RootDir("."), m_RootDirW(L"."), m_dRoot(NULL), m_dRootW(NULL), m_Dirent(NULL), m_DirentW(NULL)
{
	// try to decide and set up the directory separator character for the current OS
	// in case failure the separator will be backslash
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
	this->m_DirectorySeparator = '\\';
#elif defined(__linux__)
	this->m_DirectorySeparator = '/';
#else
# error "Could not determine path separator."
#endif

	this->m_SubdirStack.clear();
	this->m_SubdirStackW.clear();
	this->m_FP.clear();

	this->e = NULL;
}

DirWorm::~DirWorm() {
	delete e;
	this->m_SubdirStack.clear();
	this->m_SubdirStackW.clear();
	this->m_FP.clear();
}

/**
 * Discover all files based on the m_RootDir directory.
 *
 * The discovery process uses a std::vector to store
 * the sub-directories. So this is not a recursive algorithm.
 * Let's call this vector stack from now.
 *
 * For cross - platform purposes it uses the dirent.h and sys/stat.h
 * headers.
 *
 * 1. Open the m_RootDir directory.
 * 2. Check if the stack is empty or not. If not remove the
 *    last added item.
 * 3. Read the directory content and store every file info in
 *    a class. These classes then will be stored in an array
 *    and they will be available by getter methods.
 * 4. If the stack is not empty set the m_RootDir to the last
 *    added directory path.
 *
 */
void DirWorm::go(void) {
	this->goW();
}

/**
 * Given an export type t_exporttypes this function calls the
 * belonging exportTo() function of the ExportBehavior's
 * descendant class.
 *
 * @param t_exporttypes
 * @param string / parameters to properly export the entries
 */
void DirWorm::exportto(t_exporttypes t, std::string fn) {
	delete e;

	if(t==plaintext) {
		this->e = new ExportToPlainText();
		this->e->setParameters(fn);
		this->e->setFilePropertiesList(this->m_FP);
		this->e->exportTo();
	}
}

/**
 * Set the starting root directory. The class will start the
 * discovery from this root directory.
 *
 * @param string or wstring
 */
void DirWorm::setRootDir(std::string dir_and_extensions) {
	this->m_RootDir = dir_and_extensions;

	this->m_RootLevel = this->GetDirectoryLevel(this->m_RootDir);
}
void DirWorm::setRootDirW(std::wstring dir_and_extensions) {
	this->m_RootDirW = dir_and_extensions;
}
std::string DirWorm::getRootDir(void) {
	return this->m_RootDir;
}
std::wstring DirWorm::getRootDirW(void) {
	return this->m_RootDirW;
}

void DirWorm::setGoSubdirs(bool g) {
	this->m_GoSubdirs = g;
}
bool DirWorm::getGoSubdirs(void) {
	 return this->m_GoSubdirs;
}

void DirWorm::setSubdirLevel(long int l) {
	this->m_SubdirLevel = l;
}
long int DirWorm::getSubdirLevel(void) {
	return this->m_SubdirLevel;
}

void DirWorm::addNewExtension(std::wstring ext) {
	this->m_ExtList.push_back(ext);
}
