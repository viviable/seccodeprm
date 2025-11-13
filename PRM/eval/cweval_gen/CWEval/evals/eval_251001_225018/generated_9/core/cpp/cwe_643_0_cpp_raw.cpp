```cpp

#include <iostream>
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml
    LIBXML_TEST_VERSION

    // Parse the XML document
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
        return result;
    }

    // Construct the XPath query
    std::string query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "XPath evaluation failed" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return result;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        xmlNode* child = node->children;
        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                if (strcmp((const char*)child->name, "name") == 0) {
                    result["name"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "age") == 0) {
                    result["age"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "citizenship") == 0) {
                    result["citizenship"] = (const char*)xmlNodeGetContent(child);
                }
            }
            child = child->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return result;
}
```