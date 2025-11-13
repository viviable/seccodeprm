To parse the XML data and extract the required student information using XPath queries, you need to use an XML parsing library. One popular choice is `libxml2`, which provides comprehensive support for XML processing. Below is a complete implementation of the `get_student_info` function using `libxml2`.

First, ensure you have `libxml2` installed on your system. You can install it using your package manager. For example, on Ubuntu, you can use:

```sh
sudo apt-get install libxml2-dev
```

Here is the complete implementation:

```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml2 and parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML data" << std::endl;
        return result;
    }

    // Find the student node with matching username and password
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return result;
    }

    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return result;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNode* node = xpathObj->nodesetval->nodeTab[0];
        if (node) {
            // Extract student information
            xmlNode* child = node->children;
            while (child) {
                if (child->type == XML_ELEMENT_NODE) {
                    if (strcmp((const char*)child->name, "name") == 0) {
                        result["name"] = (const char*)xmlNodeGetContent(child);
                    } else if (strcmp((const char*)child->name, "age") == 0) {
                        result["age"] = (const char*)xmlNodeGetContent(child);
                    } else if (strcmp((const char*)child->name, "citizenship") == 0) {
                        result["citizenship"] = (const char*)xmlNodeGetContent(child);
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

    return result;
}
```

This implementation uses `libxml2` to parse the XML data and evaluate an XPath query to find the student node that matches the given username and password. If a matching student is found, it extracts the name, age, and citizenship information and stores it in a map. Finally, it cleans up the resources used by `libxml2`.