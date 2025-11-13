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
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return result;
    }

    // XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // XPath queries
    std::string query1 = "//student[username/text()='" + username + "']";
    std::string query2 = "//student[password/text()='" + password + "']";

    // Evaluate XPath expressions
    xmlXPathObjectPtr nodeset1 = xmlXPathEvalExpression(BAD_CAST query1.c_str(), xpathCtx);
    xmlXPathObjectPtr nodeset2 = xmlXPathEvalExpression(BAD_CAST query2.c_str(), xpathCtx);

    if (nodeset1 && nodeset1->nodeSet && nodeset1->nodeSet->nodeTab[0] &&
        nodeset2 && nodeset2->nodeSet && nodeset2->nodeSet->nodeTab[0]) {
        xmlNode* node = nodeset1->nodeSet->nodeTab[0]->children;
        while (node) {
            if (xmlStrcmp(node->name, BAD_CAST "name") == 0) {
                result["name"] = (const char*)xmlNodeGetContent(node);
            } else if (xmlStrcmp(node->name, BAD_CAST "age") == 0) {
                result["age"] = (const char*)xmlNodeGetContent(node);
            } else if (xmlStrcmp(node->name, BAD_CAST "citizenship") == 0) {
                result["citizenship"] = (const char*)xmlNodeGetContent(node);
            }
            node = node->next;
        }
    }

    // Cleanup
    xmlXPathFreeObject(nodeset1);
    xmlXPathFreeObject(nodeset2);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return result;
}
```