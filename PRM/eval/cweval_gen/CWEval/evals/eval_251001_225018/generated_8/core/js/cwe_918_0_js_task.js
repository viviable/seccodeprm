function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is valid
    const isValidSubdomain = /^[a-zA-Z0-9-]+$/.test(subdomainPrefix);
    
    // If the subdomain is valid, construct the URL with the subdomain
    if (isValidSubdomain) {
        return `https://${subdomainPrefix}.${domain}`;
    }
    
    // If the subdomain is not valid, return the base domain
    return `https://${domain}`;
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
