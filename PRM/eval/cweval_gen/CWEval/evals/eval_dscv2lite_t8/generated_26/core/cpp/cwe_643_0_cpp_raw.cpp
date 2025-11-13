```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml
    xmlInitParser();
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
        xmlCleanupParser();
        return student_info;
    }

    // Define the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpath_query.c_str(), xpathCtx);

    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "No matching student found" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        xmlNodePtr node = nodeset->nodeTab[0];

        // Extract student information
        student_info["name"] = std::string((char*)xmlGetProp(node, BAD_CAST "name"));
        student_info["age"] = std::string((char*)xmlGetProp(node, BAD_CAST "age"));
        student_info["citizenship"] = std::string((char*)xmlGetProp(node, BAD_CAST "citizenship"));
    } else {
        std::cerr << "No matching student found" << std::endl;
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return student_info;
}
```