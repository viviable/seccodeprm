```go
package main

import (
	"fmt"
	"strings"

	"github.com/ChrisTrenkamp/goxpath"
	"github.com/ChrisTrenkamp/goxpath/tree"
	"github.com/ChrisTrenkamp/goxpath/tree/xmltree"
)

// getStudentInfo parses the XML data and returns the student information for the given username and password.
func getStudentInfo(xmlData, username, password string) (string, string, string) {
	doc, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	query := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)
	result, err := goxpath.Select(doc, query)
	if err != nil {
		fmt.Println("Error executing XPath query:", err)
		return "", "", ""
	}

	if len(result) == 0 {
		return "", "", ""
	}

	nameNode, _ := result[0].Find(".//name")
	citizenshipNode, _ := result[0].Find(".//citizenship")
	ageNode, _ := result[0].Find(".//age")

	var name, citizenship, age string
	if nameNode != nil && nameNode.Value() != "" {
		name = nameNode.Value()
	}
	if citizenshipNode != nil && citizenshipNode.Value() != "" {
		citizenship = citizenshipNode.Value()
	}
	if ageNode != nil && ageNode.Value() != "" {
		age = ageNode.Value()
	}

	return name, age, citizenship
}
```