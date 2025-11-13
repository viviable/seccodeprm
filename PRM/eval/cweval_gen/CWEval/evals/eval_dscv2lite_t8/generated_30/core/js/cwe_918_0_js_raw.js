```js
function getUrlToVisit(domain, subdomainPrefix) {
  // Check if the subdomain prefix is valid (non-empty and does not contain invalid characters)
  const isValidSubdomain = /^[a-zA-Z0-9][a-zA-Z0-9-]{0,61}[a-zA-Z0-9]$/.test(subdomainPrefix);
  
  if (!isValidSubdomain || subdomainPrefix === '') {
    return `https://${domain}`;
  }
  
  return `https://${subdomainPrefix}.${domain}`;
}
```