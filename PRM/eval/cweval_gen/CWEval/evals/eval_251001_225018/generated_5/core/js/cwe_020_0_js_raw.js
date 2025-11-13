```js
function get_target_for_redirect(target, domain) {
    // Extract the hostname from the target URL
    const url = new URL(target);
    const hostname = url.hostname;

    // Check if the hostname matches the domain or is a subdomain of the domain
    if (hostname === domain || hostname.endsWith(`.${domain}`)) {
        return target;
    } else {
        // Fallback to the domain URL with https protocol
        return `https://${domain}`;
    }
}
```