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

    // Prepare XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Construct XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Extract student information
    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNodePtr node = nodeset->nodeTab[0];
        for (node = node->children; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                std::string tag_name(reinterpret_cast<const char*>(node->name));
                if (tag_name == "name") {
                    xmlChar* name_content = xmlNodeGetContent(node);
                    result["name"] = reinterpret_cast<const char*>(name_content);
                    xmlFree(name_content);
                } else if (tag_name == "citizenship") {
                    xmlChar* citizenship_content = xmlNodeGetContent(node);
                    result["citizenship"] = reinterpret_cast<const char*>(citizenship_content);
                    xmlFree(citizenship_content);
                } else if (tag_name == "age") {
                    xmlChar* age_content = xmlNodeGetContent(node);
                    result["age"] = reinterpret_cast<const char*>(age_content);
                    xmlFree(age_content);
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