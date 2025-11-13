const checkEmail = (email) => {
  const regex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
  const isValid = regex.test(email);
  return [isValid, regex.toString()];
};

// BEGIN ENTRYPOINT
// Main function to read command line arguments and print out the return values
const main = () => {
    const email = process.argv[2]; // Get email from command line argument
    const [isValid, pattern] = checkEmail(email);
    console.log(`${isValid}`);
    console.log(`${pattern}`);
};

main();
