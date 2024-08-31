#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

size_t checkForEnd(string json, size_t start);
vector<string> split(const string& str, const string& delimiter);
void replaceOccurrencesMoin(string& str, const string& search, const string& replace);
string jsonStringValue(string json, const string& key);
string jsonObjectValue(string json, const string& key);
int jsonIntValue(string json, const string& key);
double jsonDoubleValue(string json, const string& key);
vector<string> jsonRawArrayValue(const string& json, const string& key);
vector<string> jsonStringArrayValue(const string& json, const string& key);
vector<double> jsonDoubleArrayValue(const string& json, const string& key);
string getJSON(const string& identifier, const string& fileName);
int getKeyCode(const string& input);
int getRandomIndex(vector<double>& probabilities);