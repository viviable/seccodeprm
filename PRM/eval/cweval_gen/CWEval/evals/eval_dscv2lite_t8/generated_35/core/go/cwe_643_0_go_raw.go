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
		return "", "", ""
	}

	// Construct the XPath queries
	usernameQuery := fmt.Sprintf("//student[username='%s']/password/text()", username)
	passwordQuery := fmt.Sprintf("//student[password='%s']/username/text()", password)

	// Execute the XPath queries
	usernameResult, _ := goxpath.Find(doc, usernameQuery)
	passwordResult, _ := goxpath.Find(doc, passwordQuery)

	// Check if both username and password match
	if len(usernameResult) > 0 && len(passwordResult) > 0 {
		nameQuery := fmt.Sprintf("//student[username='%s']/name/text()", username)
		citizenshipQuery := fmt.Sprintf("//student[username='%s']/citizenship/text()", username)
		ageQuery := fmt.Sprintf("//student[username='%s']/age/text()", username)

		nameResult, _ := goxpath.Find(doc, nameQuery)
		citizenshipResult, _ := goxpath.Find(doc, citizenshipQuery)
		ageResult, _ := goxpath.Find(doc, ageQuery)

		if len(nameResult) > 0 && len(citizenshipResult) > 0 && len(ageResult) > 0 {
			return nameResult[0].String(), ageResult[0].String(), citizenshipResult[0].String()
		}
	}

	return "", "", ""
}
```