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
	// Convert the XML data into a tree structure
	root, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	// Construct an XPath query to find the student with the given username and password
	xpathQuery := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)

	// Execute the XPath query
	nodes, err := goxpath.Find(root, xpathQuery)
	if err != nil {
		fmt.Println("Error executing XPath query:", err)
		return "", "", ""
	}

	// Check if any nodes were found
	if len(nodes) == 0 {
		return "", "", ""
	}

	// Extract the name, age, and citizenship from the first matching node
	nameNode := goxpath.FindFirst(nodes[0], "name")
	ageNode := goxpath.FindFirst(nodes[0], "age")
	citizenshipNode := goxpath.FindFirst(nodes[0], "citizenship")

	var name, age, citizenship string
	if nameNode != nil {
		name = nameNode.String()
	}
	if ageNode != nil {
		age = ageNode.String()
	}
	if citizenshipNode != nil {
		citizenship = citizenshipNode.String()
	}

	return name, age, citizenship
}
```