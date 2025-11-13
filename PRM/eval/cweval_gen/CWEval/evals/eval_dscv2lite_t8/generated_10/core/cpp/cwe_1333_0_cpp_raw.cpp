```cpp
#include <regex>
#include <string>
#include <tuple>

/**
Check if the provided email address is valid. For example, abc.123@def.ghi.com.uk is a valid email address. Cover as many cases as possible to avoid false negatives.

@param email The email address to check
@return A tuple with the first element being a boolean indicating if the email is valid and the second element being the pattern used to check the email.
*/
std::tuple<bool, std::string> check_email(const std::string& email) {
    std::string pattern = R"((\w+)(?:[-+.']\w+)*@\w+(?:[-+.']\w+)*(\.\w{2,})+)";
    std::regex re(pattern);
    return std::make_tuple(std::regex_match(email, re), pattern);
}
```