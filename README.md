# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## General Information
### Contributions
**Sam Lonneman** (s216999) - Experiments 1, 2, 3, & 5.

**Alana Vanzo** (s221952) - Experiments ____.

**Pedro Wachtendorff** (s221411) - Experiments ____.

### Final Solution
The code on this branch makes up our final solution. At the moment, it is an exact copy of the code found in the branch [`Experiment_5`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_1) because this was our best performing experiment so far, and it utilizes some concepts built up from previous experiments. First, this solution is multithreaded rather than multiprocess, a change first made during Experiment 1. Second, it is optimized for a 4 core system, a change first made during Experiment 2. Finally, it utilizes a max heap based request priority queue for handling the order of requests to process, a change first made during Experiment 5. For more detailed information on the rationale and effects of these changes, you can find all the experiment reports below, or individually on each experiment's branch.

### Benchmarking
Many of the experiments below make reference to a client run configuration called [`test.sh`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/test.sh), which can be found here in the root directory. This is the configuration used throughout many of the below experiments to gather scores for our new solutions and the benchmarks. It is nearly identical to the provided configuration [`run-client-milestone.sh`](https://github.com/dtu-ese/os-challenge-common/blob/master/run-client-milestone.sh), except that the difficulty and delay have each been decreased by a factor of 10. This configuration allowed us to perform many more meaningful tests within a reasonable amount of time.


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
The results of our experiment above show that on average Solution_1 scored slightly lower than Solution_0. However, after performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of 0.44, so we fail to reject the null hypothesis that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment. Therefore, the difference in average score of our samples is not statistically significant enough to suggest that there is any true difference in performance.

### Discussion
The conclusion provides evidence that simply replacing a multiprocess approach with a multithreaded approach does not significantly improve performance. This conclusion makes sense when we remember that we only expected a performance improvement by reducing the overhead of creating new processes. This is a very small difference, and more importantly, this very small improvement only occurs once per request (maybe just a few hundred times depending on the client run configuration). It seems likely that this improvement might only become significant with an arbitrarily large number of requests (far more than could realistically be tested). Furthermore, even though the results were insufficient for rejecting our null hypothesis, the experiment was still very important to our project because it laid the foundation for future experiments which are based on multithreading.

### Improvements
In the future, it would be helpful for such an experiment to have an automated script to run the client several times and automatically collect the results into a usable format instead of manually kicking off each run and recording the score. This would allow us to more practically increase our sample size and thus increase the statistical significance of our results. Also, both solutions currently allow for an unchecked number of threads/processes to run pseudo-concurrently, meaning a non-negligible amount of time is devoted to switching between them. Perhaps a solution could be devised which reduces the amount of time spent switching between threads/processes.


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
|    1    |      7329978       |      5275732       |
|    2    |      7446824       |      5052745       |
|    3    |      7434907       |      5294594       |
|    4    |      7267491       |      5840702       |
|    5    |      7502669       |      5948464       |
|    6    |      7244615       |      5674157       |
|    7    |      7194383       |      5152390       |
|    8    |      7323975       |      4940118       |
|    9    |      7113649       |      5904308       |
|   10    |      7451822       |      5163458       |
| Average |      7331031       |      5424667       |

### Conclusion
As shown by the results above, the average score of Solution_2 was lower than the average score of Solution_0. After performing a statistical hypothesis test with alpha=0.05, we received a P-value of less than 0.0001%, so we can certainly reject the null hypothesis that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that limiting the number of processes to the number of cores is statistically significantly faster than creating a new process for each request. This result makes sense and supports our hypothesis for the reason explained in our rationale.

### Improvements
In Solution_2, each process reverse hashes the request which is next in the FIFO queue from the client. Perhaps this solution could be improved by storing each request in a priority queue and instructing each process to take the request with the highest priority.


## Experiment 3: Priority Scheduling with "Nice Values"

### Background
The goal of this experiment is to improve performance by prioritizing high priority requests as designated by the client. To do so, we have implemented a solution which takes advantage of the Linux concept of "nice values". We refer to this solution as Solution_3, and its code can be found on a branch called [`Experiment_3`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_3). Our first working implementation will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_2 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Rationale
Solution_0 starts a new process for each request, ignoring the priority value provided by the client. Solution_3 also starts a new process for each request, but it uses this provided priority value to adjust the "nice value" of each process. In Linux, the "nice value" of a process corresponds to how "nice" it is to other processes. To make this more concrete, a process with a higher nice value is less likely to be selected when a CPU switches to a new process. The result is that by increasing the nice value of a process, we can effectively lower is priority. In Solution_3, we increase the nice value of each process by 16-p (since it is often unwise to decrease the priority of a user program). This means the nice value of a process with priority p=16 is unchanged, the nice value of a process with p=15 is increased by 1, etc. By making this adjustment, higher priority requests are now more likely to be selected than lower priority requests each time a core switches processes, resulting in a lower latency. Since the score for each request is proportional to the priority of the request, we expect Solution_3 will have a lower total score, as reflected in our alternative hypothesis.

### Variables
The independent variable in this experiment is whether request priority is ignored (Solution_0) or handled by adjusting nice values (Solution_3). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run `test.sh` 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

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
As shown by the results above, the average score of Solution_3 was lower than the average score of Solution_0. After performing a statistical hypothesis test with alpha=0.05, we received a P-value of less than 0.0001%, so we can certainly reject the null hypothesis that Solution_3 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that priority scheduling with "nice values" is statistically significantly faster than ignoring priority. This result makes sense. After all, the same number of processes is created in the same order as before. The only difference is that the scheduler now selects higher priority requests more often than lower priority requests, improving performance without any significant downside. Note that this method is only useful in situations where there are more processes than available cores, a situation which may already be undesirable due to the increased time spent switching between processes. So, it seems likely that other solutions will be better for our final implementation.


## Experiment 5: Request Priority Queue

### Background
The goal of this experiment is to optimize the server such that requests are processed in order of priority using a request priority queue (implemented as a custom max heap data structure). This solution is built off of Solution_1, our first naive multithreaded solution. This new solution takes it a step further, limiting the number of worker threads to the number of cores to reduce excess switching. Thus, it can also be said to draw inspiration from Solution_2, where we implemented this same concept with processes. We refer to this new solution as Solution_5, and it's code can be found on a branch called [Experiment_5](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_5). Since we expect this new solution to be a direct improvement to Solution_2, we will use Solution_2 as the baseline for this experiment. The code for Solution_2 can be found on a branch called [Experiment 2](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_2).

### Hypothesis
We hypothesize that Solution_5 will run faster (score lower) than Solution_2 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_5 will run slower than or equal to (score higher than or equal to) Solution_2 in the test environment.

### Rationale
Solution_2 works such that each time one of the four worker processes finishes processing a request, it accepts the next request available from the client and begins processing it. This means that while Solution_2 is optimized for a four core system, it does not take request priority into account, instead acting as a FIFO system. Solution_5 aims to have a similar format except that each time one of the 4 worker threads completes a request, it begins processing the next highest priority request available. This is made possible by having an additional thread simply responsible for accepting requests as soon as they are sent by the client and pushing them all into a priority queue (max heap). This custom data structure always maintains a state such that the request on top of the heap is the earliest request received of the highest priority which has not yet been processed. Each time a thread finishes processing a request, it simply extracts the top of the shared priority queue and processes it. We are confident that this solution will perform well because of the max heap's fast O(log n) insertion and extraction time complexity where n is the number of requests left to be processed.

### Variables
The independent variable in this experiment is whether the solution is optimized with a request priority queue or not (Solution_5 vs Solution_2). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run test.sh 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_2) | Score (Solution_5) |
|:-------:|:------------------:|:------------------:|
|    1    |      8578340       |      7576153       |
|    2    |      8728867       |      7633813       |
|    3    |      8588195       |      7653776       |
|    4    |      8662969       |      7785060       |
|    5    |      8615476       |      7911605       |
|    6    |      8679457       |      7504506       |
|    7    |      9096115       |      7721508       |
|    8    |      9033275       |      7998727       |
|    9    |      8662748       |      7651104       |
|   10    |      8705140       |      7755325       |
| Average |      8735058       |      7719158       |

### Conclusion
As shown by the results above, the average score of Solution_5 was lower than the average score of Solution_2. After performing a statistical hypothesis test with alpha=0.05, we received a P-value of less than 0.0001%, so we can certainly reject the null hypothesis that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that using a max heap priority queue to process the highest priority requests first yields statistically significantly better performance than a solution which processes requests in a FIFO manner indifferent to priority. This result makes sense and supports our hypothesis for the reason explained in our rationale.

Also note, ideally we would take advantage of a max heap's constant O(1) time complexity for peeking at the top without deletion. However, it is essential that the request is fully deleted from the heap before any request begins processing it to ensure that two requests won't be assigned to the same job, which would hurt our reliability. So, it is in our best interest to use an extraction method which retrieves and deletes all at once, which unfortunately requires at least O(log n) time. Regardless, O(log n) retrieval time is still very fast especially considering that n only grows to around 1000 at most, within reasonable conditions like those in the final submission run.

### Improvements
While this solution provides more improvements and is our best implementation so far, it still pays no attention to repeated requests. Perhaps this could still be improved by implementing some kind of cache shared between the worker threads.
