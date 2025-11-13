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

// Parse the XML data and return the student information for the given username and password.
func getStudentInfo(xmlData, username, password string) (string, string, string) {
	// Parse the XML data
	var root tree.Node
	err := xml.Unmarshal([]byte(xmlData), &root)
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	// Create an XPath expression
	expr, err := goxpath.NewExpr("/students/student[username='" + username + "'][password='" + password + "']")
	if err != nil {
		fmt.Println("Error creating XPath expression:", err)
		return "", "", ""
	}

	// Execute the XPath expression
	results, err := expr.Evaluate(&root)
	if err != nil {
		fmt.Println("Error evaluating XPath expression:", err)
		return "", "", ""
	}

	// Check if any results were found
	if len(results) == 0 {
		return "", "", ""
	}

	// Get the first result
	result := results[0]

	// Extract the name, age, and citizenship
	name := ""
	age := ""
	citizenship := ""

	for _, child := range result.Children() {
		switch child.Name() {
		case "name":
			name = child.Text()
		case "age":
			age = child.Text()
		case "citizenship":
			citizenship = child.Text()
		}
	}

	return name, age, citizenship
}
```