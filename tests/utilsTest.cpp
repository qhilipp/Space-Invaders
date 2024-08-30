#define BOOST_TEST_MODULE utilTest
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <vector>

#include "../include/util.h"

BOOST_AUTO_TEST_SUITE(utilTest)

BOOST_AUTO_TEST_CASE(test_checkForEnd){
     std::string json = R"({"key1":"value1","key2":"value2"})";

    // Test case: Find position of ','
    size_t start1 = 7;
    size_t expected1 = json.find_first_of(",", start1);
    BOOST_CHECK(checkForEnd(json, start1) == expected1);

    // Test case: Find position of '}'
    size_t start2 = 17;
    size_t expected2 = json.find_first_of("}", start2);
    BOOST_CHECK(checkForEnd(json, start2) == expected2);

    // Test case: No ',' or '}' found
    std::string json_no_end = R"({"key":"value")";
    size_t start3 = 13;
    BOOST_CHECK(checkForEnd(json_no_end, start3) == std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR split:
///////////////////////////////////////////////////////////////////////////////////////////////

// Test case for splitting with a simple delimiter
BOOST_AUTO_TEST_CASE(split_BasicTest) {
    vector<string> result = split("a,b,c", ",");
    vector<string> expected = {"a", "b", "c"};
    BOOST_CHECK(result == expected);
}

// Test case for delimiter not found in the string
BOOST_AUTO_TEST_CASE(split_DelimiterNotFound) {
    vector<string> result = split("abc", ",");
    vector<string> expected = {"abc"};
    BOOST_CHECK(result == expected);
}

// Test case for empty string with non-empty delimiter
BOOST_AUTO_TEST_CASE(split_EmptyString) {
    vector<string> result = split("", ",");
    vector<string> expected = {""};
    BOOST_CHECK(result == expected);
}

// Test case for empty delimiter
BOOST_AUTO_TEST_CASE(split_EmptyDelimiter) {
    vector<string> result = split("a b c", "");
    vector<string> expected = {};
    BOOST_CHECK(result == expected);
}

// Test case for delimiter is a single character
BOOST_AUTO_TEST_CASE(split_SingleCharacterDelimiter) {
    vector<string> result = split("abc|def|ghi", "|");
    vector<string> expected = {"abc", "def", "ghi"};
    BOOST_CHECK(result == expected);
}

// Test case for multiple consecutive delimiters
BOOST_AUTO_TEST_CASE(split_ConsecutiveDelimiters) {
    vector<string> result = split("a,,b,,c", ",");
    vector<string> expected = {"a", "", "b", "", "c"};
    BOOST_CHECK(result == expected);
}

// Test case for delimiter at the end of the string
BOOST_AUTO_TEST_CASE(split_DelimiterAtEnd) {
    vector<string> result = split("a,b,c,", ",");
    vector<string> expected = {"a", "b", "c", ""};
    BOOST_CHECK(result == expected);
}

// Test case for delimiter at the start of the string
BOOST_AUTO_TEST_CASE(split_DelimiterAtStart) {
    vector<string> result = split(",a,b,c", ",");
    vector<string> expected = {"", "a", "b", "c"};
    BOOST_CHECK(result == expected);
}


///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR replaceOccurencesMoin:
///////////////////////////////////////////////////////////////////////////////////////////////

// Test case for basic replacement
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_BasicTest) {
    string text = "hello world";
    replaceOccurrencesMoin(text, "world", "there");
    BOOST_CHECK_EQUAL(text, "hello there");
}

// Test case for no occurrences of search string
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_NoOccurrencesTest) {
    string text = "hello world";
    replaceOccurrencesMoin(text, "foo", "bar");
    BOOST_CHECK_EQUAL(text, "hello world");
}

// Test case for replacing with an empty string
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_WithEmptyStringTest) {
    string text = "hello world";
    replaceOccurrencesMoin(text, "world", "");
    BOOST_CHECK_EQUAL(text, "hello ");
}

// Test case for empty search string (should not replace anything)
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_EmptySearchStringTest) {
    string text = "hello world";
    replaceOccurrencesMoin(text, "", "bar");
    BOOST_CHECK_EQUAL(text, "hello world");
}

// Test case for multiple occurrences
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_MultipleOccurrencesTest) {
    string text = "foo foo foo";
    replaceOccurrencesMoin(text, "foo", "bar");
    BOOST_CHECK_EQUAL(text, "bar bar bar");
}

// Test case for search string being a substring of the replace string
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_SearchIsSubstringTest) {
    string text = "abababab";
    replaceOccurrencesMoin(text, "ab", "abc");
    BOOST_CHECK_EQUAL(text, "abcabcabcabc");
}

// Test case for replacement string being a substring of the search string
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_IsSubstringTest) {
    string text = "abcabcabc";
    replaceOccurrencesMoin(text, "abc", "ab");
    BOOST_CHECK_EQUAL(text, "ababab");
}

// Test case for empty input string
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_EmptyInputStringTest) {
    string text = "";
    replaceOccurrencesMoin(text, "search", "replace");
    BOOST_CHECK_EQUAL(text, "");
}

// Test case for search string longer than the input string
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_SearchLongerThanInputTest) {
    string text = "short";
    replaceOccurrencesMoin(text, "longsearch", "replace");
    BOOST_CHECK_EQUAL(text, "short");
}

// Test case for replace string longer than the search string
BOOST_AUTO_TEST_CASE(replaceOccurencesMoin_LongerThanSearchTest) {
    string text = "abc";
    replaceOccurrencesMoin(text, "ab", "abcd");
    BOOST_CHECK_EQUAL(text, "abcdc");
}


///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR jsonStringValue:
///////////////////////////////////////////////////////////////////////////////////////////////



// Test case for basic JSON extraction
BOOST_AUTO_TEST_CASE(jsonStringValue_BasicTest) {
    string json = R"({"name": "John", "age": "30"})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "name"), "John");
}

// Test case for extracting a value when key is not found
BOOST_AUTO_TEST_CASE(jsonStringValue_KeyNotFoundTest) {
    string json = R"({"name": "John", "age": "30"})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "address"), "");
}

// Test case for extracting a value when JSON is empty
BOOST_AUTO_TEST_CASE(jsonStringValue_EmptyJsonTest) {
    string json = "";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "name"), "");
}

// Test case for extracting a value when key is at the end
BOOST_AUTO_TEST_CASE(JsonKeyAtEndTest) {
    string json = R"({"name": "John", "age": "30", "city": "New York"})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "city"), "New York");
}

// Test case for extracting a value when there are spaces around the colon
BOOST_AUTO_TEST_CASE(jsonStringValue_SpacesAroundColonTest) {
    string json = R"({"name" : "John", "age": "30"})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "name"), "John");
}


// Test case for key with special characters
BOOST_AUTO_TEST_CASE(jsonStringValue_SpecialCharactersKeyTest) {
    string json = R"({"na@me": "John", "age": "30"})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "na@me"), "John");
}

// Test case for extracting a numeric value as a string
BOOST_AUTO_TEST_CASE(jsonStringValue_NumericValueTest) {
    string json = R"({"name": "John", "age": "30"})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "age"), "30");
}

// Test case for key with an empty value
BOOST_AUTO_TEST_CASE(jsonStringValue_EmptyValueTest) {
    string json = R"({"name": "John", "age": ""})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "age"), "");
}

// Test case for JSON with multiple occurrences of the same key
BOOST_AUTO_TEST_CASE(jsonStringValue_MultipleOccurrencesTest) {
    string json = R"({"name": "John", "name": "Doe", "age": "30"})";
    BOOST_CHECK_EQUAL(jsonStringValue(json, "name"), "John");
}


///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR jsonObjectValue:
///////////////////////////////////////////////////////////////////////////////////////////////




// Test case for extracting a simple nested object
BOOST_AUTO_TEST_CASE(jsonObjectValue_SimpleNestedObjectTest) {
    string json = R"({"user": {"name": "John", "age": "30"}, "city": "New York"})";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "user"), "{\"name\": \"John\", \"age\": \"30\"}");
}

// Test case for extracting an object nested within another object
BOOST_AUTO_TEST_CASE(jsonObjectValue_DeeplyNestedObjectTest) {
    string json = R"({"user": {"profile": {"name": "John", "age": "30"}}, "city": "New York"})";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "profile"), "{\"name\": \"John\", \"age\": \"30\"}");
}

// Test case for object with multiple levels of nesting
BOOST_AUTO_TEST_CASE(jsonObjectValue_MultipleLevelsNestedObjectTest) {
    string json = R"({"company": {"department": {"team": {"lead": "Alice", "members": 5}}}})";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "team"), "{\"lead\": \"Alice\", \"members\": 5}");
}

// Test case for key not found
BOOST_AUTO_TEST_CASE(jsonObjectValue_KeyNotFoundTest) {
    string json = R"({"user": {"name": "John", "age": "30"}})";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "address"), "");
}

// Test case for empty JSON string
BOOST_AUTO_TEST_CASE(jsonObjectValue_EmptyJsonTest) {
    string json = "";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "user"), "");
}

// Test case for non-object value associated with key
BOOST_AUTO_TEST_CASE(jsonObjectValue_NonObjectValueTest) {
    string json = R"({"user": "John", "age": "30"})";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "user"), "");
}

// Test case for object with an empty nested object
BOOST_AUTO_TEST_CASE(jsonObjectValue_EmptyNestedObjectTest) {
    string json = R"({"user": {}, "city": "New York"})";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "user"), "{}");
}

// Test case for object with multiple objects at the same level
BOOST_AUTO_TEST_CASE(jsonObjectValue_MultipleObjectsSameLevelTest) {
    string json = R"({"user": {"name": "John"}, "address": {"city": "New York", "zip": "10001"}})";
    BOOST_CHECK_EQUAL(jsonObjectValue(json, "address"), "{\"city\": \"New York\", \"zip\": \"10001\"}");
}


///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR jsonIntValue:
///////////////////////////////////////////////////////////////////////////////////////////////



// Test case for a simple valid integer value
BOOST_AUTO_TEST_CASE(jsonIntValue_SimpleIntValueTest) {
    string json = R"({"age": 30, "name": "John"})";
    BOOST_CHECK_EQUAL(jsonIntValue(json, "age"), 30);
}

// Test case for key not found
BOOST_AUTO_TEST_CASE(jsonIntValue_KeyNotFoundTest) {
    string json = R"({"age": 30, "name": "John"})";
    BOOST_CHECK_EQUAL(jsonIntValue(json, "height"), 0);
}

// Test case for negative integer value
BOOST_AUTO_TEST_CASE(jsonIntValue_NegativeIntValueTest) {
    string json = R"({"age": -25, "name": "John"})";
    BOOST_CHECK_EQUAL(jsonIntValue(json, "age"), -25);
}


// Test case for integer value at the end 
BOOST_AUTO_TEST_CASE(jsonIntValue_IntValueAtEndTest) {
    string json = R"({"name": "John", "age": 50})";
    BOOST_CHECK_EQUAL(jsonIntValue(json, "age"), 50);
}

// Test case for key, but no value
BOOST_AUTO_TEST_CASE(jsonIntValue_MissingIntValueTest) {
    string json = R"({"age": , "name": "John"})";
    BOOST_CHECK_THROW(jsonIntValue(json, "age"), std::invalid_argument);
}

// Test case for non-integer value, where an integer is expected
BOOST_AUTO_TEST_CASE(jsonIntValue_NonIntValueTest) {
    string json = R"({"age": "thirty", "name": "John"})";
    BOOST_CHECK_THROW(jsonIntValue(json, "age"), std::invalid_argument);
}

// Test case for malformed JSON (no closing brace)
BOOST_AUTO_TEST_CASE(jsonIntValue_MalformedTest) {
    string json = R"({"age": 25, "name": "John")";
    BOOST_CHECK_EQUAL(jsonIntValue(json, "age"), 25);
}

// Test case for integer value surrounded by whitespace
BOOST_AUTO_TEST_CASE(jsonIntValue_IntValueWithWhitespaceTest) {
    string json = R"({"age":    42   , "name": "John"})";
    BOOST_CHECK_EQUAL(jsonIntValue(json, "age"), 42);
}


///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR jsonDoubleValue:
///////////////////////////////////////////////////////////////////////////////////////////////




// Test case for a simple valid double value
BOOST_AUTO_TEST_CASE(jsonDoubleValue_SimpleDoubleValueTest) {
    string json = R"({"price": 29.99, "name": "Item"})";
    BOOST_CHECK_CLOSE(jsonDoubleValue(json, "price"), 29.99, 0.001);
}

// Test case for key not found
BOOST_AUTO_TEST_CASE(jsonDoubleValue_KeyNotFoundTest) {
    string json = R"({"price": 29.99, "name": "Item"})";
    BOOST_CHECK_EQUAL(jsonDoubleValue(json, "weight"), 0.0);
}

// Test case for negative double value
BOOST_AUTO_TEST_CASE(jsonDoubleValue_NegativeDoubleValueTest) {
    string json = R"({"price": -49.95, "name": "Item"})";
    BOOST_CHECK_CLOSE(jsonDoubleValue(json, "price"), -49.95, 0.001);
}

// Test case for double value with spaces around colon
BOOST_AUTO_TEST_CASE(jsonDoubleValue_DoubleValueWithSpacesTest) {
    string json = R"({"price" : 123.456, "name": "Item"})";
    BOOST_CHECK_CLOSE(jsonDoubleValue(json, "price"), 123.456, 0.001);
}

// Test case for double value at the end of JSON
BOOST_AUTO_TEST_CASE(jsonDoubleValue_DoubleValueAtEndTest) {
    string json = R"({"name": "Item", "price": 99.99})";
    BOOST_CHECK_CLOSE(jsonDoubleValue(json, "price"), 99.99, 0.001);
}

// Test case for missing double value (key is present but no value)
BOOST_AUTO_TEST_CASE(jsonDoubleValue_MissingDoubleValueTest) {
    string json = R"({"price": , "name": "Item"})";
    BOOST_CHECK_THROW(jsonDoubleValue(json, "price"), std::invalid_argument);
}

// Test case for non-numeric value (e.g., string) where a double is expected
BOOST_AUTO_TEST_CASE(jsonDoubleValue_NonNumericValueTest) {
    string json = R"({"price": "expensive", "name": "Item"})";
    BOOST_CHECK_THROW(jsonDoubleValue(json, "price"), std::invalid_argument);
}

// Test case for malformed JSON (no closing brace)
BOOST_AUTO_TEST_CASE(jsonDoubleValue_MalformedTest) {
    string json = R"({"price": 75.50, "name": "Item")";
    BOOST_CHECK_CLOSE(jsonDoubleValue(json, "price"), 75.50, 0.001);
}

// Test case for double value surrounded by whitespace
BOOST_AUTO_TEST_CASE(jsonDoubleValue_DoubleValueWithWhitespaceTest) {
    string json = R"({"price":    150.75   , "name": "Item"})";
    BOOST_CHECK_CLOSE(jsonDoubleValue(json, "price"), 150.75, 0.001);
}




///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR jsonStringArrayValue:
///////////////////////////////////////////////////////////////////////////////////////////////






// Test case for a simple valid string array
BOOST_AUTO_TEST_CASE(jsonStringArrayValue_SimpleStringArrayTest) {
    string json = R"({"fruits": ["apple", "banana", "cherry"], "price": 10})";
    vector<string> expected = {"apple", "banana", "cherry"};  // Adjusted expected values
    vector<string> result = jsonStringArrayValue(json, "fruits");
    
    BOOST_CHECK_EQUAL_COLLECTIONS(
        result.begin(),
        result.end(),
        expected.begin(),
        expected.end()
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////
//TESTS FOR jsonDoubleArrayValue:
///////////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(jsonDoubleArrayValue_ValidDoubles) {
    std::string json = R"({"numbers": [1.23, 4.56, 7.89]})";
    std::vector<double> expected = {1.23, 4.56, 7.89};
    std::vector<double> result = jsonDoubleArrayValue(json, "numbers");
    

    BOOST_CHECK_EQUAL_COLLECTIONS(
        result.begin(),
        result.end(),
        expected.begin(),
        expected.end()
    );
}



BOOST_AUTO_TEST_SUITE_END()

