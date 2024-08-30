#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ncurses.h>

using namespace std;

vector<string> split(const string& str, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    if (delimiter.empty()) {
        cerr << "Delimiter cannot be empty!" << "\n";
        return tokens;
    }

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    
    tokens.push_back(str.substr(start));

    return tokens;
}

void replaceOccurrencesMoin(string& str, const string& search, const string& replace) {
    size_t pos = 0;
    
    while((pos = str.find(search, pos)) != string::npos) {
        str.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

string jsonStringValue(string json, const string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if(keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return "";
    }
    
    size_t colonPos = json.find(":", keyPos);
    size_t start = json.find_first_of("\"", colonPos) + 1;
    size_t end = json.find_first_of("\"", start);

    return json.substr(start, end - start);
}

string jsonObjectValue(string json, const string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if(keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return "";
    }
    
    size_t colonPos = json.find(":", keyPos);
    size_t start = json.find_first_of("{", colonPos) + 1;
    int stack = 1;
    size_t end = start + 1;
    while(stack != 0) {
        if(json[end] == '{') stack++;
        if(json[end] == '}') stack--;
        end++;
    }

    return json.substr(start - 1, end - start + 1);
}

int jsonIntValue(string json, const string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if(keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return 0;
    }
    
    size_t start = json.find(":", keyPos) + 1;
    size_t end = json.find_first_of(",", start);

    return stoi(json.substr(start, end - start));
}

double jsonDoubleValue(string json, const string& key) {
	size_t keyPos = json.find("\"" + key + "\"");
    if(keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return 0;
    }
    
    size_t start = json.find(":", keyPos) + 1;
    size_t end = json.find_first_of(",", start);

    return stod(json.substr(start, end - start));
}

vector<string> jsonRawArrayValue(const string& json, const string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if(keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return {};
    }
    
    size_t colonPos = json.find(":", keyPos);
    size_t start = json.find_first_of("[", colonPos) + 1;
    size_t end = json.find_first_of("]", start);

    return split(json.substr(start, end - start), ",");
}

vector<string> jsonStringArrayValue(const string& json, const string& key) {
    vector<string> strings;
    for(string value : jsonRawArrayValue(json, key)) {
        strings.push_back(value.substr(1, value.size() - 2));
    }
    return strings;
}

vector<double> jsonDoubleArrayValue(const string& json, const string& key) {
    vector<double> doubles;
    for(string value : jsonRawArrayValue(json, key)) {
        doubles.push_back(stod(value));
    }
    return doubles;
}

string getJSON(const string& identifier, const string& fileName) {
	ifstream file(string("resources/") + fileName + string(".json"));
    string json;
    string line;
    if(file.is_open()) {
        while (getline(file, line)) {
            json.append(line);
        }
        file.close();
    } else {
        cerr << "Unable to open " << fileName << "\n";
    }
    file.close();
    replaceOccurrencesMoin(json, "\n", "");
    replaceOccurrencesMoin(json, "\t", "");
    replaceOccurrencesMoin(json, " ", "");
    return jsonObjectValue(json, identifier);
}

int getKeyCode(const string& input) {
    if(input == "left") return KEY_LEFT;
    if(input == "right") return KEY_RIGHT;
    if(input == "up") return KEY_UP;
    if(input == "down") return KEY_DOWN;
    if(input == "") {
        return ' '; // This is because the json function remove all white spaces
    }
    if(input.size() != 1) {
        cerr << "Invalid key input " << input;
        return -1;
    }
    return input[0];
}