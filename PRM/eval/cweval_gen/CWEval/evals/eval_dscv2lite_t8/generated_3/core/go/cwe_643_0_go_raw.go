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
	Age         string `xml:"age"`
}

func parseXML(xmlData string) (*tree.Node, error) {
	reader := strings.NewReader(xmlData)
	doc, err := xmltree.Parse(reader)
	if err != nil {
		return nil, err
	}
	root, err := tree.Build(doc)
	if err != nil {
		return nil, err
	}
	return root, nil
}

func getStudentInfo(xmlData, username, password string) (string, string, string) {
	root, err := parseXML(xmlData)
	if err != nil {
		fmt.Println("Error parsing XML:", err)
		return "", "", ""
	}

	xpathQuery := fmt.Sprintf("//student[username='%s' and password='%s']", username, password)
	result, err := goxpath.Select(root, xpathQuery)
	if err != nil {
		fmt.Println("Error executing XPath query:", err)
		return "", "", ""
	}

	if len(result) == 0 {
		return "", "", ""
	}

	var student Student
	err = xml.Unmarshal([]byte(strings.TrimSpace(result[0].String())), &student)
	if err != nil {
		fmt.Println("Error unmarshalling student info:", err)
		return "", "", ""
	}

	return student.Name, student.Age, student.Citizenship
}
```