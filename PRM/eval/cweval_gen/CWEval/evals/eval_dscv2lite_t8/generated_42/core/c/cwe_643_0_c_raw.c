```c
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    char* age;
    char* citizenship;
} StudentInfo;

static void free_student_info(StudentInfo* info) {
    if (info != NULL) {
        free(info->name);
        free(info->age);
        free(info->citizenship);
        free(info);
    }
}

StudentInfo* get_student_info(const char* xml_data, const char* username, const char* password) {
    xmlDocPtr doc = xmlReadMemory(xml_data, strlen(xml_data), "noname.xml", NULL, 0);
    if (doc == NULL) {
        return NULL;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL) {
        xmlFreeDoc(doc);
        return NULL;
    }

    // Construct XPath queries
    char* usernameQuery = (char*)malloc(100 * sizeof(char));
    sprintf(usernameQuery, "//student[username='%s']/name/text()", username);
    char* passwordQuery = (char*)malloc(100 * sizeof(char));
    sprintf(passwordQuery, "//student[password='%s']/name/text()", password);

    xmlXPathObjectPtr xpathObjUsername = xmlXPathEvalExpression((xmlChar*)usernameQuery, xpathCtx);
    xmlXPathObjectPtr xpathObjPassword = xmlXPathEvalExpression((xmlChar*)passwordQuery, xpathCtx);

    if (xpathObjUsername == NULL || xpathObjPassword == NULL ||
        xpathObjUsername->nodesetval->nodeNr == 0 || xpathObjPassword->nodesetval->nodeNr == 0) {
        free(xpathQuery);
        free(usernameQuery);
        xmlXPathFreeObject(xpathObjUsername);
        xmlXPathFreeObject(xpathObjPassword);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodeSetPtr nodeset = xpathObjUsername->nodesetval;
    xmlNodePtr node = nodeset->nodeTab[0];
    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    info->name = strdup((const char*)xmlNodeGetContent(node));

    // Check if the password matches
    node = xpathObjPassword->nodesetval->nodeTab[0];
    if (strcmp((const char*)xmlNodeGetContent(node), info->name) != 0) {
        free_student_info(info);
        free(xpathQuery);
        free(usernameQuery);
        xmlXPathFreeObject(xpathObjUsername);
        xmlXPathFreeObject(xpathObjPassword);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    // Get other details
    xmlXPathFreeObject(xpathObjUsername);
    xmlXPathFreeObject(xpathObjPassword);
    free(xpathQuery);
    free(usernameQuery);

    usernameQuery = (char*)malloc(100 * sizeof(char));
    sprintf(usernameQuery, "//student[username='%s']/citizenship/text()", username);
    passwordQuery = (char*)malloc(100 * sizeof(char));
    sprintf(passwordQuery, "//student[password='%s']/age/text()", password);

    xpathObjUsername = xmlXPathEvalExpression((xmlChar*)usernameQuery, xpathCtx);
    xpathObjPassword = xmlXPathEvalExpression((xmlChar*)passwordQuery, xpathCtx);

    if (xpathObjUsername == NULL || xpathObjPassword == NULL ||
        xpathObjUsername->nodesetval->nodeNr == 0 || xpathObjPassword->nodesetval->nodeNr == 0) {
        free_student_info(info);
        free(xpathQuery);
        free(usernameQuery);
        xmlXPathFreeObject(xpathObjUsername);
        xmlXPathFreeObject(xpathObjPassword);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    nodeset = xpathObjUsername->nodesetval;
    node = nodeset->nodeTab[0];
    info->citizenship = strdup((const char*)xmlNodeGetContent(node));

    nodeset = xpathObjPassword->nodesetval;
    node = nodeset->nodeTab[0];
    info->age = strdup((const char*)xmlNodeGetContent(node));

    free(xpathQuery);
    free(usernameQuery);
    xmlXPathFreeObject(xpathObjUsername);
    xmlXPathFreeObject(xpathObjPassword);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```