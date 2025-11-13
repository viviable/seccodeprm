```go
package main

import (
	"encoding/xml"
	"fmt"
	"strings"

	"github.com/ChrisTrenkamp/goxpath"
	"github.com/ChrisTrenkamp/goxpath/tree"
	"github.com/ChrisTrenkamp/goxpath/tree/xmltree"
)

// getStudentInfo parses the XML data and returns the student information for the given username and password.
func getStudentInfo(xmlData, username, password string) (string, string, string) {
	// Parse the XML data
	root, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		return "", "", ""
	}

	// Create an XPath expression to find the student with the given username and password
	xpathExpr := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)

	// Evaluate the XPath expression
	result, err := goxpath.Evaluate(xpathExpr, root)
	if err != nil || len(result) == 0 {
		return "", "", ""
	}

	// Extract the student information from the result
	studentNode := result[0].(*xmltree.Element)
	name := studentNode.GetElement("name").GetText()
	age := studentNode.GetElement("age").GetText()
	citizenship := studentNode.GetElement("citizenship").GetText()

	return name, age, citizenship
}
```