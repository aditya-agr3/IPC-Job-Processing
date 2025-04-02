#include<bits/stdc++.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

struct Worker {
    int jobType;
    int pid;
    int write_fd; // Parent writes job duration here
    int read_fd;  // Worker writes completion signal here
};

// Function to make pipes non-blocking
void make_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int numWorkers;
    vector<Worker> workers;

    // Step 1: Read Worker Configuration
    cin >> numWorkers;
    for (int i = 0; i < numWorkers; i++) {
        int jobType, count;
        cin >> jobType >> count;

        for (int j = 0; j < count; j++) {
            int parent_to_child[2], child_to_parent[2];
            pipe(parent_to_child);
            pipe(child_to_parent);

            pid_t pid = fork();
            if (pid == 0) {
                // Worker process
                close(parent_to_child[1]); // Close writing end of parent_to_child
                close(child_to_parent[0]); // Close reading end of child_to_parent

                while (true) {
                    int jobDuration;
                    if (read(parent_to_child[0], &jobDuration, sizeof(jobDuration)) > 0) {
                        sleep(jobDuration); // Simulate work
                        write(child_to_parent[1], &jobType, sizeof(jobType)); // Notify completion
                    }
                }
                exit(0);
            } else {
                // Main process
                close(parent_to_child[0]); // Close reading end of parent_to_child
                close(child_to_parent[1]); // Close writing end of child_to_parent

                make_non_blocking(child_to_parent[0]); // Make read pipe non-blocking

                workers.push_back({jobType, pid, parent_to_child[1], child_to_parent[0]});
            }
        }
    }

    // Step 2: Handle Incoming Jobs
    fd_set read_fds;
    vector<pair<int, int>> jobQueue; // (jobType, duration)

    int jobType, duration;
    while (cin >> jobType >> duration) {
        jobQueue.emplace_back(jobType, duration);
    }

    // Step 3: Process Jobs Using `select()`
    while (!jobQueue.empty()) {
        FD_ZERO(&read_fds);
        int max_fd = 0;

        for (const auto& worker : workers) {
            FD_SET(worker.read_fd, &read_fds);
            max_fd = max(max_fd, worker.read_fd);
        }

        struct timeval timeout = {1, 0}; // 1 second timeout
        int ready = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);

        if (ready > 0) {
            for (auto& worker : workers) {
                if (FD_ISSET(worker.read_fd, &read_fds)) {
                    int completedJobType;
                    read(worker.read_fd, &completedJobType, sizeof(completedJobType));

                    // Assign new job if available
                    auto it = find_if(jobQueue.begin(), jobQueue.end(),
                                      [&](const pair<int, int>& job) { return job.first == worker.jobType; });

                    if (it != jobQueue.end()) {
                        write(worker.write_fd, &it->second, sizeof(it->second));
                        jobQueue.erase(it);
                    }
                }
            }
        }
    }

    // Step 4: Cleanup
    for (const auto& worker : workers) {
        close(worker.write_fd);
        close(worker.read_fd);
        kill(worker.pid, SIGTERM);
        waitpid(worker.pid, nullptr, 0);
    }

    return 0;
}
