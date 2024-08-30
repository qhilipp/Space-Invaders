#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

size_t checkForEnd(string json, size_t start){ //Check whether string ends with "," or "}"
    size_t end1 = json.find_first_of(",", start);
    size_t end2 = json.find_first_of("}", start);
    if(end1 <end2) return end1; //If next "," comes before next "}", then return position of it. Else return position of "}"
    return end2;

}

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
    if(search != ""){
    while((pos = str.find(search, pos)) != string::npos) {
        str.replace(pos, search.length(), replace);
        pos += replace.length();
    }}
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
    //Checks whether key is in json
    if (keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return "";
    }
    //Checks whether it actually is a key and not a value
    size_t colonPos = json.find(":", keyPos);
    if (colonPos == string::npos) {
        cerr << "Could not find colon after key " << key << " in " << json << "\n";
        return "";
    }
    //Checks whether there is a json object after key
    size_t start = json.find_first_of("{", colonPos);
    if (start == string::npos) {
        cerr << "Could not find opening '{' after key " << key << " in " << json << "\n";
        return "";
    }

    int stack = 1;
    size_t end = start + 1;
    while (end < json.size() && stack != 0) {
        if (json[end] == '{') stack++;
        if (json[end] == '}') stack--;
        end++;
    }
    //checks for mismatches of curly braces
    if (stack != 0) {
        cerr << "Unmatched '{' in JSON string after key " << key << "\n";
        return "";
    }

    return json.substr(start, end - start);
}

int jsonIntValue(string json, const string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if(keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return 0;
    }
    
    size_t start = json.find(":", keyPos) + 1;
    size_t end = checkForEnd(json, start);

    return stoi(json.substr(start, end - start));
}

double jsonDoubleValue(string json, const string& key) {
	size_t keyPos = json.find("\"" + key + "\"");
    if(keyPos == string::npos) {
        cerr << "Could not find key " << key << " in " << json << "\n";
        return 0;
    }
    
    size_t start = json.find(":", keyPos) + 1;
    size_t end = checkForEnd(json,start);

    return stod(json.substr(start, end - start));
}

vector<string> jsonStringArrayValue(const string& json, const string& key) {
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

vector<double> jsonDoubleArrayValue(const string& json, const string& key) {
    vector<double> doubles;
    for(string value : jsonStringArrayValue(json, key)) {
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
    return jsonObjectValue(json, identifier);
}