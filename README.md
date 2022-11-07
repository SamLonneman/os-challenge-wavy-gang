# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 1: Multithreading

### Background
The goal of this experiment is to determine whether multithreading is faster than using separate processes for each request. We refer to the new multithreaded solution as Solution_1, and its code can be found on a branch called [`Experiment_1`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_1). Our first working implementation is a multiprocess solution, and it will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_1 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Rationale
Solution_0 has one process which constantly accepts incoming requests and forks off new processes to immediately begin handling each one. Solution_1 has one thread which constantly accepts incoming requests and starts new threads to immediately begin handling each one. We hypothesize that Solution_1 will run faster than Solution_0 simply due to assumption that creating a new thread is computationally faster than starting a new process because less memory needs to be copied since threads share a process's memory while forked processes require the entire process to be copied.

### Variables
The independent variable in this experiment is whether new threads are created for each request (Solution_1) or new processes are forked for each request (Solution_0). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run `test.sh` 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_0) | Score (Solution_1) |
|:-------:|:------------------:|:------------------:|
|    1    |      7265388       |      7289546       |
|    2    |      7425212       |      7130323       |
|    3    |      7383798       |      7339676       |
|    4    |      7254606       |      7337064       |
|    5    |      7077907       |      7014741       |
|    6    |      7214552       |      6987736       |
|    7    |      7174563       |      7351505       |
|    8    |      7319923       |      7571753       |
|    9    |      6948300       |      7588138       |
|   10    |      7608815       |      6925995       |
| Average |      7267306       |      7253648       |

### Conclusion
The results of our experiment above show that on average Solution_1 scored slightly lower than Solution_0. However, after performing a statistical hypothesis test with alpha=0.05, we received a P-value of 0.44, so we fail to reject the null hypothesis that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that simply replacing a multiprocess approach with a multithreaded approach does not significantly improve performance. This conclusion makes sense when we remember that we only expected a performance improvement by reducing the overhead of creating new processes. This is a very small difference, and more importantly, this very small improvement only occurs once per request (maybe just a few hundred times depending on the client run configuration). It seems likely that this improvement might only become significant with an arbitrarily large number of requests (far more than could realistically be tested). Furthermore, even though the results were insufficient for rejecting our null hypothesis, the experiment was still very important to our project because it laid the foundation for future experiments which are based on multithreading.

### Improvements
In the future, it would be helpful for such an experiment to have an automated script to run the client several times and automatically collect the results into a usable format instead of manually kicking off each run and recording the score. This would allow us to more practically increase our sample size and thus increase the statistical significance of our results. Also, both solutions currently allow for an unchecked number of threads/processes to run pseudo-concurrently, meaning a non-negligible amount of time is devoted to switching between them. Perhaps a solution could be devised which reduces the amount of time spent switching between threads/processes.