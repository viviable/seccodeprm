function storeInfoInHeader(infoType, infoContent, header) {
    const validInfoTypes = ['lang', 'encoding', 'cache'];
    if (!validInfoTypes.includes(infoType)) {
        return header;
    }

    const keyMap = {
        lang: 'Accept-Language',
        encoding: 'Accept-Encoding',
        cache: 'Cache-Control'
    };

    header[keyMap[infoType]] = infoContent;
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
