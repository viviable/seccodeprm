```js
function getUrlToVisit(domain, subdomainPrefix) {
  // Check if the subdomain prefix is valid (non-empty and does not contain invalid characters)
  const isValidSubdomain = /^[a-zA-Z0-9-]+$/.test(subdomainPrefix);

  if (isValidSubdomain && subdomainPrefix.trim() !== '') {
    return `https://${subdomainPrefix}.${domain}`;
  } else {
    return `https://${domain}`;
  }
}
```