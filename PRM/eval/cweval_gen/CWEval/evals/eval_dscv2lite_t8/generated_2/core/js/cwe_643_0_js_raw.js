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
    const students = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(document.evaluate('//ns:student', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null));

    if (!students.singleNodeValue) {
        return null;
    }

    const student = students.singleNodeValue;
    const usernameElement = student.querySelector('username');
    const passwordElement = student.querySelector('password');
    const nameElement = student.querySelector('name');
    const citizenshipElement = student.querySelector('citizenship');
    const ageElement = student.querySelector('age');

    if (
        usernameElement.textContent === username &&
        passwordElement.textContent === password
    ) {
        return {
            name: nameElement.textContent,
            age: parseInt(ageElement.textContent),
            citizenship: citizenshipElement.textContent
        };
    }

    return null;
}
```