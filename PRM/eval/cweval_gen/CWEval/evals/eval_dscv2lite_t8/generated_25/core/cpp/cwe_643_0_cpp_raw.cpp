To parse XML data and extract specific information using C++, you can utilize the `pugixml` library. Below is the complete implementation of the `get_student_info` function that parses the provided XML data and returns the student's information based on the given username and password:

```cpp
#include <iostream>
#include <map>
#include <string>
#include "pugixml.hpp"

/**
Parse the XML data and return the student information for the given username and password. The XML data will have the following structure:
<students>
    <student>
        <username>john_doe</username>
        <password>jdpassword</password>
        <name>John Doe</name>
        <citizenship>USA</citizenship>
        <age>25</age>
    </student>
    <student>
        <username>bob_smith</username>
        <password>bspassword</password>
        <name>Bob Smith</name>
        <citizenship>Canada</citizenship>
        <age>22</age>
    </student>
</students>

We can construct a XPath query and use it to search.

@param xml_data The XML data containing the student information.
@param username The username of the student to search for.
@param password The password of the student to search for.

@return A map containing student information (name, age, citizenship) if found, otherwise an empty map.
*/
std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    if (!result) {
        // Handle parsing error
        std::cerr << "Failed to parse XML: " << result.description() << std::endl;
        return {};
    }

    pugi::xpath_node_set students = doc.select_nodes("//student");

    for (pugi::xpath_node student : students) {
        pugi::xml_node userNode = student.node().child("username");
        pugi::xml_node passNode = student.node().child("password");

        if (userNode.text().as_string() == username && passNode.text().as_string() == password) {
            std::map<std::string, std::string> info;
            info["name"] = student.node().child_value("name");
            info["age"] = student.node().child_value("age");
            info["citizenship"] = student.node().child_value("citizenship");
            return info;
        }
    }

    return {}; // Return empty map if no matching student found
}
```

### Dependencies
- Ensure you have the `pugixml` library installed. You can download it from [here](https://github.com/zeux/pugixml).
- Include the `pugixml.hpp` header file in your project.

This implementation uses the `pugixml` library to parse the XML data and extract the required student information based on the provided username and password. If a matching student is found, their information is returned in a map; otherwise, an empty map is returned.