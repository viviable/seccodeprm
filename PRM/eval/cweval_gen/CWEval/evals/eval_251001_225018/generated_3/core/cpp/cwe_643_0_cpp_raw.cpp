```cpp
#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml and parse the XML data
    LIBXML_TEST_VERSION
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return result;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        LIBXML_CLEAR_ERRORS
        return result;
    }

    // Define the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (xpathObj && xpathObj->type == XPATH_NODESET && xpathObj->nodesetval->nodeNr > 0) {
        // Extract the student information
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        xmlNodePtr node = nodeset->nodeTab[0];
        xmlNodePtr child = node->children;

        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                if (strcmp((char*)child->name, "name") == 0) {
                    result["name"] = (char*)xmlNodeGetContent(child);
                } else if (strcmp((char*)child->name, "age") == 0) {
                    result["age"] = (char*)xmlNodeGetContent(child);
                } else if (strcmp((char*)child->name, "citizenship") == 0) {
                    result["citizenship"] = (char*)xmlNodeGetContent(child);
                }
            }
            child = child->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    LIBXML_CLEAR_ERRORS

    return result;
}
```

This code snippet includes the necessary headers and uses the `libxml` library to parse the XML data and extract the student information based on the provided username and password. The function returns a map containing the student's name, age, and citizenship if the credentials match any student in the XML data. If no matching student is found, an empty map is returned.