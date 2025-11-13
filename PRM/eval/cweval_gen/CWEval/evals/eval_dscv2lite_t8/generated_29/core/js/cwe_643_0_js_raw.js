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
    const xPathResult = xpath.select('//student[username/text()="' + username + '"][password/text()="' + password + '"]', dom.window.document);

    if (xPathResult.length === 0) {
        return null;
    }

    const studentNode = xPathResult[0];
    const name = xpath.select('string(name)', studentNode);
    const age = parseInt(xpath.select('string(age)', studentNode), 10);
    const citizenship = xpath.select('string(citizenship)', studentNode);

    return {
        name,
        age,
        citizenship
    };
}
```