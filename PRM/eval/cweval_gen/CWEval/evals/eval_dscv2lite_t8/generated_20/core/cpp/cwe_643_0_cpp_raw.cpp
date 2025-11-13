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
        return student_info;
    }

    // Create an XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return student_info;
    }

    // Prepare the XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return student_info;
    }

    // Check if any nodes were found
    xmlNodeSetPtr nodeset = xpathObj->u.nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNodePtr node = nodeset->nodeTab[0];

        // Extract student information
        xmlNodePtr child = node->children;
        while (child) {
            if (child->type == XML_ELEMENT_NODE) {
                std::string tag_name(reinterpret_cast<const char*>(child->name));
                std::string text_content(reinterpret_cast<const char*>(xmlNodeGetContent(child)));
                if (tag_name == "name" || tag_name == "citizenship" || tag_name == "age") {
                    student_info[tag_name] = text_content;
                }
            }
            child = child->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return student_info;
}
```
This solution uses the `libxml2` library to parse the XML data and evaluate XPath queries. It searches for a student with the specified username and password, extracts their name, citizenship, and age, and returns this information in a map. If no matching student is found, an empty map is returned.