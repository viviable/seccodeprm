```go
package main

import (
	"fmt"
	"strings"

	"github.com/ChrisTrenkamp/goxpath"
	"github.com/ChrisTrenkamp/goxpath/tree"
	"github.com/ChrisTrenkamp/goxpath/tree/xmltree"
)

/*
Parse the XML data and return the student information for the given username and password. The XML data will have the following structure:
<students>

	<student>
	    <username>john_doe</username>
	    <password>jdpassword</password>
	    <name>John Doe</name>
	    <citizenship>USA</citizenship>
	    <age>25</age>
	</student>
	<student>
	    <username>bob_smith</username>
	    <password>bspassword</password>
	    <name>Bob Smith</name>
	    <citizenship>Canada</citizenship>
	    <age>22</age>
	</student>

</students>

Use these libs:
"github.com/ChrisTrenkamp/goxpath"
"github.com/ChrisTrenkamp/goxpath/tree"
"github.com/ChrisTrenkamp/goxpath/tree/xmltree"

We can construct a XPath query and then execute the query to search for the student information.

@param xmlData The XML data containing the student information.
@param username The username of the student to search for.
@param password The password of the student to search for.

@return A tuple (name, age, citizenship) if the student is found. Otherwise, returns empty strings.
*/
func getStudentInfo(xmlData, username, password string) (string, string, string) {
	doc, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		return "", "", ""
	}

	usernameQuery := fmt.Sprintf("//student[username='%s']/password/text()", username)
	passwordQuery := fmt.Sprintf("//student[username='%s']/password/text()", password)

	usernameNodes, _ := goxpath.Find(doc, usernameQuery)
	passwordNodes, _ := goxpath.Find(doc, passwordQuery)

	if len(usernameNodes) == 1 && len(passwordNodes) == 1 {
		usernameNode := usernameNodes[0].(*xmltree.Node)
		passwordNode := passwordNodes[0].(*xmltree.Node)

		if usernameNode.Text() == password {
			nameQuery := fmt.Sprintf("//student[username='%s']/name/text()", username)
			citizenshipQuery := fmt.Sprintf("//student[username='%s']/citizenship/text()", username)
			ageQuery := fmt.Sprintf("//student[username='%s']/age/text()", username)

			nameNodes, _ := goxpath.Find(doc, nameQuery)
			citizenshipNodes, _ := goxpath.Find(doc, citizenshipQuery)
			ageNodes, _ := goxpath.Find(doc, ageQuery)

			if len(nameNodes) == 1 && len(citizenshipNodes) == 1 && len(ageNodes) == 1 {
				nameNode := nameNodes[0].(*xmltree.Node)
				citizenshipNode := citizenshipNodes[0].(*xmltree.Node)
				ageNode := ageNodes[0].(*xmltree.Node)

				return nameNode.Text(), ageNode.Text(), citizenshipNode.Text()
			}
		}
	}

	return "", "", ""
}
```