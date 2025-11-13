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
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    
    if (doc == nullptr) {
        std::cerr << "Failed to parse XML" << std::endl;
        return student_info;
    }
    
    // Create XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == nullptr) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return student_info;
    }
    
    // Define XPath expression
    std::string xpath_expr = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpath_expr.c_str(), xpathCtx);
    
    if (xpathObj == nullptr) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info;
    }
    
    // Check if any nodes were found
    if (xpathObj->type == XPATH_NODESET && xpathObj->u.nodesetval->nodeNr > 0) {
        xmlNodeSetPtr nodeset = xpathObj->u.nodesetval;
        for (int i = 0; i < nodeset->nodeNr; ++i) {
            xmlNodePtr node = nodeset->nodeTab[i];
            xmlNodePtr child = node->children;
            
            while (child != nullptr) {
                if (strcmp(child->name, "name") == 0) {
                    student_info["name"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp(child->name, "age") == 0) {
                    student_info["age"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp(child->name, "citizenship") == 0) {
                    student_info["citizenship"] = (const char*)xmlNodeGetContent(child);
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
    
    return student_info;
}
```