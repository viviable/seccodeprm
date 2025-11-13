```cpp

#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml parser
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return student_info;
    }

    // Create XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }

    // Construct XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!result || result->type != XPATH_NODESET) {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }

    // Check if any nodes were found
    if (result->nodesetval && result->nodesetval->nodeNr > 0) {
        xmlNode* node = result->nodesetval->nodeTab[0];
        for (xmlNode* child = node->children; child; child = child->next) {
            if (child->type == XML_ELEMENT_NODE) {
                if (strcmp((const char*)child->name, "name") == 0) {
                    student_info["name"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "age") == 0) {
                    student_info["age"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "citizenship") == 0) {
                    student_info["citizenship"] = (const char*)xmlNodeGetContent(child);
                }
            }
        }
    }

    // Cleanup
    xmlXPathFreeObject(result);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return student_info;
}
```