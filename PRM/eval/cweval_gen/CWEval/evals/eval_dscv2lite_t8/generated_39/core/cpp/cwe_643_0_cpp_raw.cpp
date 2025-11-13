```cpp

#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml
    xmlInitParser();
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML data." << std::endl;
        return result;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context." << std::endl;
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Construct the XPath query
    std::string xpathQuery = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpathQuery.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression." << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return result;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        if (node && node->children) {
            for (xmlNode* child = node->children; child; child = child->next) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp((const char*)child->name, "name") == 0) {
                        result["name"] = (const char*)xmlNodeGetContent(child);
                    } else if (strcmp((const char*)child->name, "age") == 0) {
                        result["age"] = (const char*)xmlNodeGetContent(child);
                    } else if (strcmp((const char*)child->name, "citizenship") == 0) {
                        result["citizenship"] = (const char*)xmlNodeGetContent(child);
                    }
                }
            }
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return result;
}
```
This implementation uses the libxml2 library to parse the XML data and evaluate XPath expressions to find the student information based on the provided username and password. If a matching student is found, their name, age, and citizenship are extracted and returned in a map. If no matching student is found, an empty map is returned.