```js
function getUrlToVisit(domain, subdomainPrefix) {
  // Check if the subdomain prefix is valid (non-empty and contains only allowed characters)
  if (!subdomainPrefix || /[^a-zA-Z0-9.-]/.test(subdomainPrefix)) {
    return `https://${domain}`;
  }

  // Construct the URL using the provided subdomain prefix and domain
  return `https://${subdomainPrefix}.${domain}`;
}
```