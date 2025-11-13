```js
function get_target_for_redirect(target, domain) {
    // Extract the hostname from the target URL
    const targetUrl = new URL(target);
    const targetHostname = targetUrl.hostname;

    // Construct the full domain URL with https protocol
    const domainUrl = `https://${domain}`;

    // Check if the target hostname matches the domain (ignoring subdomains)
    if (targetHostname === domain || targetHostname.endsWith(`.${domain}`)) {
        return target;
    } else {
        return domainUrl;
    }
}
```