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
    
    // Constructing the XPath query
    const xpathQuery = `//student[username/text()="${username}" and password/text()="${password}"]`;
    const node = xpath.select(xpathQuery, document)[0];
    
    if (node) {
        const nameNode = xpath.select('name', node)[0];
        const ageNode = xpath.select('age', node)[0];
        const citizenshipNode = xpath.select('citizenship', node)[0];
        
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