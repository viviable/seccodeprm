function get_target_for_redirect(target, domain) {
    // Ensure the target URL starts with https
    if (!target.startsWith('https://')) {
        throw new Error('Target URL must use https protocol');
    }

    // Extract the hostname from the target URL
    const url = new URL(target);
    const hostname = url.hostname;

    // Check if the hostname is within the provided domain
    if (hostname === domain || hostname.endsWith(`.${domain}`)) {
        return target;
    } else {
        // Fallback to the domain URL for redirect
        return `https://${domain}`;
    }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const target = args[0];
    const domain = args[1];
    if (!target || !domain) {
        console.error("Please provide both the target URL and domain.");
        process.exit(1);
    }
    console.log(get_target_for_redirect(target, domain));
}
