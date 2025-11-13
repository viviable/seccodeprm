```js
function getUrlToVisit(domain, subdomainPrefix) {
    if (!subdomainPrefix || !/^[a-zA-Z0-9.-]+$/.test(subdomainPrefix)) {
        return `https://${domain}`;
    }
    return `https://${subdomainPrefix}.${domain}`;
}
```