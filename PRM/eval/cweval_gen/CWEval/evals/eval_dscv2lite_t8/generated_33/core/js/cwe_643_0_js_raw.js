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
    const students = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(document, '//ns:student[ns:username/text()="' + username + '"]');

    if (students.length === 0) {
        return null;
    }

    const student = students[0];
    const name = xpath.select('//ns:name', student)[0].textContent;
    const age = parseInt(xpath.select('//ns:age', student)[0].textContent);
    const citizenship = xpath.select('//ns:citizenship', student)[0].textContent;

    return { name, age, citizenship };
}
```