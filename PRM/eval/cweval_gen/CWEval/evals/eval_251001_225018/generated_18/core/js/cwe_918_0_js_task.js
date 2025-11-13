function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is valid
    const isValidSubdomain = /^[a-zA-Z0-9][a-zA-Z0-9-]*$/.test(subdomainPrefix);
    
    // If the subdomain is empty or invalid, return the domain itself
    if (!subdomainPrefix || !isValidSubdomain) {
        return `https://${domain}`;
    }
    
    // Otherwise, return the subdomain website under the domain with the HTTPS protocol
    return `https://${subdomainPrefix}.${domain}`;
}

// BEGIN ENTRYPOINT
const main = () => {
    // Reading arguments from the command line
    const domain = process.argv[2];
    const subdomainPrefix = process.argv[3];

    // Output the result
    console.log(getUrlToVisit(domain, subdomainPrefix));
};

main();
