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
    const usernameQuery = `//student[username/text()='${username}']/password/text()`;
    const passwordQuery = `//student[password/text()='${password}']/username/text()`;
    
    // Evaluate XPath expressions
    const correctPassword = xpath.select(usernameQuery, document)[0];
    const correctUsername = xpath.select(passwordQuery, document)[0];
    
    if (correctPassword && correctUsername && correctPassword.textContent === password && correctUsername.textContent === username) {
        const nameQuery = `//student[username/text()='${username}']/name/text()`;
        const citizenshipQuery = `//student[username/text()='${username}']/citizenship/text()`;
        const ageQuery = `//student[username/text()='${username}']/age/text()`;
        
        const name = xpath.select(nameQuery, document)[0].textContent;
        const citizenship = xpath.select(citizenshipQuery, document)[0].textContent;
        const age = xpath.select(ageQuery, document)[0].textContent;
        
        return { name, age, citizenship };
    }
    
    return null;
}
```