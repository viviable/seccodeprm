To parse XML data and extract student information using C++, we can utilize the `pugixml` library. This library provides a straightforward way to work with XML documents. Below is the complete implementation of the `get_student_info` function using `pugixml`.

First, ensure you have the `pugixml` library installed. You can download it from [here](https://github.com/zeux/pugixml).

Here's the complete implementation:

```cpp
#include <iostream>
#include <string>
#include <map>
#include "pugixml.hpp"

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    if (!result) {
        // Handle parsing error
        std::cerr << "XML parsing failed: " << result.description() << std::endl;
        return {};
    }

    pugi::xpath_node_set students = doc.xpath_nodes("//student[username/text()='" + username + "' and password/text()='" + password + "']");

    if (students.empty()) {
        // No matching student found
        return {};
    }

    pugi::xpath_node student = students[0];
    pugi::xpath_node name_node = student.node().child("name");
    pugi::xpath_node age_node = student.node().child("age");
    pugi::xpath_node citizenship_node = student.node().child("citizenship");

    std::map<std::string, std::string> info;
    if (!name_node.empty()) {
        info["name"] = name_node.text().as_string();
    }
    if (!age_node.empty()) {
        info["age"] = age_node.text().as_string();
    }
    if (!citizenship_node.empty()) {
        info["citizenship"] = citizenship_node.text().as_string();
    }

    return info;
}
```

This code includes the necessary headers, defines the `get_student_info` function, and uses the `pugixml` library to parse the XML data and extract the required student information. If the student is not found or there is a parsing error, the function returns an empty map.