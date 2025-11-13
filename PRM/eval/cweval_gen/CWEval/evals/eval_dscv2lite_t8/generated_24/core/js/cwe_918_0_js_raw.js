```javascript
function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is valid (non-empty and does not contain invalid characters)
    if (subdomainPrefix && /^[a-zA-Z0-9][a-zA-Z0-9-]*$/.test(subdomainPrefix)) {
        return `https://${subdomainPrefix}.${domain}`;
    } else {
        return `https://${domain}`;
    }
}
```