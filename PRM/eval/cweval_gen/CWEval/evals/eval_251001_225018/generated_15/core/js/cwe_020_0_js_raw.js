```js
function get_target_for_redirect(target, domain) {
    // Parse the target URL
    const parsedTarget = new URL(target);
    
    // Check if the hostname of the target URL matches the domain
    if (parsedTarget.hostname.endsWith(domain)) {
        return target;
    } else {
        // Fallback to the domain URL with the same path and search params
        return `https://${domain}${parsedTarget.pathname}${parsedTarget.search}`;
    }
}
```