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
		return "", "", ""
	}

	query := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)
	result, err := goxpath.Select(query, tree.New(doc))
	if err != nil || len(result) == 0 {
		return "", "", ""
	}

	nameNode, _ := result[0].Find("name")
	citizenshipNode, _ := result[0].Find("citizenship")
	ageNode, _ := result[0].Find("age")

	name := nameNode.Text()
	citizenship := citizenshipNode.Text()
	age := ageNode.Text()

	return name, age, citizenship
}
```