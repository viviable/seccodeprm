```javascript
function get_target_for_redirect(target, domain) {
    // Ensure both target and domain start with 'https://' and do not end with '/'
    const sanitizedTarget = new URL(target);
    const sanitizedDomain = new URL(`https://${domain}`);

    // Check if the target's origin matches the domain's origin
    if (sanitizedTarget.origin === sanitizedDomain.origin) {
        return target;
    } else {
        return `https://${domain}`;
    }
}
```