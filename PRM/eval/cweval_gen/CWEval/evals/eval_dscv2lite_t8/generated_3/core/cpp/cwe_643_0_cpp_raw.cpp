```cpp
#include <iostream>
#include <string>
#include <map>
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

    // Find the student node with matching username and password
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return student_info;
    }

    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "No matching student found" << std::endl;
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info;
    }

    xmlNodeSetPtr nodeset = xpathObj->u.set;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNode* node = nodeset->nodeTab[0];
        if (node->type == XML_ELEMENT_NODE) {
            xmlNodePtr child = node->children;
            while (child) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp((char*)child->name, "name") == 0) {
                        student_info["name"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp((char*)child->name, "age") == 0) {
                        student_info["age"] = (char*)xmlNodeGetContent(child);
                    } else if (strcmp((char*)child->name, "citizenship") == 0) {
                        student_info["citizenship"] = (char*)xmlNodeGetContent(child);
                    }
                }
                child = child->next;
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
This solution uses the libxml2 library to parse the XML data and extract the required student information based on the provided username and password. It constructs an XPath query to find the matching student node and then extracts the relevant details such as name, age, and citizenship. The results are stored in a map and returned.