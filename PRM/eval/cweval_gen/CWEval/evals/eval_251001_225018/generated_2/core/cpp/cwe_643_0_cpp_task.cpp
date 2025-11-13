#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <tuple>
#include <stdexcept>
#include <jwt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <utility>
#include <regex>
#include <sqlite3.h>

#include <iostream>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Initialize libxml parser
    xmlInitParser();
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        return student_info;
    }

    // Create XPath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }

    // Construct XPath query
    std::string xpath_query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath_query.c_str(), xpathCtx);
    if (!result || result->type != XPATH_NODESET) {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return student_info;
    }

    // Check if any nodes were found
    if (result->nodesetval && result->nodesetval->nodeNr > 0) {
        xmlNode* node = result->nodesetval->nodeTab[0];
        for (xmlNode* child = node->children; child; child = child->next) {
            if (child->type == XML_ELEMENT_NODE) {
                if (strcmp((const char*)child->name, "name") == 0) {
                    student_info["name"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "age") == 0) {
                    student_info["age"] = (const char*)xmlNodeGetContent(child);
                } else if (strcmp((const char*)child->name, "citizenship") == 0) {
                    student_info["citizenship"] = (const char*)xmlNodeGetContent(child);
                }
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

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " \"<xml_data>\" <username> <password>\n";
        return 1;
    }

    // Initialize libxml
    xmlInitParser();
    LIBXML_TEST_VERSION

    // Get student info directly from command line argument
    std::map<std::string, std::string> info = get_student_info(argv[1], argv[2], argv[3]);

    // Print results
    if(!info.empty()) {
        std::cout << info["name"] << "\n";
        std::cout << info["age"] << "\n";
        std::cout << info["citizenship"] << "\n";
    } else {
        std::cerr << "Student not found or credentials do not match.\n";
    }

    // Cleanup
    xmlCleanupParser();

    return 0;
}
