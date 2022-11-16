# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 3: Priority Scheduling with "Nice Values" - Sam Lonneman (s216999)

### Background
The goal of this experiment is to improve performance by prioritizing high priority requests as designated by the client. To do so, we have implemented a solution which takes advantage of the Linux concept of "nice values". We refer to this solution as Solution_3, and its code can be found on a branch called [`Experiment_3`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_3). Our first working implementation will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_2 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Rationale
Solution_0 starts a new process for each request, ignoring the priority value provided by the client. Solution_3 also starts a new process for each request, but it uses this provided priority value to adjust the "nice value" of each process. In Linux, the "nice value" of a process corresponds to how "nice" it is to other processes. To make this more concrete, a process with a higher nice value is less likely to be selected when a CPU switches to a new process. The result is that by increasing the nice value of a process, we can effectively lower is priority. In Solution_3, we increase the nice value of each process by 16-p (since it is often unwise to decrease the priority of a user program). This means the nice value of a process with priority p=16 is unchanged, the nice value of a process with p=15 is increased by 1, etc. By making this adjustment, higher priority requests are now more likely to be selected than lower priority requests each time a core switches processes, resulting in a lower latency. Since the score for each request is proportional to the priority of the request, we expect Solution_3 will have a lower total score, as reflected in our alternative hypothesis.

### Variables
The independent variable in this experiment is whether request priority is ignored (Solution_0) or handled by adjusting nice values (Solution_3). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run [`test.sh`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/test.sh) on the client 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_0) | Score (Solution_3) |
|:-------:|:------------------:|:------------------:|
|    1    |      7329978       |      7087182       |
|    2    |      7446824       |      6939602       |
|    3    |      7434907       |      6763075       |
|    4    |      7267491       |      6864998       |
|    5    |      7502669       |      6870880       |
|    6    |      7244615       |      6896880       |
|    7    |      7194383       |      6977919       |
|    8    |      7323975       |      7065699       |
|    9    |      7113649       |      6715461       |
|   10    |      7451822       |      7045885       |
| Average |      7331031       |      6922758       |

### Conclusion
As shown by the results above, the average score of Solution_3 was lower than the average score of Solution_0. After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0001%, so we can certainly reject the null hypothesis that Solution_3 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that priority scheduling with "nice values" is statistically significantly faster than ignoring priority. This result makes sense. After all, the same number of processes is created in the same order as before. The only difference is that the scheduler now selects higher priority requests more often than lower priority requests, improving performance without any significant downside. Note that this method is only useful in situations where there are more processes than available cores, a situation which may already be undesirable due to the increased time spent switching between processes. So, it seems likely that other solutions will be better for our final implementation.