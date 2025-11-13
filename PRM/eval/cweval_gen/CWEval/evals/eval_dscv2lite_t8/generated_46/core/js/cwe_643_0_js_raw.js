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
    const students = xpath.useNamespaces({ns: 'http://www.w3.org/1999/xhtml'});
    const query = "//student[username/text()='" + username + "' and password/text()='" + password + "']";
    const result = students.evaluate(query, document, null, xpath.XPathResult.FIRST_ORDERED_NODE_TYPE, null);
    
    if (result.singleNodeValue) {
        const student = result.singleNodeValue;
        const name = student.querySelector('name').textContent;
        const age = student.querySelector('age').textContent;
        const citizenship = student.querySelector('citizenship').textContent;
        return { name, age, citizenship };
    }
    
    return null;
}
```