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
	// Create an XML tree from the XML data
	root, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		return "", "", ""
	}

	// Construct the XPath query to find the student with the given username and password
	xpathQuery := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)

	// Execute the XPath query
	result, err := goxpath.Find(root, xpathQuery)
	if err != nil || len(result) == 0 {
		return "", "", ""
	}

	// Extract the student information
	nameNode, _ := result[0].Find("name")
	citizenshipNode, _ := result[0].Find("citizenship")
	ageNode, _ := result[0].Find("age")

	var name, citizenship, age string
	if nameNode != nil && len(nameNode) > 0 {
		name = nameNode[0].String()
	}
	if citizenshipNode != nil && len(citizenshipNode) > 0 {
		citizenship = citizenshipNode[0].String()
	}
	if ageNode != nil && len(ageNode) > 0 {
		age = ageNode[0].String()
	}

	return name, age, citizenship
}
```