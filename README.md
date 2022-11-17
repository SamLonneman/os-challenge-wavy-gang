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
#### Experiment 6.1 VS Experiment 6.3
The two-tailed P value is less than 0.0001
t = 18.5448

#### Experiment 6.3 VS Experiment 1
The two-tailed P value equals 0.0338
t = 2.2974

### Discussion
As expected the performance of experiment 


### Improvement 
This experiment disregards repetition of requests, therefore a truly optimised server will also consider this. 
Furthermore, there is still the restriction that low priority requests loaded in early have to wait a long time for higher priority
requests to be loaded and sent back before being worked on. This could potentially be resolved by setting up 
an interrupt system where requests are put to the top of the priority lists if they have been waiting for too long 
after being loaded in. 