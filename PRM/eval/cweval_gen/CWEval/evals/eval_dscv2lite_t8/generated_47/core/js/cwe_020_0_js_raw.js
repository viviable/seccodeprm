```javascript
function get_target_for_redirect(target, domain) {
    // Ensure both target and domain start with https
    const secureTarget = target.startsWith('https') ? target : `https://${target}`;
    const secureDomain = domain.startsWith('https') ? domain : `https://${domain}`;

    // Extract the hostname from the target URL
    const url = new URL(secureTarget);
    const hostname = url.hostname;

    // Check if the hostname matches the domain or any subdomain of the domain
    if (hostname === domain || hostname.endsWith(`.${domain}`)) {
        return secureTarget;
    } else {
        return `${secureDomain}/`;
    }
}
```