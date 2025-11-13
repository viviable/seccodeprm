```js
function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is valid
    if (!subdomainPrefix || !/^[a-zA-Z0-9.-]+$/.test(subdomainPrefix)) {
        return `https://${domain}`;
    }

    // Construct the URL with the HTTPS protocol
    return `https://${subdomainPrefix}.${domain}`;
}
```