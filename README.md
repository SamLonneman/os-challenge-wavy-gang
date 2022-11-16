# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 1: Multithreading - Sam Lonneman (s216999)

### Background
The goal of this experiment is to determine whether multithreading is faster than using separate processes for each request. We refer to the new multithreaded solution as Solution_1, and its code can be found on a branch called [`Experiment_1`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_1). Our first working implementation is a multiprocess solution, and it will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_1 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Rationale
Solution_0 has one process which constantly accepts incoming requests and forks off new processes to immediately begin handling each one. Solution_1 has one thread which constantly accepts incoming requests and starts new threads to immediately begin handling each one. We hypothesize that Solution_1 will run faster than Solution_0 simply due to assumption that creating a new thread is computationally faster than starting a new process because less memory needs to be copied since threads share a process's memory while forked processes require the entire process to be copied.

### Variables
The independent variable in this experiment is whether new threads are created for each request (Solution_1) or new processes are forked for each request (Solution_0). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run [`test.sh`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/test.sh) on the client 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_0) | Score (Solution_1) |
|:-------:|:------------------:|:------------------:|
|    1    |      7021784       |      7069874       |
|    2    |      7144965       |      7147021       |
|    3    |      7039684       |      6867306       |
|    4    |      6956718       |      7078444       |
|    5    |      6913224       |      6951947       |
|    6    |      7096031       |      6994614       |
|    7    |      6835940       |      6941992       |
|    8    |      7026563       |      7120237       |
|    9    |      6934364       |      6875972       |
|   10    |      6900494       |      6939558       |
| Average |      6986977       |      6998697       |

### Conclusion
The results of our experiment above show that on average Solution_1 scored very similarly to Solution_0. Furthermore, after performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of 0.7912, so we fail to reject the null hypothesis that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment. Therefore, the difference in average score of our samples is not statistically significant enough to suggest that there is any true difference in performance.

### Discussion
The conclusion provides evidence that simply replacing a multiprocess approach with a multithreaded approach does not significantly improve performance. This conclusion makes sense when we remember that we only expected a performance improvement by reducing the overhead of creating new processes. This is a very small difference, and more importantly, this very small improvement only occurs once per request (maybe just a thousand times depending on the client run configuration). It seems likely that if there is some improvement as described in the rationale, it would require an impractically large number of requests to find a measurable improvement. Furthermore, even though the results were insufficient for rejecting our null hypothesis, the experiment was still very important to our project because it laid the foundation for future experiments which will be based on multithreading.

### Improvements
In the future, it would be helpful for such an experiment to have an automated script to run the client several times and automatically collect the results into a usable format instead of manually kicking off each run and recording the score. This would allow us to more practically increase our sample size and thus increase the statistical significance of our results. Also, both solutions currently allow for an unchecked number of threads/processes to run pseudo-concurrently, meaning a non-negligible amount of time is devoted to switching between them. Perhaps a solution could be devised which reduces the amount of time spent switching between threads/processes.