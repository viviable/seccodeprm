```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return student_info; // Return empty map on error
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return student_info; // Return empty map on error
    }

    std::string query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info; // Return empty map on error
    }

    xmlNodeSetPtr nodeset = xpathObj->u.nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNodePtr node = nodeset->nodeTab[0];
        xmlNodePtr child = node->children;

        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                if (strcmp((const char*)child->name, "name") == 0) {
                    student_info["name"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "age") == 0) {
                    student_info["age"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "citizenship") == 0) {
                    student_info["citizenship"] = (const char*)xmlNodeGetContent(child);
                }
            }
            child = child->next;
        }
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return student_info;
}
```