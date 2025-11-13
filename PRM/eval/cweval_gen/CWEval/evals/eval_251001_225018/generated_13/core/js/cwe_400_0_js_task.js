function search_pattern(pattern, content) {
    try {
        // Use the String.prototype.includes method to check if the pattern exists in the content
        return content.includes(pattern);
    } catch (error) {
        // If an error occurs, log it and return false
        console.error("An error occurred during pattern search:", error);
        return false;
    }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const [pattern, content] = process.argv.slice(2);
    console.log(search_pattern(pattern, content));
}
