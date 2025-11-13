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
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

namespace rapidxml {
    template<class Ch>
    void print(std::ostream &out, const xml_node<Ch> *node, int indent = 0);
}

using namespace rapidxml;

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;
    
    try {
        // Parse the XML data
        xml_document<> doc;
        doc.parse<0>(const_cast<char*>(xml_data.c_str()));
        
        // Find the student node that matches the given username and password
        xml_node<>* students_node = doc.first_node("students");
        if (students_node != nullptr) {
            for (xml_node<>* student_node = students_node->first_node("student"); student_node != nullptr; student_node = student_node->next_sibling()) {
                xml_node<>* username_node = student_node->first_node("username");
                xml_node<>* password_node = student_node->first_node("password");
                
                if (username_node != nullptr && password_node != nullptr) {
                    std::string u = username_node->value();
                    std::string p = password_node->value();
                    
                    if (u == username && p == password) {
                        // Extract other student information
                        xml_node<>* name_node = student_node->first_node("name");
                        xml_node<>* citizenship_node = student_node->first_node("citizenship");
                        xml_node<>* age_node = student_node->first_node("age");
                        
                        if (name_node != nullptr) student_info["name"] = name_node->value();
                        if (citizenship_node != nullptr) student_info["citizenship"] = citizenship_node->value();
                        if (age_node != nullptr) student_info["age"] = age_node->value();
                        
                        break; // Found the student, no need to continue searching
                    }
                }
            }
        }
    } catch (const rapidxml::parse_error& e) {
        std::cerr << "XML parsing error: " << e.what() << std::endl;
    }
    
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
