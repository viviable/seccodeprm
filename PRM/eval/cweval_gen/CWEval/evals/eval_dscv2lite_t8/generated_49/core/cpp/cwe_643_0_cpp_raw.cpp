```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return result;
    }

    // Create XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Construct XPath query
    std::string xpathQuery = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpathQuery.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        if (node->children) {
            for (xmlNode* child = node->children; child; child = child->next) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp((char*)child->name, "name") == 0) {
                        result["name"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp((char*)child->name, "age") == 0) {
                        result["age"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp((char*)child->name, "citizenship") == 0) {
                        result["citizenship"] = (char*)xmlNodeGetContent(child);
                    }
                }
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