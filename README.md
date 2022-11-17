# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 6.1: Priority Over all Other Factors

### Background
This experiment purely priorities the priority level of each request as it will output the requests directly in order from highest priority to lowest priority
(regardless of when the request is received). It makes use of 4 processes to optimize for the four core system, as each process will begin ordering requests
based on priority level and processing them once all requests have been received in sync. 

### Hypothesis 
This test will likely yield a higher score than experiment 6.3 as although the requests will be outputted in descending
order from highest priorities, on average request time from start to finish for each request will take much longer due 
to every request having to wait for all requests to be loaded in and saved. 

### Purpose 
The purpose of this experiment is to isolate the importance of priority and test performance when priority is the most valued 
variable. The program is running with multiple processes as well in an attempt to optimize score where possible while not 
taking any focus away from priority level.

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
#### Experiment 6.1 VS Milestone 
The two-tailed P value is less than 0.0001
t = 18.5720

#### Experiment 6.1 VS Experiment 6.3 
The two-tailed P value is less than 0.0001
t = 18.5448

#### Experiment 6.3 VS Experiment 1
The two-tailed P value equals 0.0338
t = 2.2974

### Discussion
As suspected the results from experiment 1 are much worse than other experiments. The evident weakness in this experiment resides in the delay between receiving the first priority and sending requests back,
as the processes will not send a request back until all requests have been received and ordered. Therefore, while the time taken 
for the whole program to run will be very similar to a four process system without priority ordering, the time delay will be very high
for low priority requests received early on as they will then be processed last. 

### Improvements
This server could be improved by beginning having one process in charge of scheduling while other 
processes or threads begin sending back requests simultaneously. This would ensure that all requests do not need to wait for
every request to be loaded before beginning process time. Furthermore, as the server is currently set up it needs to be manually adjusted based on 
how many requests will be sent through - this is not ideal for situations where the client will be 
sending through an unknown number of requests. Experiment 6.3 utilizes different implementation so that it is not 
restricted in both of these ways. 
