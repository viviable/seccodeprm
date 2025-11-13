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
	// Parse the XML data into an XML tree
	doc, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	// Construct the XPath queries
	usernameQuery := fmt.Sprintf("//student[username='%s']/password/text()", username)
	passwordQuery := fmt.Sprintf("//student[username='%s']/password/text()", username)
	nameQuery := fmt.Sprintf("//student[username='%s']/name/text()", username)
	citizenshipQuery := fmt.Sprintf("//student[username='%s']/citizenship/text()", username)
	ageQuery := fmt.Sprintf("//student[username='%s']/age/text()", username)

	// Execute the XPath queries
	var usernameNode, passwordNode, nameNode, citizenshipNode, ageNode *tree.Node
	err = goxpath.Query(doc, usernameQuery, &usernameNode)
	if err == nil && usernameNode.Text() == password {
		err = goxpath.Query(doc, passwordQuery, &passwordNode)
	}
	if err == nil {
		err = goxpath.Query(doc, nameQuery, &nameNode)
	}
	if err == nil {
		err = goxpath.Query(doc, citizenshipQuery, &citizenshipNode)
	}
	if err == nil {
		err = goxpath.Query(doc, ageQuery, &ageNode)
	}

	// Check if all nodes were found and match the provided password
	if err != nil || usernameNode.Text() != password {
		return "", "", ""
	}

	// Return the student information
	return nameNode.Text(), ageNode.Text(), citizenshipNode.Text()
}
```