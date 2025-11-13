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
    LIBXML_TEST_VERSION

    // Parse the XML data
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
    std::string xpathQuery = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpathQuery.c_str(), xpathCtx);
    if (!xpathObj || !xpathObj->nodesetval || xpathObj->nodesetval->nodeNr == 0) {
        std::cerr << "Student not found" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Extract the student information
    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    for (int i = 0; i < nodeset->nodeNr; ++i) {
        xmlNodePtr node = nodeset->nodeTab[i];
        if (node->type == XML_ELEMENT_NODE && node->children != nullptr) {
            xmlNodePtr child = node->children;
            while (child != nullptr) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp(child->name, "name") == 0) {
                        result["name"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp(child->name, "age") == 0) {
                        result["age"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp(child->name, "citizenship") == 0) {
                        result["citizenship"] = (char*)xmlNodeGetContent(child);
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