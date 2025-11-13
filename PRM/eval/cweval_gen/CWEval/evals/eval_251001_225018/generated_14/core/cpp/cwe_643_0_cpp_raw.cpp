To parse XML data and extract specific information using XPath queries in C++, we can utilize the `pugixml` library. This library provides a simple and efficient way to work with XML documents. Below is the complete implementation of the `get_student_info` function using `pugixml`.

First, you need to install the `pugixml` library. You can download it from its [GitHub repository](https://github.com/zeux/pugixml) or install it via a package manager like vcpkg or conan.

Here's the complete implementation:

```cpp
#include <iostream>
#include <map>
#include <string>
#include "pugixml.hpp"

/**
 * Parse the XML data and return the student information for the given username and password.
 * @param xml_data The XML data containing the student information.
 * @param username The username of the student to search for.
 * @param password The password of the student to search for.
 * @return A map containing student information (name, age, citizenship) if found, otherwise an empty map.
 */
std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    if (!result) {
        std::cerr << "XML parsing failed: " << result.description() << std::endl;
        return {};
    }

    std::map<std::string, std::string> student_info;

    for (pugi::xml_node student : doc.child("students").children("student")) {
        std::string current_username = student.child_value("username");
        std::string current_password = student.child_value("password");

        if (current_username == username && current_password == password) {
            student_info["name"] = student.child_value("name");
            student_info["age"] = student.child_value("age");
            student_info["citizenship"] = student.child_value("citizenship");
            break;
        }
    }

    return student_info;
}
```

### Explanation

1. **Include Headers**: We include necessary headers such as `<iostream>`, `<map>`, `<string>`, and `pugixml.hpp`.

2. **Function Signature**: The function `get_student_info` takes three parameters: `xml_data` (the XML string), `username` (the username to search for), and `password` (the password to search for).

3. **Parsing XML**: We create an instance of `pugi::xml_document` and load the XML data into it using `doc.load_string(xml_data.c_str())`. If the loading fails, we print an error message and return an empty map.

4. **Extracting Student Information**: We iterate through each `student` element within the `students` parent element. For each student, we check if the `username` and `password` match the provided values. If they do, we extract the `name`, `age`, and `citizenship` and store them in the `student_info` map.

5. **Return Result**: Finally, we return the `student_info` map containing the extracted information or an empty map if no matching student was found.

This implementation uses the `pugixml` library to parse the XML data and extract the required information efficiently.