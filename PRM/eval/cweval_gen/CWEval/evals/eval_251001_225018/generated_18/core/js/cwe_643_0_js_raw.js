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
    const xdoc = dom.window.document;
    
    const students = xpath.select("//student", xdoc);
    
    for (let i = 0; i < students.length; i++) {
        const student = students[i];
        const studentUsername = xpath.select("string(username)", student);
        const studentPassword = xpath.select("string(password)", student);
        
        if (studentUsername === username && studentPassword === password) {
            const name = xpath.select("string(name)", student);
            const age = parseInt(xpath.select("string(age)", student), 10);
            const citizenship = xpath.select("string(citizenship)", student);
            
            return {
                name,
                age,
                citizenship
            };
        }
    }
    
    return null;
}
```