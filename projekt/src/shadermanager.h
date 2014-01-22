#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include <string>
#include <array>

namespace core {
class ShaderManager {
public:
	ShaderManager();
	~ShaderManager() {
	};
	
	enum shaderId {
		BASIC = 0,
		COLOR_CUBE = 1,
		TEX2SCREEN = 2
	};
	unsigned int getId(shaderId id);

	bool loadShaders(std::string vertFileName, std::string fragFileName, shaderId id);
	bool loadShadersG(std::string vertFileName, std::string fragFileName, std::string geomFileName, shaderId id);	

	static std::string fixPath(std::string in);
	

private:
	unsigned int loadShader(const char *vertFileName, const char *fragFileName);
	unsigned int loadShaderG(const char *vertFileName, const char *fragFileName, const char *geomFileName);
	char* readFile(char *file);
	void printShaderInfoLog(unsigned int obj, const char *fn);
	void printProgramInfoLog(unsigned int obj, const char *vfn, const char *ffn, const char *gfn);
	unsigned int compileShaders(const char *vs, const char *fs, const char *gs, const char *vfn, const char *ffn, const char *gfn);
	
	static const int numShaders = 3;
	std::array<unsigned int, numShaders> shaders;
	
};
} //namespace core

#endif
