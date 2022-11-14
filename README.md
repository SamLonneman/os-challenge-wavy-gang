# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 6.3: Manual Priority Queue with Multithreading 

### Background
This experiment stems off of experiment 1 where multithreading is introduced, with 
the addition of the consideration of priorities. A singular process is used to obtain key information 
for each request, which is then stored in a two-dimensional matrix in a position that is determined by its priority level.
After the first request is saved threads are created simultaneously as request information is saved such that the highest request
that each thread works on the highest priority request that has been retrieved and stored. 


### Hypothesis
The hypothesis for this experiment is that performance will be improved when compared to 
the performance of experiment one due to the priorities being taken into consideration. 
Both programs should have a very similar speed as long as looping over requests doesn't 
increase time a large amount - hence the key difference will lie in whether the extra time required 
to loop over requests is made up for in higher priorities being handled more quickly. 

### Variables 
Independent Variable: Introduction of priority arrays
Controlled Variable: Multithreading - consistent with experiment 1
Dependent Variable: Performance score 



### Method 
For both implementations of the server, we will run test.sh 10 times and take the average of the scores. We will then 
compare the average scores of each implementation to determine which implementation runs faster. Finally, we will perform 
a hypothesis test with alpha=0.05 to determine whether the difference in scores is statistically significant given our sample size.


### Results
The results of this experiment can be found in the table below.

|   Run   | Score (Solution_6.3) |      Score (Solution_1)       |
|:-------:|:--------------------:|:-----------------------------:|
|    1    |       11773654       |           22082119            |
|    2    |       11477175       |           21938293            |
|    3    |       11635787       |           12133698            |
|    4    |       32359716       |           11322790            |
|    5    |       21773045       |           11731503            |
|    6    |       21886657       |           11658533            |
|    7    |       21582834       |           11503608            |
|    8    |       21488221       |           12456861            |
|    9    |       21113560       |           11387776            |
|   10    |       21166621       |           11741256            |
| Average |          X           |               X               |


### Conclusion 


### Discussion 


### Improvement 
This experiment disregards repetition of requests, therefore a truly optimised server will also consider this. 
