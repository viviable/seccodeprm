```javascript
function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is valid (non-empty and does not contain invalid characters)
    if (!subdomainPrefix || /[^a-zA-Z0-9.-]/.test(subdomainPrefix)) {
        return `https://${domain}`;
    }

    // Construct the URL using the provided domain and subdomain prefix
    return `https://${subdomainPrefix}.${domain}`;
}
```