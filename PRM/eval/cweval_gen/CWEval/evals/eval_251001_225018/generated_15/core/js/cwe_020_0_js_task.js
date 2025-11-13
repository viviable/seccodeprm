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
