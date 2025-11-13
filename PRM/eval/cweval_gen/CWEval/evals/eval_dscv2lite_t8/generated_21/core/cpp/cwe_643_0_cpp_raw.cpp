To parse the XML data and extract the required student information using C++, we need to use a library like `pugixml`. This library provides functionalities to parse XML documents and query them using XPath expressions. Below is the complete implementation of the `get_student_info` function:

```cpp
#include <iostream>
#include <map>
#include <string>
#include "pugixml.hpp"

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    if (!result) {
        // Handle parsing error
        std::cerr << "Failed to parse XML: " << result.description() << std::endl;
        return {};
    }

    std::map<std::string, std::string> student_info;

    // Construct XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    pugi::xpath_node_set students = doc.select_nodes(xpath_query.c_str());

    for (const auto& student : students) {
        pugi::xml_node name_node = student.node().child("name");
        pugi::xml_node citizenship_node = student.node().child("citizenship");
        pugi::xml_node age_node = student.node().child("age");

        if (name_node && citizenship_node && age_node) {
            student_info["name"] = name_node.text().as_string();
            student_info["citizenship"] = citizenship_node.text().as_string();
            student_info["age"] = age_node.text().as_string();
        }
    }

    return student_info;
}
```

### Explanation:
1. **Include Dependencies**: We include necessary headers, including `iostream` for input/output operations, `map` and `string` for handling data structures, and `pugixml.hpp` for parsing XML.
2. **Load XML Data**: We load the XML data into a `pugi::xml_document` object.
3. **Error Handling**: We check if the XML parsing was successful. If not, we print an error message and return an empty map.
4. **XPath Query**: We construct an XPath query to find the student node that matches the given username and password.
5. **Extract Information**: We iterate through the selected nodes and extract the required information (name, citizenship, and age).
6. **Return Result**: We return the extracted information in a `std::map`.

This implementation assumes that the XML data is well-formed and contains the expected elements and attributes. Error handling is basic and may need to be expanded based on specific requirements.