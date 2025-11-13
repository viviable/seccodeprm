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
        return student_info;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }

    // Construct the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (xpathObj && xpathObj->type == XPATH_NODESET) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        if (nodeset && nodeset->nodeNr > 0) {
            xmlNodePtr node = nodeset->nodeTab[0];
            xmlNodePtr child = node->children;
            while (child != NULL) {
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
    xmlCleanupParser();

    return student_info;
}
```
This solution uses the `libxml2` library to parse the XML data and extract the required student information based on the provided username and password. The function constructs an XPath query to find the matching student node and then extracts the necessary details such as name, age, and citizenship. The result is returned as a map.