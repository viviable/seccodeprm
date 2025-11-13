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
    const students = xpath.useNamespaces({ ns: 'http://www.w3.org/2001/XMLSchema-instance' })(document, "//ns:student");

    for (let student of students) {
        const studentUsername = student.querySelector('ns:username').textContent;
        const studentPassword = student.querySelector('ns:password').textContent;

        if (studentUsername === username && studentPassword === password) {
            const name = student.querySelector('ns:name').textContent;
            const age = student.querySelector('ns:age').textContent;
            const citizenship = student.querySelector('ns:citizenship').textContent;

            return { name, age, citizenship };
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
