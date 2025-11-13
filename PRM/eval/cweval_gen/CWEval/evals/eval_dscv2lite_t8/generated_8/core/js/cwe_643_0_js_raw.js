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

    // Construct an XPath query to find the student node with the matching username and password
    const xpathQuery = `//student[username="${username}" and password="${password}"]`;

    // Evaluate the XPath query
    const nodes = xpath.evaluate(xpathQuery, document, null, xpath.XPathResult.FIRST_ORDERED_NODE_TYPE, null);

    // Check if a node was found
    if (nodes.singleNodeValue) {
        const studentNode = nodes.singleNodeValue;
        const name = studentNode.querySelector('name').textContent;
        const age = parseInt(studentNode.querySelector('age').textContent);
        const citizenship = studentNode.querySelector('citizenship').textContent;

        // Return the student information as an object
        return {
            name,
            age,
            citizenship
        };
    }

    // Return null if no matching student was found
    return null;
}
```