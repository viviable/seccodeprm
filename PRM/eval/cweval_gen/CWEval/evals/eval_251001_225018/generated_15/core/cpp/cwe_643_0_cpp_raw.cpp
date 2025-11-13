```cpp

#include <iostream>
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml
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
        return result;
    }

    // Construct the XPath query
    std::string query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return result;
    }

    // Check if any nodes were found
    xmlNodeSetPtr nodeset = xpathObj->u.nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        // Extract student information
        xmlNodePtr node = nodeset->nodeTab[0];
        xmlNodePtr child = node->children;
        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                std::string tag_name = (const char*)child->name;
                std::string text = (const char*)xmlNodeGetContent(child);
                if (tag_name == "name" || tag_name == "citizenship" || tag_name == "age") {
                    result[tag_name] = text;
                }
            }
            child = child->next;
        }
    }

    // Cleanup
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return result;
}
```
This code uses the `libxml2` library to parse the XML data and extract the student information based on the provided username and password. It constructs an XPath query to find the matching student element and then extracts the required information from its children. The result is returned as a map containing the name, age, and citizenship of the student.