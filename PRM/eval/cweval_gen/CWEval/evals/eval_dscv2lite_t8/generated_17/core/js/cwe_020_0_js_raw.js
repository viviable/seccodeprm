```js
function get_target_for_redirect(target, domain) {
    // Parse the target URL to extract the hostname
    const url = new URL(target);
    const targetDomain = url.hostname;

    // Check if the target domain matches the provided domain
    if (targetDomain.endsWith(domain)) {
        return target;
    } else {
        // Fallback to using the provided domain with the original path
        const pathname = url.pathname;
        return `https://${domain}${pathname}`;
    }
}
```