```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

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
        fprintf(stderr, "Failed to parse XML\n");
        return NULL;
    }

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        fprintf(stderr, "Failed to create XPath context\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    char xpath_query[1024];
    snprintf(xpath_query, sizeof(xpath_query),
             "//student[username='%s' and password='%s']", username, password);

    xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath_query, context);
    if (result == NULL || result->type != XPATH_NODESET) {
        fprintf(stderr, "No matching student found\n");
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return NULL;
    }

    if (result->nodesetval->nodeNr == 0) {
        fprintf(stderr, "No matching student found\n");
        xmlXPathFreeResult(result);
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodePtr node = result->nodesetval->nodeTab[0]->children;
    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    info->name = NULL;
    info->age = NULL;
    info->citizenship = NULL;

    while (node != NULL) {
        if (strcmp((const char*)node->name, "name") == 0) {
            info->name = strdup((const char*)xmlNodeGetContent(node));
        } else if (strcmp((const char*)node->name, "age") == 0) {
            info->age = strdup((const char*)xmlNodeGetContent(node));
        } else if (strcmp((const char*)node->name, "citizenship") == 0) {
            info->citizenship = strdup((const char*)xmlNodeGetContent(node));
        }
        node = node->next;
    }

    xmlXPathFreeResult(result);
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);
    return info;
}
```