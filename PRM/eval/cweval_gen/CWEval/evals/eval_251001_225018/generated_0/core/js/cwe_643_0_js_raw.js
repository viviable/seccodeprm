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

    // Constructing XPath queries
    const usernameQuery = `//student[username/text()="${username}"]/password/text()`;
    const passwordQuery = `//student[password/text()="${password}"]/username/text()`;

    // Evaluating XPath queries
    const usernameNode = xpath.select(usernameQuery, document)[0];
    const passwordNode = xpath.select(passwordQuery, document)[0];

    // Checking if both username and password match
    if (usernameNode && passwordNode && usernameNode.textContent === passwordNode.textContent) {
        const studentNameQuery = `//student[username/text()="${username}"]/name/text()`;
        const studentAgeQuery = `//student[username/text()="${username}"]/age/text()`;
        const studentCitizenshipQuery = `//student[username/text()="${username}"]/citizenship/text()`;

        const nameNode = xpath.select(studentNameQuery, document)[0];
        const ageNode = xpath.select(studentAgeQuery, document)[0];
        const citizenshipNode = xpath.select(studentCitizenshipQuery, document)[0];

        if (nameNode && ageNode && citizenshipNode) {
            return {
                name: nameNode.textContent,
                age: parseInt(ageNode.textContent),
                citizenship: citizenshipNode.textContent
            };
        }
    }

    return null;
}
```