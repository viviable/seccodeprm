```js
function getUrlToVisit(domain, subdomainPrefix) {
    // Check if the subdomain prefix is valid (non-empty and contains only allowed characters)
    const subdomainRegex = /^[a-zA-Z0-9.-]+$/;
    if (!subdomainRegex.test(subdomainPrefix)) {
        return `https://${domain}`;
    }

    // Construct the full URL using the provided subdomain prefix and domain
    return `https://${subdomainPrefix}.${domain}`;
}
```