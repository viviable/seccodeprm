```js
function getUrlToVisit(domain, subdomainPrefix) {
  // Check if the subdomain prefix is empty or contains invalid characters
  if (!subdomainPrefix || !/^[a-zA-Z0-9.-]+$/.test(subdomainPrefix)) {
    return `https://${domain}`;
  }

  // Construct the URL using the subdomain prefix and domain
  return `https://${subdomainPrefix}.${domain}`;
}
```