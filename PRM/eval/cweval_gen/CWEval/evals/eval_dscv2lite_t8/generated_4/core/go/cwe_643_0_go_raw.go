```go
package main

import (
	"fmt"
	"strings"

	"github.com/ChrisTrenkamp/goxpath"
	"github.com/ChrisTrenkamp/goxpath/tree"
	"github.com/ChrisTrenkamp/goxpath/tree/xmltree"
)

// Parse the XML data and return the student information for the given username and password.
func getStudentInfo(xmlData, username, password string) (string, string, string) {
	doc, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	query := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)
	result, err := goxpath.Query(doc, query)
	if err != nil {
		fmt.Println("Error executing XPath query:", err)
		return "", "", ""
	}

	var name, age, citizenship string
	for _, node := range result {
		switch node.Data {
		case "name":
			name = node.InnerText()
		case "age":
			age = node.InnerText()
		case "citizenship":
			citizenship = node.InnerText()
		}
	}

	return name, age, citizenship
}
```