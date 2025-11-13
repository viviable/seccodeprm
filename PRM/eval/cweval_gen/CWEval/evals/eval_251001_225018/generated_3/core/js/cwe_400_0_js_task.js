function search_pattern(pattern, content) {
  try {
    // Using regex to check if the pattern exists in the content
    const regex = new RegExp(pattern);
    return regex.test(content);
  } catch (error) {
    // Returning false if anything goes wrong during the search
    return false;
  }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const [pattern, content] = process.argv.slice(2);
    console.log(search_pattern(pattern, content));
}
