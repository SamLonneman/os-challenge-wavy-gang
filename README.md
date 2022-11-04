# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 2: Optimization for Multi-Core Processor

### Background
The goal of this experiment is to take advantage of the fact that our server will be run on a machine with a multi-core processor. Furthermore, we will devise a solution which is optimized for 4 cores since this is the specification of the machine used to benchmark our server. We refer to this solution as Solution_2, and it's code can be found on a branch called [`Experiment_2`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_2). Our first working implementation will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_2 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Rationale
Solution_0 works such that a single process continuously awaits requests from the client and forks a new process to calculate and return a result for each incoming request. This means that it is possible for there to be an arbitrarily large number of processes running pseudo-concurrently. However, once there are more processes running than cores available, the cores will regularly switch between processes, which consumes a non-negligible amount of time. As we increase the number of processes running pseudo-concurrently, this time spent switching begins to add up, hurting the performance of our server. In Solution_2, we hope to reduce this time spent switching by only having up to four processes running concurrently at any time. We implemented this by forking into four processes. Each process runs concurrently on its own core, first picking up a request, calculating and returning the result to the client, then picking up a new request and repeating these steps. This helps to ensure that each core is allowed to spend more time reverse hashing relative to the time spent switching between processes, hopefully improving the overall performance of our server.

### Variables
The independent variable in this experiment is whether the solution is optimized for a 4 core system (Solution_0 vs Solution_2). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run `test.sh` 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_0) | Score (Solution_2) |
|:-------:|:------------------:|:------------------:|
|    1    |                    |                    |
|    2    |                    |                    |
|    3    |                    |                    |
|    4    |                    |                    |
|    5    |                    |                    |
|    6    |                    |                    |
|    7    |                    |                    |
|    8    |                    |                    |
|    9    |                    |                    |
|   10    |                    |                    |
| Average |                    |                    |

### Conclusion


### Discussion


### Improvements
In Solution_2, each process reverse hashes the request which is next in the FIFO queue from the client. Perhaps this solution could be improved by storing each request in a priority queue and instructing each process to take the request with the highest priority.