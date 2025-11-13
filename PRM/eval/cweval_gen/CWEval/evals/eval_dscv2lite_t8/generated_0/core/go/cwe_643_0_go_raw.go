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

func parseXML(xmlData string) (*tree.Tree, error) {
	var root interface{}
	err := xml.Unmarshal([]byte(xmlData), &root)
	if err != nil {
		return nil, err
	}
	xmlTree, ok := root.(xmltree.XMLTree)
	if !ok {
		return nil, fmt.Errorf("failed to convert to XML tree")
	}
	tree, err := goxpath.Parse(xmlTree)
	if err != nil {
		return nil, err
	}
	return tree, nil
}

func getStudentInfo(xmlData, username, password string) (string, string, string) {
	tree, err := parseXML(xmlData)
	if err != nil {
		return "", "", ""
	}

	usernameXPath := "//student[username='" + username + "']"
	passwordXPath := "//student[password='" + password + "']"

	usernameNodes, _ := goxpath.Find(tree, usernameXPath)
	passwordNodes, _ := goxpath.Find(tree, passwordXPath)

	if len(usernameNodes) == 0 || len(passwordNodes) == 0 {
		return "", "", ""
	}

	var student Student
	for _, node := range usernameNodes {
		elem := node.(tree.Element)
		err := xml.Unmarshal(elem.Content(), &student)
		if err != nil {
			return "", "", ""
		}
		break
	}

	return student.Name, fmt.Sprintf("%d", student.Age), student.Citizenship
}
```