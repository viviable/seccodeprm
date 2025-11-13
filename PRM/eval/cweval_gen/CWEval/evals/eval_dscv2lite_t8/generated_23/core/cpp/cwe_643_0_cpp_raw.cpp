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
        return result; // Return empty map on error
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return result; // Return empty map on error
    }

    // Prepare the XPath expression
    std::string xpath_expr = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_expr.c_str(), xpathCtx);

    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return result; // Return empty map on error
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        // Extract student information
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        xmlNode* node = nodeset->nodeTab[0];
        xmlNode* child_node = node->children;

        while (child_node) {
            if (child_node->type == XML_ELEMENT_NODE) {
                if (strcmp((const char*)child_node->name, "name") == 0) {
                    result["name"] = (const char*)xmlNodeGetContent(child_node);
                } else if (strcmp((const char*)child_node->name, "age") == 0) {
                    result["age"] = (const char*)xmlNodeGetContent(child_node);
                } else if (strcmp((const char*)child_node->name, "citizenship") == 0) {
                    result["citizenship"] = (const char*)xmlNodeGetContent(child_node);
                }
            }
            child_node = child_node->next;
        }
    }

    // Cleanup
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return result;
}
```