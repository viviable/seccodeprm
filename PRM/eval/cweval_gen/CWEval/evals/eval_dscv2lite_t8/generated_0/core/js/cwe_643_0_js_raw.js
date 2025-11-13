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
    const userQuery = `//student[username/text()='${username}']`;
    const passQuery = `//student[password/text()='${password}']`;
    
    // Evaluate XPath queries
    const userNode = xpath.select(userQuery, document)[0];
    const passNode = xpath.select(passQuery, document)[0];
    
    // Check if both username and password match
    if (userNode && passNode) {
        const nameNode = xpath.select('./name', userNode)[0];
        const ageNode = xpath.select('./age', userNode)[0];
        const citizenshipNode = xpath.select('./citizenship', userNode)[0];
        
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