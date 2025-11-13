```cpp

#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
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

    // Define the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "No matching student found" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        // Extract the student information
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        xmlNode* child_node = nullptr;

        child_node = node->children;
        while (child_node) {
            if (strcmp(child_node->name, "name") == 0) {
                student_info["name"] = (char*)xmlNodeGetContent(child_node);
            } else if (strcmp(child_node->name, "age") == 0) {
                student_info["age"] = (char*)xmlNodeGetContent(child_node);
            } else if (strcmp(child_node->name, "citizenship") == 0) {
                student_info["citizenship"] = (char*)xmlNodeGetContent(child_node);
            }
            child_node = child_node->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return student_info;
}
```