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
	var students []struct {
		Username    string `xml:"username"`
		Password    string `xml:"password"`
		Name        string `xml:"name"`
		Citizenship string `xml:"citizenship"`
		Age         string `xml:"age"`
	}

	err := xml.Unmarshal([]byte(xmlData), &students)
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	for _, student := range students {
		if student.Username == username && student.Password == password {
			return student.Name, student.Age, student.Citizenship
		}
	}

	return "", "", ""
}
```