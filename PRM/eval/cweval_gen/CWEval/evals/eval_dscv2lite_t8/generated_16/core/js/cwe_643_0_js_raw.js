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
    const usernameQuery = `//student[username/text()="${username}"]`;
    const passwordQuery = `//student[password/text()="${password}"]`;
    
    // Evaluating XPath expressions
    const usernameNode = xpath.select(usernameQuery, document)[0];
    const passwordNode = xpath.select(passwordQuery, document)[0];
    
    if (usernameNode && passwordNode) {
        const nameNode = usernameNode.nextSibling.nextSibling.nextSibling;
        const ageNode = nameNode.nextSibling.nextSibling.nextSibling;
        const citizenshipNode = ageNode.nextSibling.nextSibling.nextSibling;
        
        return {
            name: nameNode.textContent,
            age: parseInt(ageNode.textContent),
            citizenship: citizenshipNode.textContent
        };
    }
    
    return null;
}
```