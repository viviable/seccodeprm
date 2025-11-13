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
    const students = xpath.useNamespaces({x: 'http://www.w3.org/1999/xhtml'});
    const studentNodes = students('//x:student[x:username="' + username + '"]', document);

    if (studentNodes.length === 0) {
        return null;
    }

    const studentNode = studentNodes[0];
    const passwordNode = studentNode.querySelector('x:password');
    if (passwordNode && passwordNode.textContent === password) {
        const nameNode = studentNode.querySelector('x:name');
        const citizenshipNode = studentNode.querySelector('x:citizenship');
        const ageNode = studentNode.querySelector('x:age');
        return {
            name: nameNode ? nameNode.textContent : null,
            age: ageNode ? parseInt(ageNode.textContent, 10) : null,
            citizenship: citizenshipNode ? citizenshipNode.textContent : null
        };
    }

    return null;
}
```