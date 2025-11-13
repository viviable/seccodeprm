```cpp

#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> result;

    // Initialize libxml
    LIBXML_TEST_VERSION

    // Parse the XML data
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
        return result;
    }

    // Find the student node matching the username and password
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        std::cerr << "Failed to create XPath context" << std::endl;
        xmlFreeDoc(doc);
        return result;
    }

    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        std::cerr << "XPath evaluation failed" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return result;
    }

    // Check if any nodes were found
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNodeSetPtr nodeset = xpathObj->nodesetval;
        for (int i = 0; i < nodeset->nodeNr; ++i) {
            xmlNodePtr node = nodeset->nodeTab[i];
            xmlNodePtr child = node->children;
            while (child) {
                if (child->type == XML_ELEMENT_NODE) {
                    std::string tag_name(reinterpret_cast<char*>(child->name));
                    if (tag_name == "name") {
                        result["name"] = reinterpret_cast<char*>(xmlNodeGetContent(child));
                    } else if (tag_name == "age") {
                        result["age"] = reinterpret_cast<char*>(xmlNodeGetContent(child));
                    } else if (tag_name == "citizenship") {
                        result["citizenship"] = reinterpret_cast<char*>(xmlNodeGetContent(child));
                    }
                }
                child = child->next;
            }
        }
    }

    // Cleanup
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return result;
}
```