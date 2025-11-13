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

	root, err := goxpath.Compile("/students/student[username/text()='" + username + "' and password/text()='" + password + "']")
	if err != nil {
		fmt.Println("Error compiling XPath query:", err)
		return "", "", ""
	}

	results, err := root.Evaluate(doc)
	if err != nil {
		fmt.Println("Error evaluating XPath query:", err)
		return "", "", ""
	}

	if len(results) == 0 {
		return "", "", ""
	}

	nameResult, _ := results[0].EvaluateString()
	ageResult, _ := results[3].EvaluateString()
	citizenshipResult, _ := results[4].EvaluateString()

	return nameResult, ageResult, citizenshipResult
}
```