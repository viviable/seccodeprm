```cpp
#include <iostream>
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml
    xmlInitParser();
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
        return result;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Define the XPath query
    std::string query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)query.c_str(), xpathCtx);

    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "XPath evaluation failed or no results" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Check if any nodes were found
    xmlNodeSetPtr nodeset = xpathObj->u.nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNode* node = nodeset->nodeTab[0];

        // Extract student information
        xmlNode* child = node->children;
        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                if (strcmp(child->name, "name") == 0) {
                    result["name"] = (char*)xmlNodeGetContent(child);
                } else if (strcmp(child->name, "age") == 0) {
                    result["age"] = (char*)xmlNodeGetContent(child);
                } else if (strcmp(child->name, "citizenship") == 0) {
                    result["citizenship"] = (char*)xmlNodeGetContent(child);
                }
            }
            child = child->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return result;
}
```

This code snippet includes the necessary headers for parsing XML using `libxml2` and implements the `get_student_info` function according to the provided docstring. It uses XPath queries to find the student information based on the given username and password. If the student is found, it extracts and returns their name, age, and citizenship in a map. If not found, it returns an empty map.