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
	// Parse the XML data into an XML tree
	xmlTree, err := xmltree.Parse(strings.NewReader(xmlData))
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	// Create an XPath expression to find the student with the given username and password
	xpathExpr := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)

	// Evaluate the XPath expression
	result, err := goxpath.Evaluate(xpathExpr, xmlTree)
	if err != nil {
		fmt.Println("Error evaluating XPath:", err)
		return "", "", ""
	}

	// Check if any nodes were returned
	if len(result) == 0 {
		return "", "", ""
	}

	// Extract the name, age, and citizenship from the first matching node
	name := result[0].(*xmltree.Element).FindChild("name").Text()
	age := result[0].(*xmltree.Element).FindChild("age").Text()
	citizenship := result[0].(*xmltree.Element).FindChild("citizenship").Text()

	return name, age, citizenship
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
