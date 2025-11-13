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
        std::cerr << "Failed to parse XML data" << std::endl;
        return result;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return result;
    }

    // Construct the XPath query
    std::string xpathQuery = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpathQuery.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return result;
    }

    // Check if any nodes were found
    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNode* node = nodeset->nodeTab[0];
        xmlNode* child = node->children;
        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                std::string tagName = (char*)child->name;
                if (tagName == "name") {
                    result["name"] = (char*)xmlNodeGetContent(child);
                } else if (tagName == "age") {
                    result["age"] = (char*)xmlNodeGetContent(child);
                } else if (tagName == "citizenship") {
                    result["citizenship"] = (char*)xmlNodeGetContent(child);
                }
            }
            child = child->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return result;
}
```