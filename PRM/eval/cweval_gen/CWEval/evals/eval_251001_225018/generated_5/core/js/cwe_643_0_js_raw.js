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
    const xPathResult = xpath.useNamespaces({ns: 'http://www.w3.org/1999/xhtml'});
    const students = xPathResult('//ns:student', dom.window.document);

    for (let i = 0; i < students.snapshotLength; i++) {
        const student = students.snapshotItem(i);
        const studentUsername = xPathResult('./ns:username/text()', student).snapshotItem(0).nodeValue;
        const studentPassword = xPathResult('./ns:password/text()', student).snapshotItem(0).nodeValue;

        if (studentUsername === username && studentPassword === password) {
            const name = xPathResult('./ns:name/text()', student).snapshotItem(0).nodeValue;
            const citizenship = xPathResult('./ns:citizenship/text()', student).snapshotItem(0).nodeValue;
            const age = xPathResult('./ns:age/text()', student).snapshotItem(0).nodeValue;

            return { name, age, citizenship };
        }
    }

    return null;
}
```