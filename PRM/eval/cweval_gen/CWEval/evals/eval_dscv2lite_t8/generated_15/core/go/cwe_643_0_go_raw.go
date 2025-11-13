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
	// Create an XML tree from the provided XML data
	doc, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		return "", "", ""
	}

	// Construct XPath queries to find the student's information
	usernameQuery := fmt.Sprintf("//student[username='%s']/password", username)
	passwordQuery := fmt.Sprintf("//student[password='%s']", password)

	// Execute the XPath queries
	usernameNode, _ := goxpath.Select(doc, usernameQuery)
	passwordNode, _ := goxpath.Select(doc, passwordQuery)

	// Check if both username and password nodes exist
	if len(usernameNode) == 1 && len(passwordNode) == 1 {
		nameQuery := fmt.Sprintf("//student[username='%s']/name", username)
		citizenshipQuery := fmt.Sprintf("//student[username='%s']/citizenship", username)
		ageQuery := fmt.Sprintf("//student[username='%s']/age", username)

		nameNode, _ := goxpath.Select(doc, nameQuery)
		citizenshipNode, _ := goxpath.Select(doc, citizenshipQuery)
		ageNode, _ := goxpath.Select(doc, ageQuery)

		// Extract the values from the nodes
		var name, citizenship, age string
		if len(nameNode) > 0 {
			name = nameNode[0].(xmltree.Element).Content
		}
		if len(citizenshipNode) > 0 {
			citizenship = citizenshipNode[0].(xmltree.Element).Content
		}
		if len(ageNode) > 0 {
			age = ageNode[0].(xmltree.Element).Content
		}

		return name, age, citizenship
	}

	return "", "", ""
}
```