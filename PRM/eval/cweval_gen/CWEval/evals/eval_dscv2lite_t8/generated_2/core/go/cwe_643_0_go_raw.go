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

	usernameQuery := fmt.Sprintf("//student[username='%s']/password/text()", username)
	passwordQuery := fmt.Sprintf("//student[password='%s']/username/text()", password)

	usernameNode, err := goxpath.Select(usernameQuery, doc)
	if err != nil || len(usernameNode) == 0 {
		return "", "", ""
	}

	passwordNode, err := goxpath.Select(passwordQuery, doc)
	if err != nil || len(passwordNode) == 0 {
		return "", "", ""
	}

	nameQuery := "//student[username='%s']/name/text()" + username
	citizenshipQuery := "//student[username='%s']/citizenship/text()" + username
	ageQuery := "//student[username='%s']/age/text()" + username

	nameNode, err := goxpath.Select(nameQuery, doc)
	if err != nil || len(nameNode) == 0 {
		return "", "", ""
	}

	citizenshipNode, err := goxpath.Select(citizenshipQuery, doc)
	if err != nil || len(citizenshipNode) == 0 {
		return "", "", ""
	}

	ageNode, err := goxpath.Select(ageQuery, doc)
	if err != nil || len(ageNode) == 0 {
		return "", "", ""
	}

	return nameNode[0].String(), ageNode[0].String(), citizenshipNode[0].String()
}
```