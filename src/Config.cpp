#include "Config.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

float Config::globeRadius = 0.2f;
int Config::globeSectorCount = 144;
int Config::globeStackCount = 144;
float Config::fovDeg = 45.0f;
float Config::camDist = 3.0f; 
float Config::rotX = 20.0f;
float Config::rotY = 90.0f;

std::string Config::title = "GlobeWindow";
int Config::desiredZ_order = -1;

std::vector<string> getSettingValuePair(string str, string del = "=") {
	int start, end = -1 * del.size();
	vector<string> setting_value;
	do {
		start = end + del.size();
		end = str.find(del, start);
		//cout << s.substr(start, end - start) << endl;
		setting_value.push_back(str.substr(start, end - start));
	} while (end != -1);
	return setting_value;
}
void Config::getConfigContents() {
	ifstream contents("resources\\config\\config.txt");

	if (!contents.is_open()) {
		return;
	}
	string line;

	while (getline(contents, line)) {
		string configSetting, configValue;
		vector<string> configPair = getSettingValuePair(line);
		configSetting = configPair[0];
		if (configPair.size() > 1)
			configValue = configPair[1];
		char* end;
		if (configSetting == "radius") {
			if (configValue != "NONE") {
				float radius = strtof(configValue.c_str(), &end);
				if (radius > 0.1) globeRadius = radius;
			}
		}
		else if (configSetting == "sector_count") {
			if (configValue != "NONE") {
				int count = stoi(configValue);
				if (count > 0) globeSectorCount = count;
			}
		}
		else if (configSetting == "stack_count") {
			if (configValue != "NONE") {
				int count = stoi(configValue);
				if (count > 0) globeStackCount = count;
			}
		}
		else if (configSetting == "Field_Of_View_(Degrees)") {
			if (configValue != "NONE") {
				float FOV = strtof(configValue.c_str(), &end);
				if (FOV > 0.0) fovDeg = FOV;
			}
		}
		else if (configSetting == "CameraDistance") {
			if (configValue != "NONE") {
				float distance = strtof(configValue.c_str(), &end);
				if (distance > 0.0) camDist = distance;
			}
		}
		else if (configSetting == "title") {
			if (configValue != "NONE")
				title = configValue;
		}
		else if (configSetting == "desired_Z_order") {
			if (configValue != "NONE") 
				desiredZ_order = stoi(configValue);
		}
		else continue; // This statement is unnecessary. But I really wanted to use continue.
	}
	contents.close();
}
const char* Config::toString() {
	auto str = new string(
		"# Configuration File For Desktop Globe \n\
		### Globe Settings : \n\
		radius = " + to_string(globeRadius) + " \n\
		sector_count = " + to_string(globeSectorCount) + " \n\
		stack_count = " + to_string(globeStackCount) + " \n\
		Field_Of_View_(Degrees) = " + to_string(fovDeg) + " \n\
		CameraDistance = " + to_string(camDist) + " \n\
		\n\
		### Window Settings : \n\
		title = " + title + " \n\
		desired_Z_order = " + to_string(desiredZ_order) + "\n\
	");
	return str->c_str();
}