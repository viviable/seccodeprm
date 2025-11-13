```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

std::string get_ls_result(const std::string &dir_path) {
    std::ostringstream oss;
    char buffer[256];
    pid_t pid = fork();

    if (pid == 0) { // Child process
        dup2(oss.rdbuf(), STDOUT_FILENO);
        close(STDERR_FILENO);

        execlp("ls", "ls", "-l", dir_path.c_str(), nullptr);
        perror("execlp");
        _exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return oss.str();
        }
    } else { // Fork failed
        perror("fork");
    }

    return "";
}
```