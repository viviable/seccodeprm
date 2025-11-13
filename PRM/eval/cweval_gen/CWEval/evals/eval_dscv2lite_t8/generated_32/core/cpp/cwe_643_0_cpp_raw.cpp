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
        return student_info;
    }
    
    // Construct the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info;
    }
    
    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        
        // Extract student information
        xmlNode* child = node->children;
        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                std::string tag_name = (const char*)child->name;
                if (tag_name == "name") {
                    student_info["name"] = (const char*)xmlNodeGetContent(child);
                } else if (tag_name == "age") {
                    student_info["age"] = (const char*)xmlNodeGetContent(child);
                } else if (tag_name == "citizenship") {
                    student_info["citizenship"] = (const char*)xmlNodeGetContent(child);
                }
            }
            child = child->next;
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