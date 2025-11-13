```cpp
#include <iostream>
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    
    // Initialize libxml parser
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
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

    // Construct the XPath query
    std::string xpath_query = "//student[username='" + username + "' and password='" + password + "']";
    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);

    if (result && result->type == XPATH_NODESET && result->nodesetval) {
        if (result->nodesetval->nodeNr > 0) {
            xmlNode* node = result->nodesetval->nodeTab[0];
            student_info["name"] = (char*)xmlGetProp(node, (const xmlChar*)"name");
            student_info["citizenship"] = (char*)xmlGetProp(node, (const xmlChar*)"citizenship");
            char* age_char = (char*)xmlGetProp(node, (const xmlChar*)"age");
            if (age_char) {
                student_info["age"] = age_char;
                free(age_char); // Free the memory allocated for age
            }
        }
    }

    // Cleanup
    xmlXPathFreeObject(result);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return student_info;
}
```