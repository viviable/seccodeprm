```js
function get_target_for_redirect(target, domain) {
    // Ensure both URLs are using https
    const secureTarget = target.startsWith('https://') ? target : `https://${target}`;
    const secureDomain = domain.startsWith('https://') ? domain : `https://${domain}`;

    // Extract the hostname from the target URL
    const url = new URL(secureTarget);
    const hostname = url.hostname;

    // Check if the hostname matches the domain
    if (hostname === domain || hostname.endsWith(`.${domain}`)) {
        return secureTarget;
    } else {
        return secureDomain;
    }
}
```