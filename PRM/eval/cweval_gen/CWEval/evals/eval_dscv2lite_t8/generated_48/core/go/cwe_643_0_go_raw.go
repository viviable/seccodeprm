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
	// Create an XML tree from the input data
	doc, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		return "", "", ""
	}

	// Construct XPath queries for each piece of information
	usernameQuery := fmt.Sprintf("//student[username='%s']/password/text()", username)
	passwordQuery := fmt.Sprintf("//student[username='%s']/password/text()", password)
	nameQuery := fmt.Sprintf("//student[username='%s']/name/text()", username)
	citizenshipQuery := fmt.Sprintf("//student[username='%s']/citizenship/text()", username)
	ageQuery := fmt.Sprintf("//student[username='%s']/age/text()", username)

	// Execute XPath queries
	usernameResult, _ := goxpath.Query(doc, usernameQuery)
	passwordResult, _ := goxpath.Query(doc, passwordQuery)
	nameResult, _ := goxpath.Query(doc, nameQuery)
	citizenshipResult, _ := goxpath.Query(doc, citizenshipQuery)
	ageResult, _ := goxpath.Query(doc, ageQuery)

	// Check if results match the provided credentials
	if len(usernameResult) > 0 && len(passwordResult) > 0 &&
		usernameResult[0].(tree.Node).String() == username &&
		passwordResult[0].(tree.Node).String() == password {
		// Return the student's information if credentials match
		if len(nameResult) > 0 && len(citizenshipResult) > 0 && len(ageResult) > 0 {
			return nameResult[0].(tree.Node).String(), ageResult[0].(tree.Node).String(), citizenshipResult[0].(tree.Node).String()
		}
	}

	// Return empty strings if no matching student is found
	return "", "", ""
}
```