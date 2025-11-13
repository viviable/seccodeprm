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
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    xmlDocPtr doc = NULL;
    xmlNodePtr root_element = NULL;

    // Parse the XML data
    doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (doc == NULL) {
        std::cerr << "Failed to parse XML" << std::endl;
        return student_info;
    }

    root_element = xmlDocGetRootElement(doc);
    if (root_element == NULL) {
        std::cerr << "Empty document" << std::endl;
        xmlFreeDoc(doc);
        return student_info;
    }

    // Use XPath query to find the student node
    char xpath_query[256];
    snprintf(xpath_query, sizeof(xpath_query), "//student[username='%s' and password='%s']", username.c_str(), password.c_str());
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        std::cerr << "Error creating XPath context" << std::endl;
        xmlFreeDoc(doc);
        return student_info;
    }

    xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath_query, context);
    if (result == NULL || result->type != XPATH_NODESET) {
        std::cerr << "XPath evaluation failed" << std::endl;
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return student_info;
    }

    xmlNodeSetPtr nodeset = result->nodesetval;
    if (nodeset && nodeset->nodeNr > 0) {
        xmlNodePtr student_node = nodeset->nodeTab[0];

        // Extract student information from the node
        xmlNodePtr child_node = student_node->children;
        while (child_node) {
            if (child_node->type == XML_ELEMENT_NODE) {
                if (strcmp(child_node->name, "name") == 0) {
                    student_info["name"] = (char*)xmlNodeGetContent(child_node);
                } else if (strcmp(child_node->name, "age") == 0) {
                    student_info["age"] = (char*)xmlNodeGetContent(child_node);
                } else if (strcmp(child_node->name, "citizenship") == 0) {
                    student_info["citizenship"] = (char*)xmlNodeGetContent(child_node);
                }
            }
            child_node = child_node->next;
        }
    }

    // Clean up
    xmlXPathFreeObject(result);
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);

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
