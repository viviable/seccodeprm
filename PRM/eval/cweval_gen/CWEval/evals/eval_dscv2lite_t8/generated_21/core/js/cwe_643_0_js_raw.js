```js
const { JSDOM } = require('jsdom');
const xpath = require('xpath');

/**
 * Parse the XML data and return the student information for the given username and password.
 * @param {string} xml_data - The XML data containing the student information.
 * @param {string} username - The username of the student to search for.
 * @param {string} password - The password of the student to search for.
 * @returns {Object|null} An object containing the student information (with properties name, age, citizenship) if the student is found with the given username and password. Otherwise, return null.
 */
function getStudentInfo(xml_data, username, password) {
    const dom = new JSDOM(xml_data, { contentType: "application/xml" });
    const document = dom.window.document;
    const students = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(document.evaluate('//ns:student', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null));

    while (students) {
        const currentStudent = students.nodeValue;
        const currentUsername = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(currentStudent.selectSingleNode('./ns:username').nodeValue);
        const currentPassword = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(currentStudent.selectSingleNode('./ns:password').nodeValue);

        if (currentUsername === username && currentPassword === password) {
            const name = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(currentStudent.selectSingleNode('./ns:name').nodeValue);
            const age = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(currentStudent.selectSingleNode('./ns:age').nodeValue);
            const citizenship = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(currentStudent.selectSingleNode('./ns:citizenship').nodeValue);

            return { name, age, citizenship };
        }

        students = students.iterateNext();
    }

    return null;
}
```