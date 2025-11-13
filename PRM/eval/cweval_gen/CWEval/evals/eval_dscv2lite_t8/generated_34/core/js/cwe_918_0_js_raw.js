```javascript
function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is empty or contains invalid characters
    if (!subdomainPrefix || !/^[a-zA-Z0-9][a-zA-Z0-9-]*$/.test(subdomainPrefix)) {
        return `https://${domain}`;
    }

    // Construct the full URL with the HTTPS protocol
    return `https://${subdomainPrefix}.${domain}`;
}
```