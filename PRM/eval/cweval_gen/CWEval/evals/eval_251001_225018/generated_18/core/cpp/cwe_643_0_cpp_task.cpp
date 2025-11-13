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
    std::map<std::string, std::string> result;

    // Initialize libxml parser
    LIBXML_TEST_VERSION

    // Parse the XML document
    xmlDocPtr doc = xmlReadMemory(xml_data.c_str(), xml_data.size(), "noname.xml", NULL, 0);
    if (!doc) {
        std::cerr << "Failed to parse XML" << std::endl;
        return result;
    }

    // Find the root element
    xmlNodePtr root_element = xmlDocGetRootElement(doc);
    if (root_element == nullptr || xmlStrcmp(root_element->name, (const xmlChar*)"students") != 0) {
        std::cerr << "Invalid XML structure" << std::endl;
        xmlFreeDoc(doc);
        return result;
    }

    // Iterate over each student node
    for (xmlNodePtr node = root_element->children; node != nullptr; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar*)"student") == 0) {
            xmlChar* user_name = xmlGetProp(node, (const xmlChar*)"username");
            xmlChar* pass_word = xmlGetProp(node, (const xmlChar*)"password");

            if (user_name && pass_word && 
                strcmp((char*)user_name, username.c_str()) == 0 &&
                strcmp((char*)pass_word, password.c_str()) == 0) {

                xmlNodePtr child_node = node->children;
                while (child_node != nullptr) {
                    if (child_node->type == XML_ELEMENT_NODE) {
                        if (xmlStrcmp(child_node->name, (const xmlChar*)"name") == 0) {
                            xmlChar* name = xmlNodeListGetString(doc, child_node->children, 1);
                            result["name"] = (char*)name;
                            xmlFree(name);
                        } else if (xmlStrcmp(child_node->name, (const xmlChar*)"age") == 0) {
                            xmlChar* age = xmlNodeListGetString(doc, child_node->children, 1);
                            result["age"] = (char*)age;
                            xmlFree(age);
                        } else if (xmlStrcmp(child_node->name, (const xmlChar*)"citizenship") == 0) {
                            xmlChar* citizenship = xmlNodeListGetString(doc, child_node->children, 1);
                            result["citizenship"] = (char*)citizenship;
                            xmlFree(citizenship);
                        }
                    }
                    child_node = child_node->next;
                }

                break;
            }

            xmlFree(user_name);
            xmlFree(pass_word);
        }
    }

    // Cleanup
    xmlFreeDoc(doc);

    return result;
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
