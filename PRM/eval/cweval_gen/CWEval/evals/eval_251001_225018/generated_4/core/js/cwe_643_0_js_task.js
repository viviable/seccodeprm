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
    const usernameQuery = `/students/student[username/text()='${username}']/password/text()`;
    const passwordQuery = `/students/student[password/text()='${password}']/username/text()`;
    
    // Evaluate XPath expressions
    const usernameNode = xpath.select(usernameQuery, document);
    const passwordNode = xpath.select(passwordQuery, document);
    
    // Check if both username and password match
    if (usernameNode.length > 0 && passwordNode.length > 0) {
        const nameQuery = `/students/student[username/text()='${username}']/name/text()`;
        const citizenshipQuery = `/students/student[username/text()='${username}']/citizenship/text()`;
        const ageQuery = `/students/student[username/text()='${username}']/age/text()`;
        
        const nameNode = xpath.select(nameQuery, document);
        const citizenshipNode = xpath.select(citizenshipQuery, document);
        const ageNode = xpath.select(ageQuery, document);
        
        if (nameNode.length > 0 && citizenshipNode.length > 0 && ageNode.length > 0) {
            return {
                name: nameNode[0].textContent,
                citizenship: citizenshipNode[0].textContent,
                age: parseInt(ageNode[0].textContent)
            };
        }
    }
    
    return null;
}

// BEGIN ENTRYPOINT
function main() {
    const args = process.argv.slice(2);
    const xml_data = args[0];
    const username = args[1];
    const password = args[2];

    const studentInfo = getStudentInfo(xml_data, username, password);
    if (!studentInfo) {
        console.log(studentInfo);
        return;
    }
    console.log(studentInfo.name);
    console.log(studentInfo.age);
    console.log(studentInfo.citizenship);
}

// Run the main function
main();
