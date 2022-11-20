# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

### Folder and Branch Structure
Here on [`master`](https://github.com/SamLonneman/os-challenge-wavy-gang), you can find the code for our final solution, along with this [`README.md`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/README.md) file which contains information about our final solution, followed by a copy of every experiment report. Note that this branch also includes a directory [`helpers`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/master/helpers) which contains additional header files referenced from [`server.c`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/server.c). We wrote each one of these ourselves except for [`messages.h`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/helpers/messages.h) which was provided to us in [`os-challenge-common`](https://github.com/dtu-ese/os-challenge-common). Furthermore, each other branch of this repository represents an experiment we performed, containing the source code along with the corresponding report.

### Final Solution
The code on this branch ([`master`](https://github.com/SamLonneman/os-challenge-wavy-gang)) makes up our final solution. Throughout development, we designed, performed, and reported on numerous individual experiments to gain insights about which strategies would be most effective for our final solution. Our very first working solution ([`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone)) is naive in hindsight, and we have continuously improved on it to implement the concepts deemed most effective by our experiments. First, this final solution is multithreaded as opposed to the original multiprocess solution, a practice first utilized during [`Experiment_1`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_1). Second, it is optimized for a 4 core processor such that the thread pool is limited to 4 threads to reduce excess switching, a change we first made during [`Experiment_2`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_2). It also takes advantage of caching to avoid re-processing repeated requests, which was a concept we first implemented during experiment [`Experiment_4_v3`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_4_v3). Finally, it utilizes a max-heap based priority queue for optimizing the order of requests left to process, a concept we first implemented during [`Experiment_5`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_5). For more detailed information about the motivations, rationales, and effects of these individual developments, you can find all the experiment reports below, or individually on each experiment's branch. Of course, there are also several experiments whose results were not deemed best for our final solution (but certainly helped us strengthen our methods and understanding), and they too can be found below.

A short summary of our final solution is as follows: one thread continuously accepts requests are they are sent from the client. Each time it receives a request, it checks in the shared cache to see if the request has been processed already. If it has, the cached result is immediately returned to the client. Otherwise, the request details are packaged into a request object and pushed into a shared priority queue which takes into account both their assigned priority and relative age. 4 worker threads repeatedly extract a request off the top of the shared priority queue and use brute force to determine the key which was used to create the hash before picking up the next request. Each time a new request is processed, its key-hash pair is placed in the shared cache for future use.

# Experiments

### Individual Experiment Contributions
**Sam Lonneman** (s216999) - Experiments 1, 2, 3, & 5.

**Alana Vanzo** (s221952) - Experiments 6.1 & 6.3 --> combined into experiment 6.

**Pedro Wachtendorff** (s221411) - Experiments ____.

### Benchmarking
Many of the experiments below make reference to a client run configuration called [`test.sh`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/test.sh), which can be found here in the root directory. This is the configuration used throughout many of the below experiments to gather scores for our new solutions and the benchmarks. It is nearly identical to the provided configuration [`run-client-milestone.sh`](https://github.com/dtu-ese/os-challenge-common/blob/master/run-client-milestone.sh), except that the difficulty and delay have each been decreased by a factor of 10. This configuration allowed us to perform many more meaningful tests within a reasonable amount of time. You may assume that this was the configuration used for benchmarking _**unless stated differently in an individual experiment report**_. Note that we strove to control all variables within an experiment so that we can accurately make comparisons between each solution and a baseline. However, because each experiment could have been done on a different day on a different computer (potentially with a different run configuration if stated), results from one experiment should not be compared with results from another experiment to make any conclusions about performance.

## Experiment 1: Multithreading - Sam Lonneman (s216999)

### Background
The goal of this experiment is to determine whether multithreading is faster than using separate processes for each request. We refer to the new multithreaded solution as Solution_1, and its code can be found on a branch called [`Experiment_1`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_1). Our first working implementation is a multiprocess solution, and it will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_1 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Motivation/Rationale
Solution_0 has one process which constantly accepts incoming requests and forks off new processes to immediately begin handling each one. Solution_1 has one thread which constantly accepts incoming requests and starts new threads to immediately begin handling each one. Both solutions can potentially have an unchecked number of requests being processed pseudo-concurrently. We hypothesize that Solution_1 will run faster than Solution_0 simply due to assumption that creating a new thread is computationally faster than forking a new process because less memory needs to be copied since threads share a process's memory while forked processes require the entire process's memory to be copied, requiring more overhead time.

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
The results of our experiment above show that on average Solution_1 scored very similarly to Solution_0. Furthermore, after performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of 0.3956, so we fail to reject the null hypothesis that Solution_1 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment. Therefore, the difference in average score of our samples is not statistically significant enough to suggest that Solution_1 generally exhibits better performance than Solution_0.

### Discussion
The conclusion provides evidence that simply replacing a multiprocess approach with a multithreaded approach does not alone significantly improve performance. This conclusion makes sense when we remember that we only expected a performance improvement by reducing the overhead of creating new processes. This is a very small difference, and more importantly, this small improvement only occurs once per request (maybe just a thousand times depending on the client run configuration). It seems likely that if there is some meaningful improvement as predicted in the rationale, it would require an impractically large number of requests to become significant in this context. Furthermore, even though the results were insufficient for rejecting our null hypothesis or directly improving our final solution, the experiment was still very important to our project because it laid the foundation for future experiments which will be based on multithreading.

### Improvements
In the future, it would be helpful for such an experiment to have an automated script to run the client several times and automatically collect the results into a usable format instead of manually starting each run and recording the score by hand. This would allow us to more practically increase our sample size and thus increase the statistical significance of our results. Also, both solutions currently allow for an unchecked number of threads/processes to run pseudo-concurrently, meaning a non-negligible amount of time is devoted to switching between them. Future solutions will be devised which reduce the amount of time spent switching between threads/processes.


## Experiment 2: Optimization for Multi-Core Processor - Sam Lonneman (s216999)

### Background
The goal of this experiment is to take advantage of the fact that our server will be run on a machine with a multi-core processor with a known number of cores. Furthermore, we will devise a solution which is optimized for 4 cores since this is the specification of the machine used to evaluate our server. We refer to this solution as Solution_2, and it's code can be found on a branch called [`Experiment_2`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_2). Our first working implementation will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_2 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Motivation/Rationale
Solution_0 works such that a single process continuously awaits requests from the client and forks a new process to calculate and return a result for each incoming request. This means that it is possible for there to be a large unchecked number of processes running pseudo-concurrently. However, once there are more processes running than cores available, the scheduler will cause the cores to regularly switch between processes, which consumes a non-negligible amount of time. As we increase the number of processes running pseudo-concurrently, this time spent switching begins to add up, hurting the performance of our server. In Solution_2, we hope to reduce this time spent switching by only having a distinct pool of 4 processes running concurrently rather than forking a new pseudo-concurrent process for every request. We implemented this by forking into four processes. Each process runs concurrently on its own core, first picking up a request, calculating and returning the result to the client, then picking up a new request and repeating these steps. This helps to ensure that each core is allowed to spend more time reverse hashing relative to the time spent switching between processes, hopefully improving the overall performance of our server.

### Variables
The independent variable in this experiment is whether the solution is optimized for a 4 core system (Solution_0 vs Solution_2). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run [`test.sh`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/test.sh) on the client 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_0) | Score (Solution_2) |
|:-------:|:------------------:|:------------------:|
|    1    |      7021784       |      4783660       |
|    2    |      7144965       |      4723195       |
|    3    |      7039684       |      4817164       |
|    4    |      6956718       |      4654059       |
|    5    |      6913224       |      4935756       |
|    6    |      7096031       |      4677250       |
|    7    |      6835940       |      4827054       |
|    8    |      7026563       |      4681235       |
|    9    |      6934364       |      4827064       |
|   10    |      6900494       |      4727903       |
| Average |      6986977       |      4765434       |

### Conclusion
As shown by the results above, the average score of Solution_2 was lower than the average score of Solution_0. After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0001%, so we can reject the null hypothesis that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that limiting the number of processes to the number of cores is statistically significantly faster than creating a new process for each request. This result makes sense and supports our hypothesis for the reason explained in our rationale.

### Improvements
In Solution_2, each process reverse hashes the request which is next in the FIFO queue from the client. Perhaps this solution could be improved by storing each request in a priority queue and instructing each process to take the oldest request of the highest priority.


## Experiment 3: Priority Scheduling with "Nice Values" - Sam Lonneman (s216999)

### Background
The goal of this experiment is to improve performance by prioritizing high priority requests as designated by the client. To do so, we have implemented a solution which takes advantage of the Linux concept of "nice values". We refer to this solution as Solution_3, and its code can be found on a branch called [`Experiment_3`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_3). Our first working implementation will serve as the control for this experiment. We refer to this control solution as Solution_0, and it's code can be found on a branch called [`milestone`](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone).

### Hypothesis
We hypothesize that Solution_3 will run faster (score lower) than Solution_0 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_3 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Motivation/Rationale
Solution_0 starts a new process immediately for each request, ignoring the priority value provided by the client. Solution_3 also starts a new process immediately for each request, but it uses this provided priority value to adjust the "nice value" of the process. In Linux, the "nice value" of a process corresponds to how "nice" it is to other processes regarding scheduling. To make this more concrete, a process with a higher nice value is less likely to be selected when a CPU switches to a new process. The result is that by increasing the nice value of a process, we can effectively lower its priority. In Solution_3, we increase the nice value of each process by 16-p (since it is often unwise to decrease the priority of a user program). This means the nice value of a process with priority p=16 is unchanged, the nice value of a process with p=15 is increased by 1, etc. By making this adjustment, higher priority requests are now more likely to be selected than lower priority requests each time a core switches processes, resulting in a lower latency. Since the score for each request is proportional to the priority of the request, we expect Solution_3 will have a lower total score, as reflected in our alternative hypothesis.

### Variables
The independent variable in this experiment is whether request priority is ignored (Solution_0) or handled by adjusting nice values (Solution_3). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run [`test.sh`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/test.sh) on the client 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_0) | Score (Solution_3) |
|:-------:|:------------------:|:------------------:|
|    1    |      7021784       |      6800262       |
|    2    |      7144965       |      6705871       |
|    3    |      7039684       |      6942670       |
|    4    |      6956718       |      6705805       |
|    5    |      6913224       |      6782878       |
|    6    |      7096031       |      6753315       |
|    7    |      6835940       |      6696423       |
|    8    |      7026563       |      6686363       |
|    9    |      6934364       |      6845892       |
|   10    |      6900494       |      6687129       |
| Average |      6986977       |      6760661       |

### Conclusion
As shown by the results above, the average score of Solution_3 was lower than the average score of Solution_0. After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0001%, so we can confidently reject the null hypothesis that Solution_3 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that priority scheduling with "nice values" is statistically significantly faster than ignoring priority. This result makes sense. After all, the same number of processes is created in the same order as before. The only difference is that the scheduler now selects higher priority requests more often than lower priority requests, improving performance without any significant downside. The difference in performance could of course be exaggerated by using a client configuration with a lower priority lambda causing a greater spread of priorities, but we chose to use `test.sh` since it is a lightweight simulation of the final submission run configuration. Note that this "nice value" method is only useful in situations where there are more processes than available cores, a situation which may already be undesirable due to the increased time spent switching between processes. Since many of our future solutions are optimized by limiting the process/thread pool to the number of cores, this solution quickly becomes obsolete and will likely not make it into our final solution.


## Experiment 5: Request Priority Queue - Sam Lonneman (s216999)

### Background
The goal of this experiment is to optimize the server such that requests are processed in order of priority using a request priority queue (implemented as a custom max heap data structure. This solution is built off of Solution_1, our first naive multithreaded solution. This new solution takes it a step further, limiting the size of the thread pool to the number of cores to reduce excess switching. Thus, it can also be said to draw inspiration from Solution_2, where we implemented this same concept with processes. We refer to this new solution as Solution_5, and it's code can be found on a branch called [Experiment_5](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_5). Since we expect this new solution to be a direct improvement to Solution_2, we will use Solution_2 as the baseline for this experiment. The code for Solution_2 can be found on a branch called [Experiment 2](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_2).

### Hypothesis
We hypothesize that Solution_5 will run faster (score lower) than Solution_2 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_5 will run slower than or equal to (score higher than or equal to) Solution_2 in the test environment.

### Motivation/Rationale
Solution_2 works such that each time one of the four worker processes finishes processing a request, it accepts the very next request available from the client and begins processing it. This means that while Solution_2 is optimized for a four core system, it does not take request priority into account, instead acting as a FIFO system. Solution_5 aims to have a similar format except that each time one of the 4 worker threads completes a request, it begins processing the next highest priority request available. This is made possible by having an additional thread simply responsible for accepting requests as soon as they are sent by the client and pushing them all into a priority queue (max heap). This custom data structure always maintains a state such that the request on top of the heap is the oldest request of the highest priority which has not yet been processed. Each time a thread finishes processing a request, it simply extracts the top of the shared priority queue and processes it. We are confident that this solution will perform well because of the max heap's fast O(log n) insertion and extraction time complexity where n is the number of requests left to be processed.

Note, ideally we would take advantage of a max heap's constant O(1) time complexity for peeking at the top without deletion. However, it is essential that the request is fully deleted from the heap before any thread begins processing it to ensure that two requests won't be assigned to the same job, which would hurt our reliability. So, it is in our best interest to use an extraction method which retrieves and deletes all at once, which unfortunately requires at least O(log n) time. Regardless, O(log n) retrieval time is still very fast especially considering that n only grows to around 1000, within reasonable conditions like those in the final submission run.

### Variables
The independent variable in this experiment is whether the solution is optimized with a request priority queue or not (Solution_5 vs Solution_2). The dependent variable will be the score returned from the client after running a test suite. To ensure that other variables are consistent, we will perform every test using the same run configuration with the same randomization seed on the same virtual machine running on the same computer.

### Method
For both implementations of the server, we will run [`test.sh`](https://github.com/SamLonneman/os-challenge-wavy-gang/blob/master/test.sh) on the client 10 times and take the average of the scores. We will then compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.

### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_2) | Score (Solution_5) |
|:-------:|:------------------:|:------------------:|
|    1    |      4783660       |      4250265       |
|    2    |      4723195       |      4215769       |
|    3    |      4817164       |      4143361       |
|    4    |      4654059       |      4176056       |
|    5    |      4935756       |      4049407       |
|    6    |      4677250       |      4258646       |
|    7    |      4827054       |      4085853       |
|    8    |      4681235       |      4151206       |
|    9    |      4827064       |      4160764       |
|   10    |      4727903       |      4176249       |
| Average |      4765434       |      4166758       |

### Conclusion
As shown by the results above, the average score of Solution_5 was lower than the average score of Solution_2. After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0001%, so we can reject the null hypothesis that Solution_5 will run slower than or equal to (score higher than or equal to) Solution_2 in the test environment.

### Discussion
The conclusion provides evidence that using a max heap priority queue to process the highest priority requests first yields statistically significantly better performance than a solution which processes requests in a FIFO manner indifferent to priority. This result makes sense and supports our hypothesis for the reason explained in our rationale. Similar to in Experiment_3, the difference in performance could of course be exaggerated by using a client configuration with a lower priority lambda causing a greater spread of priorities, but we chose to use `test.sh` since it is a lightweight simulation of the final submission run configuration.

### Improvements
While this solution provides more improvements and is our best implementation so far, it still pays no attention to repeated requests. To create this improvement, our final solution combines this solution with the caching system from the previous experiment.


## Experiment 6: Manual Request Priority Queue - Alana Vanzo (s221952)

### Experiment 6.3: Manual Priority Queue with Multithreading 
### Experiment 6.1: Manual Priority Queue with 4 Processes (priority over all other factors)

### Background
Experiment 6.1 was conducted first, as Experiment 6.3 worked to incorporate the priority queue more 
naturally such that it didn't take away from the rest of the components (average wait time, etc.).
The code for Experiment 6.1's server can be found on the branch "Experiment_6.1" and the code for Experiment 6.3's
server can be found on the branch "Experiment_6.3"

#### Experiment 6.3
This experiment stems off of experiment 1 where multithreading is introduced, with 
the addition of the consideration of priorities. A singular process is used to obtain key information 
for each request, which is then stored in a two-dimensional matrix in a position that is determined by its priority level.
After the first request is saved threads are created simultaneously as request information is saved such that the highest request
that each thread works on the highest priority request that has been retrieved and stored. 

#### Experiment 6.1 
This experiment purely priorities the priority level of each request as it will output the requests directly in order from highest priority to lowest priority
(regardless of when the request is received). It makes use of 4 processes to optimize for the four core system, as each process will begin ordering requests
based on priority level and processing them once all requests have been received in sync.
The purpose of this experiment is to isolate the importance of priority and test performance when priority is the most valued
variable. The program is running with multiple processes as well in an attempt to optimize score where possible while not
taking any focus away from priority level.


### Hypothesis
#### Experiment 6.3
The hypothesis for this experiment is that performance will be improved when compared to 
the performance of experiment one due to the priorities being taken into consideration. 
Both programs should have a very similar speed as long as looping over requests doesn't 
increase time a large amount - hence the key difference will lie in whether the extra time required 
to loop over requests is made up for in higher priorities being handled more quickly. 

#### Experiment 6.1
This test will likely yield a higher score than experiment 6.3 as although the requests will be outputted in descending
order from highest priorities, on average request time from start to finish for each request will take much longer due
to every request having to wait for all requests to be loaded in and saved.

### Variables 
#### Experiment 6.3 
Independent Variable: Priority arrays with multithreading 
\
Controlled Variable: Multithreading - consistent with experiment 1
\
Dependent Variable: Performance score


#### Experiment 6.1 
Independent Variable: Introduction of priority arrays
\
Controlled Variable: Multiple Processes - consistent with Milestone 
\
Dependent Variable: Performance score

### Method 
For both implementations of the server, we will run test.sh 10 times and take the average of the scores. We will then 
compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform 
a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.
Note that test.sh has been altered to the run configuration described in the section below. 

### Run Configuration 
SERVER=192.168.101.10
\
PORT=5003
\
SEED=3435245
\
TOTAL=100
\
START=1
\
DIFFICULTY=300000
\
REP_PROB_PERCENT=20
\
DELAY_US=10
\
PRIO_LAMBDA=0.2

Note that a different client run configuration has been used as to test the priorities in an enviornment
where most priorities are obtained. As storing the values of each request takes time, theoretically if all requests were to
have the same priority then there is no point in taking the time to store values and order. All tests in the results
table have been run under the same client run configuration for consistency.



### Results
|   Run   | Experiment 6.1 |  Experiment 6.3  | Experiment 1 (multithreading) | Milestone (4 Processes) |
|:-------:|:--------------:|:----------------:|:-----------------------------:|:-----------------------------:|
|    1    |    20531239    |     13981242     |           13059565            |       12125862            |
|    2    |    24377169    |     11725117     |           13776204            |       13490430            |
|    3    |    25979897    |     12993175     |           13418582            |       13636356            |
|    4    |    26949927    |     12156278     |           14793607            |       13897344            |
|    5    |    26348427    |     13715916     |           13273441            |       14376678            |
|    6    |    26309607    |     13395847     |           13675835            |       13279553            |
|    7    |    26522276    |     13546126     |           14633293            |       14786374            |
|    8    |    27009276    |     12068886     |           13387746            |       13800504            |
|    9    |    27395650    |     13875488     |           13774564            |       13497668            |
|   10    |    25919462    |     13080453     |           13895749            |       14355790            |
| Average |    25734293    |    13053852.8    |          13768858.6           |       13771271            |

### Conclusion
Note that single-sided p-values/tests are being used as the assumptions made in the null hypothesis all imply that 
one experiment is likely to be faster than another - hence we are not testing both ways. 


#### Experiment 6.1 VS Experiment 6.3
The two-tailed P value is less than 0.0001
\
t = 18.5448
\
p < 0.00005 (one-sided test)
\
\
The results above show that the average score of Experiment 6.3 was lower than the average score of Experiment 6.1, suggesting
that experiment 6.3 yields better performance than Experiment 6.1. 
After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0001%, 
confirming that the implications suggested by our average results are also statistically accurate as we can 
reject the null hypothesis that Experiment 6.3 will score higher than or equal to Experiment 6.1 in the test environment.



#### Experiment 6.3 VS Experiment 1
The two-tailed P value equals 0.0338
\
t = 2.2974
\
p = 0.0169 (one-sided test)
\
\
The results above show that the average score of Experiment 6.3 was lower than the average score of Experiment 1, suggesting
that experiment 6.3 yields better performance than Experiment 1.
After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0169,
confirming that the implications suggested by our average results are also statistically accurate as we can
reject the null hypothesis that Experiment 6.3 will score higher than or equal to Experiment 1 in the test environment.

#### Experiment 6.1 VS Milestone
The two-tailed P value is less than 0.0001
t = 17.7814
p = 0.00005 (single-sided)

The results above show that the average score of Experiment 6.1 was higher than the average score of the milestone, suggesting
that experiment 6.1 does not have improved performance than when compared to the milestone. 
After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0169,
confirming that the implications suggested by our average results are also statistically accurate as we cannot 
reject the null hypothesis that Experiment 6.1 will score higher than or equal to the Milestone in the test environment.



### Discussion
The conclusion confirms that while priority is extremely important, the best performance is achieved when both 
priority and average wait time is taken into consideration. The best performance was obtained in Experiment 6.3 where 
a priority queue is created simultaneously as requests are processed. Experiment 6.1 purely focused on priority and as a
result its performance was significantly worse than when priority was disregarded completely (Milestone and Experiment 1).
The evident weakness in Experiment 6.1 is due to the delay between receiving the first priority and sending requests back,
as the processes will not send a request back until all requests have been received and ordered. Therefore, while the time taken
for the whole program to run will be very similar to a four process system without priority ordering (Milestone), the time 
delay will be very high for low priority requests received early on as they will then be processed last. Experiment 6.3
did prove to work on this issue as requests began processing at the same time the queue is created.
Furthermore, as these tests only receive 100 requests from the client, the improvements seen across average scores are likely to 
extend to a greater difference as the sample size increases due to the t-tests proving our predictions to be 
statistically correct. 


### Improvements
#### Experiment 6.3 
This experiment disregards repetition of requests, therefore a truly optimised server will also consider this. 
Furthermore, there is still the restriction that low priority requests loaded in early have to wait a long time for higher priority
requests to be loaded and sent back before being worked on. This could potentially be resolved by setting up 
an interrupt system where requests are put to the top of the priority lists if they have been waiting for too long 
after being loaded in. 

#### Experiment 6.1 
This server could be improved by beginning having one process in charge of scheduling while other
processes or threads begin sending back requests simultaneously. This would ensure that all requests do not need to wait for
every request to be loaded before beginning process time. Furthermore, as the server is currently set up it needs to be manually adjusted based on
how many requests will be sent through - this is not ideal for situations where the client will be
sending through an unknown number of requests. Experiment 6.3 utilizes different implementation so that it is not
restricted in both of these ways. 
