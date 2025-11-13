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
    const xpr = xpath.useNamespaces({ students: 'http://example.com/students' });
    const students = xpr('//students/student', dom.window.document);

    for (let i = 0; i < students.length; i++) {
        const student = students[i];
        const userNode = xpr('./username/text()', student)[0];
        const passNode = xpr('./password/text()', student)[0];
        const nameNode = xpr('./name/text()', student)[0];
        const citizenshipNode = xpr('./citizenship/text()', student)[0];
        const ageNode = xpr('./age/text()', student)[0];

        if (userNode && passNode && userNode.textContent === username && passNode.textContent === password) {
            return {
                name: nameNode ? nameNode.textContent : null,
                age: ageNode ? parseInt(ageNode.textContent, 10) : null,
                citizenship: citizenshipNode ? citizenshipNode.textContent : null
            };
        }
    }

    return null;
}
```