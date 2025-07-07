#ifndef CONFIG_CLASS_H
#define CONFIG_CLASS_H

#include <string>

class Config {
public:
	const char* toString();
	// Sphere Configs
	static int globeSectorCount;
	static int globeStackCount;
	static float globeRadius;
	static float fovDeg;
	static float camDist;
	static float rotX;
	static float rotY;

	// Window Configs
	static int desiredZ_order;
	static std::string title;
	
	static void getConfigContents();
private:
	Config() = delete;
};

#endif // !CONFIG_CLASS_H
