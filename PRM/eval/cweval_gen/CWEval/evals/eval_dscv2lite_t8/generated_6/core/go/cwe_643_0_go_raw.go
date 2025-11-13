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
		return "", "", ""
	}

	query := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)
	result, err := goxpath.Find(doc, query)
	if err != nil || len(result) == 0 {
		return "", "", ""
	}

	node := result[0].(tree.Node)
	nameNode := node.SelectSingle("./name")
	citizenshipNode := node.SelectSingle("./citizenship")
	ageNode := node.SelectSingle("./age")

	var name, citizenship, age string
	if nameNode != nil {
		name = nameNode.(tree.Text).String()
	}
	if citizenshipNode != nil {
		citizenship = citizenshipNode.(tree.Text).String()
	}
	if ageNode != nil {
		age = ageNode.(tree.Text).String()
	}

	return name, age, citizenship
}
```