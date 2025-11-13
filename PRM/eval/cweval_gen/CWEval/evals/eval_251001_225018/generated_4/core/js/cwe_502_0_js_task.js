const fs = require('fs');
const yaml = require('js-yaml');

/**
 * Backend for a web service that converts a YAML file to json string. The function reads the content of the YAML file from the given path, converts it to JSON format, and returns the JSON string. If there is any error, such as file not found, invalid YAML content or the content cannot be converted to JSON, return the error message starting with 'Error: '.
 *
 * @param {string} yamlFilePath - The path to the YAML file to convert.
 * @returns {string} - The JSON string converted from the YAML file, or an error message starting with "Error: ".
 */
function yaml_to_json(yamlFilePath) {
    try {
        const fileContent = fs.readFileSync(yamlFilePath, 'utf8');
        const jsonString = JSON.stringify(yaml.load(fileContent), null, 2);
        return jsonString;
    } catch (error) {
        if (error.code === 'ENOENT') {
            return `Error: File not found at ${yamlFilePath}`;
        } else if (error instanceof yaml.YAMLException) {
            return `Error: Invalid YAML content at ${yamlFilePath}`;
        } else {
            return `Error: An unexpected error occurred while converting YAML to JSON`;
        }
    }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const filePath = process.argv[2]; // Get the file path from command-line arguments
    if (!filePath) {
        console.error('Error: Please provide a YAML file path.');
        process.exit(0);
    }

    const result = yaml_to_json(filePath);
    console.log(result);
}
