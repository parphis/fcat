#ifndef UTF8TOOLH
#define	UTF8TOOLH

std::string to_utf8(const wchar_t* buffer, int len);
std::string to_utf8(const std::wstring& str);

#endif
