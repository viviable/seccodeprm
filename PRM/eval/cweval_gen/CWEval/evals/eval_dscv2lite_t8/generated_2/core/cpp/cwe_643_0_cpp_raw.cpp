```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return result;
    }

    // Find the student node with the matching username and password
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return result;
    }

    std::string query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return result;
    }

    xmlNodeSetPtr nodeset = xpathObj->u.set;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNodePtr node = nodeset->nodeTab[0];
        for (xmlNodePtr child = node->children; child; child = child->next) {
            if (child->type == XML_ELEMENT_NODE) {
                if (strcmp((const char*)child->name, "name") == 0) {
                    result["name"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "age") == 0) {
                    result["age"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "citizenship") == 0) {
                    result["citizenship"] = (const char*)xmlNodeGetContent(child);
                }
            }
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return result;
}
```

This solution includes the necessary headers for parsing XML using `libxml2`. It initializes the library, parses the provided XML data, constructs an XPath query to find the student node with the specified username and password, extracts the required information, and cleans up resources before returning the result.