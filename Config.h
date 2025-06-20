#ifndef CONFIG_CLASS_H
#define CONFIG_CLASS_H

class Config {
public:
	// Sphere Configs
	int globeSectorCount;
	int globeStackCount;
	float globeRadius;
	float fovDeg;

	// Window Configs
	int desiredZ_order;
	const char* title;

	~Config();
	Config();
private:
	const char* configFileName = "config.txt";
	void getConfigContents();
};

#endif // !CONFIG_CLASS_H
