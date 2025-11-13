To parse the XML data and extract the required student information using XPath queries, you can use the `pugixml` library in C++. This library provides a convenient way to work with XML documents.

Here is the complete implementation of the `get_student_info` function:

```cpp
#include <iostream>
#include <string>
#include <map>
#include "pugixml.hpp"

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    if (!result) {
        // Handle parsing error
        std::cerr << "XML parsing failed: " << result.description() << std::endl;
        return student_info;
    }

    pugi::xpath_node_set nodes = doc.select_nodes("//student[username/text()='" + username + "' and password/text()='" + password + "']");

    if (nodes.size() == 0) {
        // No matching student found
        return student_info;
    }

    pugi::xpath_node node = nodes[0];
    student_info["name"] = node.child("name").text().as_string();
    student_info["age"] = node.child("age").text().as_string();
    student_info["citizenship"] = node.child("citizenship").text().as_string();

    return student_info;
}
```

This implementation uses the `pugixml` library to load the XML data and execute an XPath query to find the student with the specified username and password. If a match is found, the function extracts the student's name, age, and citizenship from the XML and returns them in a map. If no match is found or there is a parsing error, the function returns an empty map.

Make sure to link against the `pugixml` library when compiling your code. For example, if you are using g++, you can compile the code with the following command:

```sh
g++ -o get_student_info get_student_info.cpp -lpugixml
```