```javascript
function get_target_for_redirect(target, domain) {
    // Parse the target URL
    const parsedTarget = new URL(target);
    
    // Ensure the protocol is https
    if (parsedTarget.protocol !== 'https:') {
        return `https://${domain}`;
    }
    
    // Extract the hostname from the target URL
    const targetHostname = parsedTarget.hostname;
    
    // Check if the target hostname matches the domain (ignoring subdomains)
    if (targetHostname.endsWith(`.${domain}`) || targetHostname === domain) {
        return target;
    } else {
        return `https://${domain}`;
    }
}
```