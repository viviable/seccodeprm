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
    xmlLineReaderPtr reader = xmlReaderForMemory(xml_data.c_str(), xml_data.size(), nullptr, nullptr, 0);
    if (!reader) {
        return student_info;
    }

    int ret;
    while ((ret = xmlTextReaderRead(reader)) == 1) {
        int type = xmlTextReaderNodeType(reader);
        if (type == XML_READER_TYPE_ELEMENT && !xmlStrcmp(xmlTextReaderConstName(reader), (const xmlChar*)"student")) {
            bool match = false;
            do {
                ret = xmlTextReaderMoveToNextAttribute(reader);
                if (ret != 1) break;
                if (!xmlStrcmp(xmlTextReaderConstName(reader), (const xmlChar*)"username") &&
                    !xmlStrcmp(xmlTextReaderValue(reader), (const xmlChar*)username.c_str())) {
                    match = true;
                } else if (!xmlStrcmp(xmlTextReaderConstName(reader), (const xmlChar*)"password") &&
                          !xmlStrcmp(xmlTextReaderValue(reader), (const xmlChar*)password.c_str())) {
                    match = true;
                }
            } while (match);

            if (match) {
                xmlTextReaderMoveToElement(reader);
                while ((ret = xmlTextReaderRead(reader)) == 1) {
                    type = xmlTextReaderNodeType(reader);
                    if (type == XML_READER_TYPE_END_ELEMENT && !xmlStrcmp(xmlTextReaderConstName(reader), (const xmlChar*)"student")) {
                        break;
                    } else if (type == XML_READER_TYPE_ELEMENT) {
                        std::string tag_name(reinterpret_cast<const char*>(xmlTextReaderConstName(reader)));
                        std::string tag_value(reinterpret_cast<const char*>(xmlTextReaderValue(reader)));
                        student_info[tag_name] = tag_value;
                    }
                }
            }
        }
    }

    xmlFreeTextReader(reader);
    xmlCleanupParser();

    return student_info;
}
```