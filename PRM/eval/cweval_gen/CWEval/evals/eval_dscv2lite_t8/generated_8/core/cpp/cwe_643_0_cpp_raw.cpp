```cpp

#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize the libxml library
    xmlInitParser();

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), nullptr, nullptr, 0);
    if (!doc) {
        return student_info; // Return empty map on parse error
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return student_info; // Return empty map on context creation failure
    }

    // Define the XPath query
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    // Check if the XPath query returned any nodes
    if (xpathObj && xpathObj->type == XPATH_NODESET && xpathObj->nodesetval->nodeNr > 0) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        for (int i = 0; i < nodeset->nodeNr; ++i) {
            xmlNode* node = nodeset->nodeTab[i];
            if (node->type == XML_ELEMENT_NODE) {
                xmlNode* child = node->children;
                while (child != nullptr) {
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