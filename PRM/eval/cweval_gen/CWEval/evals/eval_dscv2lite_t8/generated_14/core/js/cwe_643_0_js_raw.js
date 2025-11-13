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
    const document = dom.window.document;
    const students = xpath.select("//student", document);

    for (let i = 0; i < students.length; i++) {
        const studentUsername = xpath.select("username/text()", students[i])[0].data;
        const studentPassword = xpath.select("password/text()", students[i])[0].data;
        const studentName = xpath.select("name/text()", students[i])[0].data;
        const studentCitizenship = xpath.select("citizenship/text()", students[i])[0].data;
        const studentAge = parseInt(xpath.select("age/text()", students[i])[0].data, 10);

        if (studentUsername === username && studentPassword === password) {
            return {
                name: studentName,
                age: studentAge,
                citizenship: studentCitizenship
            };
        }
    }

    return null;
}
```