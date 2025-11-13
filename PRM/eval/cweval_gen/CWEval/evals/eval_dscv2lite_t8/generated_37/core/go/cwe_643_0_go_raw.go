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
	result, err := goxpath.Select(query, tree.New(doc), nil)
	if err != nil || len(result) == 0 {
		return "", "", ""
	}

	name := result[0].(*xmltree.Element).SelectSingleNode("name").Value
	age := result[0].(*xmltree.Element).SelectSingleNode("age").Value
	citizenship := result[0].(*xmltree.Element).SelectSingleNode("citizenship").Value

	return name, age, citizenship
}
```