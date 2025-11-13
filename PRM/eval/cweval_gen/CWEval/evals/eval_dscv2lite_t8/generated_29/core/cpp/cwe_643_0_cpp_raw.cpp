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
        std::cerr << "Failed to parse XML" << std::endl;
        return result;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Construct the XPath query
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "XPath evaluation failed or no matching nodes found" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Check if any nodes were found
    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNodePtr node = nodeset->nodeTab[0];
        xmlNodePtr child_node = node->children;

        while (child_node) {
            if (child_node->type == XML_ELEMENT_NODE) {
                std::string tag_name = (char*)child_node->name;
                std::string tag_content = (char*)xmlNodeGetContent(child_node);
                if (tag_name == "name") {
                    result["name"] = tag_content;
                } else if (tag_name == "age") {
                    result["age"] = tag_content;
                } else if (tag_name == "citizenship") {
                    result["citizenship"] = tag_content;
                }
            }
            child_node = child_node->next;
        }
    }

    // Cleanup
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return result;
}
```