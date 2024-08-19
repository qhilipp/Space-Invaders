#include <string>
#include <vector>

using namespace std;

vector<string> split(const string& str, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    if (delimiter.empty()) {
        std::cerr << "Delimiter cannot be empty!" << std::endl;
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
    replaceOccurrencesMoin(json, "\n", "");
    size_t keyPos = json.find("\"" + key + "\"");
    if (keyPos == string::npos) return "";

    size_t colonPos = json.find(":", keyPos);
    size_t start = json.find_first_of("\"", colonPos) + 1;
    size_t end = json.find_first_of("\"", start);

    return json.substr(start, end - start);
}

int jsonIntValue(string json, const string& key) {
	return 0; // TODO: Implement
}

double jsonDoubleValue(string json, const string& key) {
	return 0.0; // TODO: Implement
}

vector<string> jsonArrayValue(const string& json, const string& key) {
    string rawString = jsonStringValue(json, key);
    if(rawString == "") return {};
    vector<string> elements;
    for(string element : split(rawString, ",")) {
    	elements.push_back(element.substr(1, element.length() - 1));
    }
    return elements;
}

string getJSON(const string& identifier, const string& fileName) {
	ifstream file(string("resources/") + fileName + string(".json"));
    string json;
    if(!file.is_open()) {
        cerr << "Unable to open resources/" << fileName << ".json";
        return ""; // TODO: Exit program
    }
    file >> json;
    file.close();
    return jsonStringValue(json, identifier);
}