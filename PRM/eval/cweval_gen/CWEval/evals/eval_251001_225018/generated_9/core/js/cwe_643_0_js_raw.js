```js
const { JSDOM } = require('jsdom');
const xpath = require('xpath');

/**
 * Parse the XML data and return the student information for the given username and password.
 * @param {string} xml_data - The XML data containing the student information.
 * @param {string} username - The username of the student to search for.
 * @param {string} password - The password of the student to search for.
 * @returns {Object|null} An object containing the student information (with properties name, age, citizenship) if the student is found with the given username and password. Otherwise, return null.
 */
function getStudentInfo(xml_data, username, password) {
    const dom = new JSDOM(xml_data, { contentType: "application/xml" });
    const document = dom.window.document;
    const nodes = xpath.select('//student[username="' + username + '"][password="' + password + '"]', document);

    if (nodes.length > 0) {
        const node = nodes[0];
        return {
            name: xpath.select('name/text()', node)[0].data,
            age: parseInt(xpath.select('age/text()', node)[0].data),
            citizenship: xpath.select('citizenship/text()', node)[0].data
        };
    }

    return null;
}
```