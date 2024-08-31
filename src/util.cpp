#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <sys/time.h>

using namespace std;

// Check whether string ends with "," or "}"
size_t checkForEnd(string json, size_t start) {
    size_t end1 = json.find_first_of(",", start);
    size_t end2 = json.find_first_of("}", start);

    // If next "," comes before next "}", then return position of it. Else return position of "}"
    return min(end1, end2);
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
    for (const string& value : jsonRawArrayValue(json, key)) {
        // Find position of quote at beginning
        size_t firstQuotePos = value.find_first_of("\"");
        //Find position of quote at the end
        size_t lastQuotePos = value.find_last_of("\"");

        // Check whether both positions exist and if they arent the same
        if (firstQuotePos != string::npos && lastQuotePos != string::npos && firstQuotePos != lastQuotePos) {
            // Extract the substring between the quotes
            strings.push_back(value.substr(firstQuotePos + 1, lastQuotePos - firstQuotePos - 1));
        } 
        else {
            // Handle cases where quotes are not found or incorrectly positioned
            // For instance, log an error, throw an exception, or handle as needed
            cerr << "Error: Malformed JSON string value: " << value << endl;
        }
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
        return ' '; // This is because the json function removes all white spaces
    }
    if(input.size() != 1) {
        cerr << "Invalid key input " << input;
        return -1;
    }
    return input[0];
}

int getRandomIndex(vector<double>& probabilities) {
    double randomValue = (rand() % 101) / 100.0;

    double cumulativeSum = 0.0;
    for(int i = 0; i < (int) probabilities.size(); ++i) {
        cumulativeSum += probabilities[i];
        if (randomValue <= cumulativeSum) {
            return i;
        }
    }

    return -1;
}

long int getTime() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int getRandom(int min, int max) {
    return rand() % (max - min) + min;
}