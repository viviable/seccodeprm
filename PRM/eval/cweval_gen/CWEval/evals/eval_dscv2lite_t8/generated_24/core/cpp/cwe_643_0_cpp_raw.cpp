```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    
    // Initialize libxml and parse the XML data
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return student_info; // Return empty map if parsing fails
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info; // Return empty map if XPath context creation fails
    }

    // Construct the XPath query
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    // Check if the XPath query returned any nodes
    if (xpathObj && xpathObj->type == XPATH_NODESET && xpathObj->nodesetval->nodeNr > 0) {
        // Get the first node from the result set
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        xmlNode* node = nodeset->nodeTab[0];

        // Extract student information
        student_info["name"] = xmlGetProp(node, (const xmlChar*)"name");
        student_info["age"] = xmlGetProp(node, (const xmlChar*)"age");
        student_info["citizenship"] = xmlGetProp(node, (const xmlChar*)"citizenship");

        // Free resources
        xmlXPathFreeObject(xpathObj);
    }

    // Clean up
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return student_info;
}
```