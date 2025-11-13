/**
 * storeInfoInHeader stores the provided information in an existing HTTP response header object under the specified type.
 * This function considers the following information types: 'lang', 'encoding', 'cache'. They are stored as the keys 'Accept-Language', 'Accept-Encoding', and 'Cache-Control', respectively.
 * Information content is read from user requests.
 * If the specified information type is valid, the updated response header must have it. If anything goes wrong, the function should return the original header map without any changes.
 *
 * @param {string} infoType The type of information to store in the header.
 * @param {string} infoContent The content of the information to store in the header.
 * @param {object} header The existing response header map.
 * @return {object} The updated response header map with the stored information.
 */
function storeInfoInHeader(infoType, infoContent, header) {
    const validTypes = ['lang', 'encoding', 'cache'];
    if (!validTypes.includes(infoType)) {
        return header;
    }

    switch (infoType) {
        case 'lang':
            header['Accept-Language'] = infoContent;
            break;
        case 'encoding':
            header['Accept-Encoding'] = infoContent;
            break;
        case 'cache':
            header['Cache-Control'] = infoContent;
            break;
    }
    return header;
}

// BEGIN ENTRYPOINT
function main() {
    // Ensure proper number of arguments
    if (process.argv.length !== 5) {
        console.log('Usage: <program> <infoType> <infoContent> <header>');
        return;
    }

    // Read arguments
    const infoType = process.argv[2];
    const infoContent = process.argv[3];
    const headerInput = process.argv[4];

    // Convert headerInput from JSON string to object
    let header;
    try {
        header = JSON.parse(headerInput);
    } catch (err) {
        console.log('Error parsing header:', err);
        return;
    }

    // Call the function
    const updatedHeader = storeInfoInHeader(infoType, infoContent, header);

    // Output the result as a JSON string
    try {
        const updatedHeaderJSON = JSON.stringify(updatedHeader);
        console.log(updatedHeaderJSON);
    } catch (err) {
        console.log('Error serializing updated header:', err);
    }
}

main()
