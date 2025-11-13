```cpp
#include <iostream>
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml and parse the XML data
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return result;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Construct the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpath_query.c_str(), xpathCtx);

    if (xpathObj && xpathObj->type == XPATH_NODESET && xpathObj->nodesetval != nullptr) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        if (nodeset->nodeNr > 0) {
            xmlNodePtr node = nodeset->nodeTab[0];
            xmlNodePtr child = node->children;
            while (child) {
                if (child->type == XML_ELEMENT_NODE) {
                    std::string tag_name(reinterpret_cast<const char*>(child->name));
                    if (tag_name == "name" || tag_name == "citizenship" || tag_name == "age") {
                        xmlChar* content = xmlNodeGetContent(child);
                        result[tag_name] = reinterpret_cast<const char*>(content);
                        xmlFree(content);
                    }
                }
                child = child->next;
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