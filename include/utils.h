#ifndef PLANAR_REFLECTION_UTILS
#define PLANAR_REFLECTION_UTILS
#include <string>

class Utils
{
public:
	static std::string TextFileToString(const std::string& file_path);
	static std::stringstream TextFileToStream(const std::string& file_path);
};

#endif