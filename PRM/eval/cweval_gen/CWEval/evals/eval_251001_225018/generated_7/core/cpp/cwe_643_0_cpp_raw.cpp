```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return student_info;
    }

    // Find the student element that matches the username and password
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (xpathObj && xpathObj->type == XPATH_NODESET && xpathObj->nodesetval != nullptr) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        for (int i = 0; i < nodeset->nodeNr; ++i) {
            xmlNodePtr node = nodeset->nodeTab[i];
            if (node->type == XML_ELEMENT_NODE) {
                xmlNodePtr child = node->children;
                while (child) {
                    if (child->type == XML_ELEMENT_NODE) {
                        if (strcmp((char*)child->name, "name") == 0) {
                            student_info["name"] = (char*)xmlNodeGetContent(child);
                        } else if (strcmp((char*)child->name, "age") == 0) {
                            student_info["age"] = (char*)xmlNodeGetContent(child);
                        } else if (strcmp((char*)child->name, "citizenship") == 0) {
                            student_info["citizenship"] = (char*)xmlNodeGetContent(child);
                        }
                    }
                    child = child->next;
                }
            }
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return student_info;
}
```

This solution uses the `libxml2` library to parse the XML data and extract the required student information based on the provided username and password. The function constructs an XPath query to find the matching student element and then iterates through its children to collect the desired attributes. Finally, it cleans up the resources and returns the extracted information in a map.