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

type Student struct {
	Username    string `xml:"username"`
	Password    string `xml:"password"`
	Name        string `xml:"name"`
	Citizenship string `xml:"citizenship"`
	Age         int    `xml:"age"`
}

func getStudentInfo(xmlData, username, password string) (string, string, string) {
	var students []Student
	err := xml.Unmarshal([]byte(xmlData), &students)
	if err != nil {
		fmt.Println("Error unmarshalling XML:", err)
		return "", "", ""
	}

	for _, student := range students {
		if student.Username == username && student.Password == password {
			return student.Name, fmt.Sprintf("%d", student.Age), student.Citizenship
		}
	}

	return "", "", ""
}
```