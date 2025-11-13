```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml parser
    xmlInitParser();
    xmlLineReaderPtr reader = xmlReaderForMemory(xml_data.c_str(), xml_data.size(), NULL, NULL, 0);
    if (!reader) {
        std::cerr << "Failed to create XML reader" << std::endl;
        return student_info;
    }

    int ret;
    while ((ret = xmlTextReaderRead(reader)) == 1) {
        xmlChar *name = xmlTextReaderConstName(reader);
        if (name && !xmlStrcmp(name, (const xmlChar *)"student")) {
            bool match = false;
            std::string current_username, current_password, current_name, current_citizenship, current_age;

            while ((ret = xmlTextReaderRead(reader)) == 1) {
                name = xmlTextReaderConstName(reader);
                if (name) {
                    if (!xmlStrcmp(name, (const xmlChar *)"username")) {
                        xmlChar *content = xmlTextReaderReadString(reader);
                        current_username = reinterpret_cast<char*>(content);
                        xmlFree(content);
                    } else if (!xmlStrcmp(name, (const xmlChar *)"password")) {
                        xmlChar *content = xmlTextReaderReadString(reader);
                        current_password = reinterpret_cast<char*>(content);
                        xmlFree(content);
                    } else if (!xmlStrcmp(name, (const xmlChar *)"name")) {
                        xmlChar *content = xmlTextReaderReadString(reader);
                        current_name = reinterpret_cast<char*>(content);
                        xmlFree(content);
                    } else if (!xmlStrcmp(name, (const xmlChar *)"citizenship")) {
                        xmlChar *content = xmlTextReaderReadString(reader);
                        current_citizenship = reinterpret_cast<char*>(content);
                        xmlFree(content);
                    } else if (!xmlStrcmp(name, (const xmlChar *)"age")) {
                        xmlChar *content = xmlTextReaderReadString(reader);
                        current_age = reinterpret_cast<char*>(content);
                        xmlFree(content);
                    }
                }
                if (!match && current_username == username && current_password == password) {
                    match = true;
                }
                if (match && !xmlStrcmp(name, (const xmlChar *)"student")) {
                    break;
                }
            }

            if (match) {
                student_info["name"] = current_name;
                student_info["age"] = current_age;
                student_info["citizenship"] = current_citizenship;
                break;
            }
        }
    }

    xmlFreeTextReader(reader);
    xmlCleanupParser();

    return student_info;
}
```
This code snippet uses the `libxml2` library to parse the XML data and extract the required student information based on the provided username and password. The function returns a map containing the student's name, age, and citizenship if a matching student is found; otherwise, it returns an empty map.