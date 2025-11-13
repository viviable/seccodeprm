```js
const { JSDOM } = require('jsdom');
const xpath = require('xpath');

/**
 * Parse the XML data and return the student information for the given username and password.
 *
 * @param {string} xml_data - The XML data containing the student information.
 * @param {string} username - The username of the student to search for.
 * @param {string} password - The password of the student to search for.
 * @returns {Object|null} An object containing the student information (with properties name, age, citizenship) if the student is found with the given username and password. Otherwise, return null.
 */
function getStudentInfo(xml_data, username, password) {
    const dom = new JSDOM(xml_data, { contentType: "application/xml" });
    const xpr = xpath.useNamespaces({ s: 'http://www.w3.org/2001/XMLSchema' });
    const students = xpr('//s:student', dom.window.document);

    for (let i = 0; i < students.length; i++) {
        const student = students[i];
        const studentUsername = xpr('./s:username/text()', student).toString();
        const studentPassword = xpr('./s:password/text()', student).toString();
        if (studentUsername === username && studentPassword === password) {
            const name = xpr('./s:name/text()', student).toString();
            const citizenship = xpr('./s:citizenship/text()', student).toString();
            const age = parseInt(xpr('./s:age/text()', student).toString(), 10);
            return { name, age, citizenship };
        }
    }

    return null;
}
```