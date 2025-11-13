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

    // Construct XPath queries
    const usernameQuery = `//student[username='${username}']/password`;
    const passwordQuery = `//student[password='${password}']/username`;

    // Evaluate XPath queries
    const passwordNode = xpath.select(passwordQuery, document)[0];
    const usernameNode = xpath.select(usernameQuery, document)[0];

    // Check if both nodes exist and match
    if (passwordNode && passwordNode.textContent === password && usernameNode && usernameNode.textContent === username) {
        const studentNode = passwordNode.parentNode;
        const nameNode = studentNode.querySelector('name');
        const ageNode = studentNode.querySelector('age');
        const citizenshipNode = studentNode.querySelector('citizenship');

        return {
            name: nameNode ? nameNode.textContent : null,
            age: ageNode ? parseInt(ageNode.textContent, 10) : null,
            citizenship: citizenshipNode ? citizenshipNode.textContent : null
        };
    }

    return null;
}
```