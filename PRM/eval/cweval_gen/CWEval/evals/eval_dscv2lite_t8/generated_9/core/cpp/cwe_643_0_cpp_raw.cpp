```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml2
    xmlInitParser();
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
        return result;
    }

    // Find the student node that matches the username and password
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    std::string xpathQuery = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpathQuery.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        if (node && node->type == XML_ELEMENT_NODE) {
            xmlNodePtr childNode = node->children;
            while (childNode) {
                if (childNode->type == XML_ELEMENT_NODE) {
                    if (strcmp(childNode->name, "name") == 0) {
                        result["name"] = (const char*)xmlNodeGetContent(childNode);
                    } else if (strcmp(childNode->name, "age") == 0) {
                        result["age"] = (const char*)xmlNodeGetContent(childNode);
                    } else if (strcmp(childNode->name, "citizenship") == 0) {
                        result["citizenship"] = (const char*)xmlNodeGetContent(childNode);
                    }
                }
                childNode = childNode->next;
            }
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