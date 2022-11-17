# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 4: Caché

### Background
The goal of this experiment is to determine whether using a caché is faster than not using one. We refer to the new solution as Solution_4, and its code can be found on a branch called [`Experiment_4_v3`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_4_v3). Our second working implementation is a multithreading solution, and it will serve as the control for this experiment. We refer to this control solution as Solution_1, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_1).

### Hypothesis
We hypothesize that Solution_4 will run faster (score lower) than Solution_1 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_4 will run slower than or equal to (score higher than or equal to) Solution_1 in the test environment.

### Rationale
Solution_1 has one thread which constantly accepts incoming requests and starts new thread to immediately begin handling each one. Solution_4 works with the same idea, but previously handling each request it asked to a caché for known values that were already proccessed before. We hypothesize that Solution_4 will run faster than Solution_1 simply due to assumption that accesing to known values in a conveniently restricted memory store is considerably faster than processing the same repeated value. Therefore, this will be better in the entire request process.

### Variables
The independent variable in this experiment is whether the solution is optimized including a cache or not (Solution_4 vs Solution_1). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run `test.sh` 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_0) | Score (Solution_1) |
|:-------:|:------------------:|:------------------:|
|    1    |      3980713       |      2752393       |
|    2    |      3625083       |      3474491       |
|    3    |      3595248       |      2909007       |
|    4    |      3492727       |      2316086       |
|    5    |      3924176       |      3704961       |
|    6    |      3943407       |      3371207       |
|    7    |      3489506       |      3392503       |
|    8    |      3715929       |      2684450       |
|    9    |      3545475       |      2086809       |
|   10    |      3862122       |      2058934       |
| Average |      3717439       |      2875084       |

### Conclusion
The results of our experiment above show that on average Solution_1 scored slightly lower than Solution_0. However, after performing a statistical hypothesis test with alpha=0.05, we received a P-value of 0.44, so we fail to reject the null hypothesis that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that simply replacing a multiprocess approach with a multithreaded approach does not significantly improve performance. This conclusion makes sense when we remember that we only expected a performance improvement by reducing the overhead of creating new processes. This is a very small difference, and more importantly, this very small improvement only occurs once per request (maybe just a few hundred times depending on the client run configuration). It seems likely that this improvement might only become significant with an arbitrarily large number of requests (far more than could realistically be tested). Furthermore, even though the results were insufficient for rejecting our null hypothesis, the experiment was still very important to our project because it laid the foundation for future experiments which are based on multithreading.

### Improvements
In the future, it would be helpful for such an experiment to have an automated script to run the client several times and automatically collect the results into a usable format instead of manually kicking off each run and recording the score. This would allow us to more practically increase our sample size and thus increase the statistical significance of our results. Also, both solutions currently allow for an unchecked number of threads/processes to run pseudo-concurrently, meaning a non-negligible amount of time is devoted to switching between them. Perhaps a solution could be devised which reduces the amount of time spent switching between threads/processes.