# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 5: Request Priority Queue - Sam Lonneman (s216999)

### Background
The goal of this experiment is to optimize the server such that requests are processed in order of priority using a request priority queue (implemented as a custom max heap data structure). This solution is built off of Solution_1, our first naive multithreaded solution. This new solution takes it a step further, limiting the number of worker threads to the number of cores to reduce excess switching. Thus, it can also be said to draw inspiration from Solution_2, where we implemented this same concept with processes. We refer to this new solution as Solution_5, and it's code can be found on a branch called [Experiment_5](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_5). Since we expect this new solution to be a direct improvement to Solution_2, we will use Solution_2 as the baseline for this experiment. The code for Solution_2 can be found on a branch called [Experiment 2](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/Experiment_2).

### Hypothesis
We hypothesize that Solution_5 will run faster (score lower) than Solution_2 in the test environment. Furthermore, the null hypothesis for this experiment is that Solution_5 will run slower than or equal to (score higher than or equal to) Solution_2 in the test environment.

### Rationale
Solution_2 works such that each time one of the four worker processes finishes processing a request, it accepts the next request available from the client and begins processing it. This means that while Solution_2 is optimized for a four core system, it does not take request priority into account, instead acting as a FIFO system. Solution_5 aims to have a similar format except that each time one of the 4 worker threads completes a request, it begins processing the next highest priority request available. This is made possible by having an additional thread simply responsible for accepting requests as soon as they are sent by the client and pushing them all into a priority queue (max heap). This custom data structure always maintains a state such that the request on top of the heap is the earliest request received of the highest priority which has not yet been processed. Each time a thread finishes processing a request, it simply extracts the top of the shared priority queue and processes it. We are confident that this solution will perform well because of the max heap's fast O(log n) insertion and extraction time complexity where n is the number of requests left to be processed.

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
As shown by the results above, the average score of Solution_5 was lower than the average score of Solution_2. After performing a one-tailed statistical hypothesis t-test with alpha=0.05, we received a P-value of less than 0.0001%, so we can certainly reject the null hypothesis that Solution_2 will run slower than or equal to (score higher than or equal to) Solution_0 in the test environment.

### Discussion
The conclusion provides evidence that using a max heap priority queue to process the highest priority requests first yields statistically significantly better performance than a solution which processes requests in a FIFO manner indifferent to priority. This result makes sense and supports our hypothesis for the reason explained in our rationale.

Also note, ideally we would take advantage of a max heap's constant O(1) time complexity for peeking at the top without deletion. However, it is essential that the request is fully deleted from the heap before any request begins processing it to ensure that two requests won't be assigned to the same job, which would hurt our reliability. So, it is in our best interest to use an extraction method which retrieves and deletes all at once, which unfortunately requires at least O(log n) time. Regardless, O(log n) retrieval time is still very fast especially considering that n only grows to around 1000 at most, within reasonable conditions like those in the final submission run.

### Improvements
While this solution provides more improvements and is our best implementation so far, it still pays no attention to repeated requests. Perhaps this could still be improved by implementing some kind of cache shared between the worker threads.