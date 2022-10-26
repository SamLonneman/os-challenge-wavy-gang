# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 1: Multithreading

### Background
The goal of this experiment is to determine whether multithreading is faster than using separate processes for each request. The code for this experiment's implementation can be found on a branch called [`Experiment_1`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_1). Our first working implementation for the milestone can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone), and it will serve as the control in this experiment.

### Hypothesis
We hypothesize that the implementation which uses one thread per request will score lower (run faster) than the implementation which uses one process per request. We hypothesize this due to the assumption that creating a new thread is computationally easier than starting a new process because threads share a process's memory, so less memory needs to be copied for each request. The null hypothesis for our experiment is that the thread-based approach will score greater than or equal to (run slower than or equal to) the process-based approach.

### Variables
The independent variable in this experiment is whether processes or threads are used to process each request. The dependent variable will be the resulting score returned by the client using the "milestone" run configuration. In both cases, we will use a processing structure in which the parent thread/process accepts connections from the client and produces a new thread/process for each request to calculate and return a response with default pseudo-concurrent scheduling. Furthermore, when testing the performance of each method, an identical run configuration will be used. All tests in this experiment will be run on the same virtual environment running on the same physical computer.

### Method
For both implementations of the server, we will run `run-client-milestone.sh` 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (process-based implementation) | Score (thread-based implementation) |
|:-------:|:------------------------------------:|:-----------------------------------:|
|    1    |               11528773               |              15632364               |
|    2    |               14962546               |              18699715               |
|    3    |               9649223                |              16237716               |
|    4    |               20546787               |              18379834               |
|    5    |               8087227                |               9944169               |
|    6    |               13734484               |              15402195               |
|    7    |               19296649               |              18895384               |
|    8    |               5059456                |              10246281               |
|    9    |               15261579               |               7966930               |
|   10    |               12454365               |              11688490               |
| Average |               13058109               |              14309308               |

### Conclusion
The results of our experiment above show a slight increase in score for the thread based implementation when compared with the process based implementation. Given this fact, it is unsurprising that after performing a one-tailed statistical hypothesis test on this data with alpha=0.05 (as constructed in the Hypothesis section), we failed to reject the null hypothesis that the thread-based implementation would score greater than or equal to (run slower than or equal to) the process-based approach. This means that we cannot conclude that the thread based implementation is faster than the process based implementation.

### Discussion
The reader might now wonder whether the thread based implementation is actually _slower_ than the original process based implementation (opposite to our original hypothesis). However, after performing a new (opposite) statistical hypothesis test with alpha=0.05, we found that despite the average scores differing slightly from one another, there is insufficient evidence to conclude that the thread based implementation is slower than the process based implementation. In other words, the difference in scores is not large enough (given our sample size) to be considered a statistically significant difference. Furthermore, Even though the results were insufficient for rejecting our original null hypothesis, the experiment was still very important to our project. This is because many of our future experiments will be based on a thread-based implementation rather than a process-based implementation since we are more familiar with this type of programming.

### Improvements
In the future, it would be helpful for such an experiment to have an automated script to run the client several times and automatically collect the results into a usable format instead of manually kicking off each run and recording the score. This would allow us to more practically increase our sample size and thus increase the statistical significance of our results.