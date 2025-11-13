```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml parser
    LIBXML_TEST_VERSION

    // Parse the XML document
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
        return result;
    }

    // Find the root element
    xmlNodePtr root_element = xmlDocGetRootElement(doc);
    if (root_element == nullptr || xmlStrcmp(root_element->name, (const xmlChar*)"students") != 0) {
        std::cerr << "Invalid XML structure" << std::endl;
        xmlFreeDoc(doc);
        return result;
    }

    // Iterate over each student node
    for (xmlNodePtr node = root_element->children; node != nullptr; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar*)"student") == 0) {
            xmlChar* user_name = xmlGetProp(node, (const xmlChar*)"username");
            xmlChar* pass_word = xmlGetProp(node, (const xmlChar*)"password");

            if (user_name && pass_word && 
                strcmp((char*)user_name, username.c_str()) == 0 &&
                strcmp((char*)pass_word, password.c_str()) == 0) {

                xmlNodePtr child_node = node->children;
                while (child_node != nullptr) {
                    if (child_node->type == XML_ELEMENT_NODE) {
                        if (xmlStrcmp(child_node->name, (const xmlChar*)"name") == 0) {
                            xmlChar* name = xmlNodeListGetString(doc, child_node->children, 1);
                            result["name"] = (char*)name;
                            xmlFree(name);
                        } else if (xmlStrcmp(child_node->name, (const xmlChar*)"age") == 0) {
                            xmlChar* age = xmlNodeListGetString(doc, child_node->children, 1);
                            result["age"] = (char*)age;
                            xmlFree(age);
                        } else if (xmlStrcmp(child_node->name, (const xmlChar*)"citizenship") == 0) {
                            xmlChar* citizenship = xmlNodeListGetString(doc, child_node->children, 1);
                            result["citizenship"] = (char*)citizenship;
                            xmlFree(citizenship);
                        }
                    }
                    child_node = child_node->next;
                }

                break;
            }

            xmlFree(user_name);
            xmlFree(pass_word);
        }
    }

    // Cleanup
    xmlFreeDoc(doc);

    return result;
}
```

This solution uses the `libxml2` library to parse the XML data and extract the required student information based on the provided username and password. The function returns a map containing the student's name, age, and citizenship if the credentials match, otherwise, it returns an empty map.