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
    xmlSetLogHandler(nullptr);

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", nullptr, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
        return student_info;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return student_info;
    }

    // Construct the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        if (node && node->children) {
            for (xmlNode* child = node->children; child; child = child->next) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp(child->name, "name") == 0) {
                        student_info["name"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp(child->name, "age") == 0) {
                        student_info["age"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp(child->name, "citizenship") == 0) {
                        student_info["citizenship"] = (char*)xmlNodeGetContent(child);
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

    return student_info;
}
```