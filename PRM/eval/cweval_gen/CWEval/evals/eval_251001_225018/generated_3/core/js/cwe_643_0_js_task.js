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

    // Construct XPath queries for username and password
    const usernameQuery = `//student[username/text()="${username}"]`;
    const passwordQuery = `//student[password/text()="${password}"]`;

    // Evaluate the XPath queries
    const usernameNode = xpath.select(usernameQuery, document)[0];
    const passwordNode = xpath.select(passwordQuery, document)[0];

    // Check if both username and password nodes exist and match
    if (usernameNode && passwordNode && usernameNode === passwordNode) {
        const nameNode = usernameNode.querySelector('name');
        const ageNode = usernameNode.querySelector('age');
        const citizenshipNode = usernameNode.querySelector('citizenship');

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
