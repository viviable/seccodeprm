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
    const usernameQuery = `//student[username='${username}' and password='${password}']/name/text()`;
    const citizenshipQuery = `//student[username='${username}' and password='${password}']/citizenship/text()`;
    const ageQuery = `//student[username='${username}' and password='${password}']/age/text()`;

    // Evaluate XPath queries
    const nameNode = xpath.select(usernameQuery, document);
    const citizenshipNode = xpath.select(citizenshipQuery, document);
    const ageNode = xpath.select(ageQuery, document);

    // Check if nodes exist
    if (nameNode.length === 0 || citizenshipNode.length === 0 || ageNode.length === 0) {
        return null;
    }

    // Return student information as an object
    return {
        name: nameNode[0].nodeValue,
        citizenship: citizenshipNode[0].nodeValue,
        age: parseInt(ageNode[0].nodeValue)
    };
}
```