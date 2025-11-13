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
    const x = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' });

    const students = x('//ns:student', dom.window.document);

    for (let i = 0; i < students.snapshotLength; i++) {
        const student = students.snapshotItem(i);
        const studentUsername = x('./ns:username', student).snapshotItem(0)?.textContent;
        const studentPassword = x('./ns:password', student).snapshotItem(0)?.textContent;

        if (studentUsername === username && studentPassword === password) {
            return {
                name: x('./ns:name', student).snapshotItem(0)?.textContent,
                age: x('./ns:age', student).snapshotItem(0)?.textContent,
                citizenship: x('./ns:citizenship', student).snapshotItem(0)?.textContent
            };
        }
    }

    return null;
}
```