```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return student_info;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return student_info;
    }

    // Prepare the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (xpathObj && xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];

        // Extract student information
        xmlNode* child = node->children;
        while (child != nullptr) {
            if (xmlStrcmp(child->name, (const xmlChar*)"name") == 0) {
                student_info["name"] = (char*)xmlNodeGetContent(child);
            } else if (xmlStrcmp(child->name, (const xmlChar*)"age") == 0) {
                student_info["age"] = (char*)xmlNodeGetContent(child);
            } else if (xmlStrcmp(child->name, (const xmlChar*)"citizenship") == 0) {
                student_info["citizenship"] = (char*)xmlNodeGetContent(child);
            }
            child = child->next;
        }
    }

    // Cleanup
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return student_info;
}
```