```go
package main

import (
	"encoding/xml"
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
	// Parse the XML data
	var students struct {
		Students []struct {
			Username    string `xml:"username"`
			Password    string `xml:"password"`
			Name        string `xml:"name"`
			Citizenship string `xml:"citizenship"`
			Age         string `xml:"age"`
		} `xml:"student"`
	}
	err := xml.Unmarshal([]byte(xmlData), &students)
	if err != nil {
		return "", "", ""
	}

	// Search for the student with the given username and password
	for _, student := range students.Students {
		if student.Username == username && student.Password == password {
			return student.Name, student.Age, student.Citizenship
		}
	}

	// If no student is found, return empty strings
	return "", "", ""
}
```