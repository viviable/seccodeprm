To parse XML data in C++, we can use the RapidXML library, which is a fast and easy-to-use C++ XML parser. Below is the complete implementation of the `get_student_info` function using RapidXML:

```cpp
#include <iostream>
#include <map>
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

namespace rapidxml {
    template<class Ch>
    void print(std::ostream &out, const xml_node<Ch> *node, int indent = 0);
}

using namespace rapidxml;

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    
    try {
        // Parse the XML data
        xml_document<> doc;
        doc.parse<0>(const_cast<char*>(xml_data.c_str()));
        
        // Find the student node that matches the given username and password
        xml_node<>* students_node = doc.first_node("students");
        if (students_node != nullptr) {
            for (xml_node<>* student_node = students_node->first_node("student"); student_node != nullptr; student_node = student_node->next_sibling()) {
                xml_node<>* username_node = student_node->first_node("username");
                xml_node<>* password_node = student_node->first_node("password");
                
                if (username_node != nullptr && password_node != nullptr) {
                    std::string u = username_node->value();
                    std::string p = password_node->value();
                    
                    if (u == username && p == password) {
                        // Extract other student information
                        xml_node<>* name_node = student_node->first_node("name");
                        xml_node<>* citizenship_node = student_node->first_node("citizenship");
                        xml_node<>* age_node = student_node->first_node("age");
                        
                        if (name_node != nullptr) student_info["name"] = name_node->value();
                        if (citizenship_node != nullptr) student_info["citizenship"] = citizenship_node->value();
                        if (age_node != nullptr) student_info["age"] = age_node->value();
                        
                        break; // Found the student, no need to continue searching
                    }
                }
            }
        }
    } catch (const rapidxml::parse_error& e) {
        std::cerr << "XML parsing error: " << e.what() << std::endl;
    }
    
    return student_info;
}
```

This implementation includes the necessary headers and uses the RapidXML library to parse the XML data. It searches for the student node that matches the given username and password and extracts the required information into a map. If the student is not found, an empty map is returned.