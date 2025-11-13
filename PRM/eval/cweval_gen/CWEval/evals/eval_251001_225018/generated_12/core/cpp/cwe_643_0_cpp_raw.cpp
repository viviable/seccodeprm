```cpp

#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml2
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
        return student_info;
    }

    // Prepare XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return student_info;
    }

    // Define the XPath query
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (xpathObj && xpathObj->type == XPATH_NODESET) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        if (nodeset && nodeset->nodeNr > 0) {
            xmlNode* node = nodeset->nodeTab[0];
            xmlNode* child = node->children;

            while (child) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp((char*)child->name, "name") == 0) {
                        student_info["name"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp((char*)child->name, "age") == 0) {
                        student_info["age"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp((char*)child->name, "citizenship") == 0) {
                        student_info["citizenship"] = (char*)xmlNodeGetContent(child);
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

    return student_info;
}
```