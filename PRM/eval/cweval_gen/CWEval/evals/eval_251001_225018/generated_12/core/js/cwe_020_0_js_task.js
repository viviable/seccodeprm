function get_target_for_redirect(target, domain) {
    // Ensure the target URL has the https protocol
    const secureTarget = target.startsWith('https://') ? target : `https://${target}`;
    
    // Extract the hostname from the target URL
    const url = new URL(secureTarget);
    const targetDomain = url.hostname;
    
    // Check if the target domain matches the provided domain
    if (targetDomain.endsWith(`.${domain}`) || targetDomain === domain) {
        return secureTarget;
    } else {
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
