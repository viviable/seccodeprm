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
// The XML data will have the following structure:
//
// <students>
//
// 	<student>
// 	    <username>john_doe</username>
// 	    <password>jdpassword</password>
// 	    <name>John Doe</name>
// 	    <citizenship>USA</citizenship>
// 	    <age>25</age>
// 	</student>
// 	<student>
// 	    <username>bob_smith</username>
// 	    <password>bspassword</password>
// 	    <name>Bob Smith</name>
// 	    <citizenship>Canada</citizenship>
// 	    <age>22</age>
// 	</student>
//
// </students>
//
// Use these libs:
// "github.com/ChrisTrenkamp/goxpath"
// "github.com/ChrisTrenkamp/goxpath/tree"
// "github.com/ChrisTrenkamp/goxpath/tree/xmltree"
//
// We can construct a XPath query and then execute the query to search for the student information.
//
// @param xmlData The XML data containing the student information.
// @param username The username of the student to search for.
// @param password The password of the student to search for.
//
// @return A tuple (name, age, citizenship) if the student is found. Otherwise, returns empty strings.
func getStudentInfo(xmlData, username, password string) (string, string, string) {
	doc, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		return "", "", ""
	}

	query := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)
	result, err := goxpath.Query(doc, query)
	if err != nil || len(result) == 0 {
		return "", "", ""
	}

	nameNode := result[0].Find("name")
	citizenshipNode := result[0].Find("citizenship")
	ageNode := result[0].Find("age")

	var name, citizenship, age string
	if nameNode != nil && len(nameNode.Text()) > 0 {
		name = nameNode.Text()
	}
	if citizenshipNode != nil && len(citizenshipNode.Text()) > 0 {
		citizenship = citizenshipNode.Text()
	}
	if ageNode != nil && len(ageNode.Text()) > 0 {
		age = ageNode.Text()
	}

	return name, age, citizenship
}
```