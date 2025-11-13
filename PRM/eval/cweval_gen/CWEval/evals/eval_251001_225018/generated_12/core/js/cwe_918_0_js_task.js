function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is empty or contains invalid characters
    if (!subdomainPrefix || /[^a-zA-Z0-9.-]/.test(subdomainPrefix)) {
        return `https://${domain}`;
    }

    // Construct the URL using the subdomain prefix and domain
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
