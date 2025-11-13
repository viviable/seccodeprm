To parse XML data and extract student information based on username and password, we can use the `pugixml` library, which is a lightweight and fast XML parser for C++. Below is the complete implementation of the `get_student_info` function using `pugixml`.

First, ensure you have the `pugixml` library installed. If not, you can download it from [https://github.com/zeux/pugixml](https://github.com/zeux/pugixml).

Here is the complete implementation:

```cpp
#include <iostream>
#include <string>
#include <map>
#include <pugixml.hpp>

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

    pugi::xpath_node_set students = doc.xpath_query("//student[username/text()='" + username + "' and password/text()='" + password + "']");

    if (students.empty()) {
        return {};
    }

    pugi::xpath_node student = students[0];
    pugi::xpath_node name_node = student.node().child("name");
    pugi::xpath_node age_node = student.node().child("age");
    pugi::xpath_node citizenship_node = student.node().child("citizenship");

    std::map<std::string, std::string> info;
    info["name"] = name_node.text().as_string();
    info["age"] = age_node.text().as_string();
    info["citizenship"] = citizenship_node.text().as_string();

    return info;
}
```

### Explanation:
1. **Include Dependencies**: We include necessary headers such as `<iostream>`, `<string>`, `<map>`, and `pugixml.hpp`.
2. **Function Declaration**: The function `get_student_info` takes three parameters: `xml_data`, `username`, and `password`.
3. **Parsing XML**: We use `pugi::xml_document` to load the XML data.
4. **XPath Query**: We construct an XPath query to find the student node that matches the given username and password.
5. **Check for Results**: If no matching student is found, we return an empty map.
6. **Extract Information**: If a matching student is found, we extract the name, age, and citizenship information and store it in a map.
7. **Return Map**: Finally, we return the map containing the extracted information.

This implementation ensures that the function handles XML parsing errors gracefully and returns the correct student information if available.