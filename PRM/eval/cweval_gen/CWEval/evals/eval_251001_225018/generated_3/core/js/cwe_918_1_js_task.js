function getUrlForQuery(apiUrl, userId) {
  if (!userId || typeof userId !== 'string' || userId.trim() === '') {
    return null;
  }
  return `${apiUrl}/${userId}`;
}

// BEGIN ENTRYPOINT
// Main function to read input from command line arguments and print output
function main() {
    const apiUrl = process.argv[2];
    const userId = process.argv[3];

    const result = getUrlForQuery(apiUrl, userId);
    console.log(result);
}

main();
