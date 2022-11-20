# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff


## Experiment 6: Manual Priority Queue
### Experiment 6.3: Manual Priority Queue with Multithreading 
### Experiment 6.1: Manual Priority Queue with 4 Processes (priority over all other factors)

### Background
Experiment 6.1 was conducted first, as Experiment 6.3 worked to incorporate the priority queue more 
naturally such that it didn't take away from the rest of the components (average wait time, etc.).

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
Independent Variable: Introduction of priority arrays
\
Controlled Variable: Multithreading - consistent with experiment 1
\
Dependent Variable: Performance score


#### Experiment 6.1 



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


### Improvement 
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