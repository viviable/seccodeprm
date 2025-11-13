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
        std::cerr << "Failed to parse XML data" << std::endl;
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
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "No matching student found" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }

    // Extract the student information
    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNodePtr node = nodeset->nodeTab[0];
        for (xmlNodePtr child = node->children; child; child = child->next) {
            if (child->type == XML_ELEMENT_NODE) {
                std::string tag_name(reinterpret_cast<const char*>(child->name));
                std::string text(reinterpret_cast<const char*>(xmlNodeGetContent(child)));
                if (tag_name == "name" || tag_name == "age" || tag_name == "citizenship") {
                    student_info[tag_name] = text;
                }
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