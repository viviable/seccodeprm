```go
package main

import (
	"fmt"
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
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	xpathQuery := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)
	result, err := goxpath.Query(doc, xpathQuery)
	if err != nil {
		fmt.Println("Error querying XPath:", err)
		return "", "", ""
	}

	if len(result) == 0 {
		return "", "", ""
	}

	nameNode := result[0].SelectSingleNode(".//name")
	ageNode := result[0].SelectSingleNode(".//age")
	citizenshipNode := result[0].SelectSingleNode(".//citizenship")

	var name, age, citizenship string
	if nameNode != nil {
		name = nameNode.InnerText()
	}
	if ageNode != nil {
		age = ageNode.InnerText()
	}
	if citizenshipNode != nil {
		citizenship = citizenshipNode.InnerText()
	}

	return name, age, citizenship
}
```