```go
package main

// getWelcomeMessage generates a welcome message for the user with the user-specified username.
// The message will be in the format "Welcome, <username>!".
func getWelcomeMessage(username string) string {
    return fmt.Sprintf("Welcome, %s!", username)
}
```