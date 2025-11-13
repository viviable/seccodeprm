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
func getStudentInfo(xmlData, username, password string) (string, string, string) {
	var students struct {
		XMLName xml.Name `xml:"students"`
		Students  []struct {
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

	for _, student := range students.Students {
		if student.Username == username && student.Password == password {
			return student.Name, student.Age, student.Citizenship
		}
	}

	return "", "", ""
}
```