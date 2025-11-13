```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml2 library
    xmlInitParser();

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return student_info; // Return empty map on error
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return student_info; // Return empty map on error
    }

    // Construct the XPath query
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info; // Return empty map on error
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        xmlNodePtr node = nodeset->nodeTab[0];

        // Extract student information
        xmlNodePtr child = node->children;
        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                std::string tag_name = (const char*)child->name;
                if (tag_name == "name") {
                    student_info["name"] = (const char*)xmlNodeGetContent(child);
                } else if (tag_name == "age") {
                    student_info["age"] = (const char*)xmlNodeGetContent(child);
                } else if (tag_name == "citizenship") {
                    student_info["citizenship"] = (const char*)xmlNodeGetContent(child);
                }
            }
            child = child->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return student_info;
}
```