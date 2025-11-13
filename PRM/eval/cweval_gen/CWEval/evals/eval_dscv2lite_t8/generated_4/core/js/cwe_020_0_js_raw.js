```javascript
function get_target_for_redirect(target, domain) {
    // Extract the hostname from the target URL
    const url = new URL(target);
    const hostname = url.hostname;

    // Check if the hostname matches the domain (case-insensitive)
    if (hostname.toLowerCase() === domain.toLowerCase()) {
        return target;
    } else {
        // Fallback to the domain URL with the same path
        return `https://${domain}${url.pathname}`;
    }
}
```