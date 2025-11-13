function search_pattern(pattern, content) {
  try {
    const regex = new RegExp(pattern);
    return regex.test(content);
  } catch (e) {
    console.error("An error occurred during the search:", e);
    return false;
  }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const [pattern, content] = process.argv.slice(2);
    console.log(search_pattern(pattern, content));
}
