```cpp

#include <iostream>
#include <string>
#include <map>
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
        std::cerr << "Failed to parse XML data." << std::endl;
        return student_info;
    }
    
    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context." << std::endl;
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }
    
    // Construct the XPath query
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "XPath evaluation failed." << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }
    
    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        
        // Extract student information
        xmlNode* child_node = node->children;
        while (child_node) {
            if (child_node->type == XML_ELEMENT_NODE) {
                std::string tag_name(reinterpret_cast<const char*>(child_node->name));
                std::string text_content(reinterpret_cast<const char*>(xmlNodeGetContent(child_node)));
                
                if (tag_name == "name" || tag_name == "citizenship" || tag_name == "age") {
                    student_info[tag_name] = text_content;
                }
            }
            child_node = child_node->next;
        }
    } else {
        std::cerr << "No student found with the given username and password." << std::endl;
    }
    
    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    
    return student_info;
}
```