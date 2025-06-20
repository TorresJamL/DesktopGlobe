#include "Config.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
Config::~Config() {
	delete configFileName;
	delete title;
}
Config::Config() {
	getConfigContents();
}
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
	ifstream contents(configFileName);
	
	if (!contents.is_open()) {
		return;
	}
	string line;

	while (getline(contents, line)) {
		string configSetting, configValue;
		vector<string> configPair = getSettingValuePair(line);
		configSetting = configPair[0];
		configValue = configPair[1];
		char* end;
		if (configSetting == "radius") {
			globeRadius = strtof(configValue.c_str(), &end);
		}
	}

	contents.close();
}
