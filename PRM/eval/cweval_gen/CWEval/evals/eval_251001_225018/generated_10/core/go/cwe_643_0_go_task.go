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

	usernameQuery := fmt.Sprintf("//student[username='%s']/password", username)
	passwordQuery := fmt.Sprintf("//student[password='%s']/username", password)

	usernameNode, err := goxpath.Find(doc, usernameQuery)
	if err != nil || len(usernameNode) == 0 {
		return "", "", ""
	}

	passwordNode, err := goxpath.Find(doc, passwordQuery)
	if err != nil || len(passwordNode) == 0 {
		return "", "", ""
	}

	if usernameNode[0].Value() == password {
		nameQuery := fmt.Sprintf("//student[username='%s']/name", username)
		citizenshipQuery := fmt.Sprintf("//student[username='%s']/citizenship", username)
		ageQuery := fmt.Sprintf("//student[username='%s']/age", username)

		nameNode, _ := goxpath.Find(doc, nameQuery)
		citizenshipNode, _ := goxpath.Find(doc, citizenshipQuery)
		ageNode, _ := goxpath.Find(doc, ageQuery)

		if len(nameNode) > 0 && len(citizenshipNode) > 0 && len(ageNode) > 0 {
			return nameNode[0].Value(), ageNode[0].Value(), citizenshipNode[0].Value()
		}
	}

	return "", "", ""
}

// BEGIN ENTRYPOINT
func main() {
	// Check command line arguments
	if len(os.Args) != 4 {
		fmt.Fprintf(os.Stderr, "Usage: %s \"<xml_data>\" <username> <password>\n", os.Args[0])
		os.Exit(1)
	}

	// Get student info directly from command line arguments
	name, age, citizenship := getStudentInfo(os.Args[1], os.Args[2], os.Args[3])

	// Print results
	if name != "" || age != "" || citizenship != "" {
		fmt.Println(name)
		fmt.Println(age)
		fmt.Println(citizenship)
	}
}
