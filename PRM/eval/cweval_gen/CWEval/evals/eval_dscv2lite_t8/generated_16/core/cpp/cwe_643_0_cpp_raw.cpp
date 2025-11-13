```cpp

#include <iostream>
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    
    // Initialize libxml and parse the XML data
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.length(), "noname.xml", NULL, 0);
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
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!result || !result->nodesetval || result->nodesetval->nodeNr == 0) {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info; // Return empty map if no matching student is found
    }
    
    // Extract student information from the first matching node
    xmlNodeSetPtr nodeset = result->nodesetval;
    xmlNode* node = nodeset->nodeTab[0];
    xmlNode* child_node = node->children;
    
    while (child_node) {
        if (child_node->type == XML_ELEMENT_NODE && child_node->children) {
            std::string tag_name = (char*)child_node->name;
            std::string text_content = (char*)child_node->children->content;
            student_info[tag_name] = text_content;
        }
        child_node = child_node->next;
    }
    
    // Clean up
    xmlXPathFreeObject(result);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    
    return student_info;
}
```