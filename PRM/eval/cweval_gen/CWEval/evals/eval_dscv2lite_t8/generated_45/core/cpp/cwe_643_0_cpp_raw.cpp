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
        std::cerr << "Failed to parse XML data" << std::endl;
        return student_info;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return student_info;
    }

    // Define the XPath expression
    std::string xpath_expr = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_expr.c_str(), xpathCtx);

    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        for (int i = 0; i < nodeset->nodeNr; ++i) {
            xmlNode* node = nodeset->nodeTab[i];
            if (node->type == XML_ELEMENT_NODE) {
                xmlNode* child_node = node->children;
                while (child_node) {
                    if (child_node->type == XML_ELEMENT_NODE) {
                        std::string tag_name(reinterpret_cast<const char*>(child_node->name));
                        if (tag_name == "name") {
                            student_info["name"] = reinterpret_cast<const char*>(child_node->children->content);
                        } else if (tag_name == "age") {
                            student_info["age"] = reinterpret_cast<const char*>(child_node->children->content);
                        } else if (tag_name == "citizenship") {
                            student_info["citizenship"] = reinterpret_cast<const char*>(child_node->children->content);
                        }
                    }
                    child_node = child_node->next;
                }
            }
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return student_info;
}
```