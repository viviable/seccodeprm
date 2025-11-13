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

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
        BAD_CAST "//student[username/text()='" BAD_CAST username BAD_CAST "' and password/text()='" BAD_CAST password BAD_CAST "']",
        xpathCtx
    );

    if (xpathObj == NULL || xpathObj->type != XPATH_NODESET) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodeSetPtr nodeset = xpathObj->u.nodesetval;
    if (nodeset == NULL || nodeset->nodeNr == 0) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodePtr node = nodeset->nodeTab[0];
    xmlChar* name = xmlGetProp(node, BAD_CAST "name");
    xmlChar* age = xmlGetProp(node, BAD_CAST "age");
    xmlChar* citizenship = xmlGetProp(node, BAD_CAST "citizenship");

    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    if (info != NULL) {
        info->name = strdup((const char*)name);
        info->age = strdup((const char*)age);
        info->citizenship = strdup((const char*)citizenship);
    } else {
        free(name);
        free(age);
        free(citizenship);
    }

    xmlFree(name);
    xmlFree(age);
    xmlFree(citizenship);

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```