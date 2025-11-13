```cpp

#include <iostream>
#include <map>
#include <string>
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

    // Prepare the XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Construct the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (xpathObj && xpathObj->type == XPATH_NODESET && xpathObj->nodesetval) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        if (nodeset->nodeNr > 0) {
            xmlNode* node = nodeset->nodeTab[0];
            xmlNode* child_node = NULL;

            child_node = node->children;
            while (child_node != NULL) {
                if (child_node->type == XML_ELEMENT_NODE) {
                    if (strcmp((const char*)child_node->name, "name") == 0) {
                        result["name"] = (const char*)xmlNodeGetContent(child_node);
                    } else if (strcmp((const char*)child_node->name, "age") == 0) {
                        result["age"] = (const char*)xmlNodeGetContent(child_node);
                    } else if (strcmp((const char*)child_node->name, "citizenship") == 0) {
                        result["citizenship"] = (const char*)xmlNodeGetContent(child_node);
                    }
                }
                child_node = child_node->next;
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

This code snippet includes the necessary headers for working with XML using libxml2, initializes the parser, constructs an XPath query to find the student matching the given username and password, extracts the required information, and cleans up the resources properly.